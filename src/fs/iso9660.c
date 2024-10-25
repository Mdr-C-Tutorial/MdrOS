#include "iso9660.h"
#include "vfs.h"
#include "krlibc.h"
#include "vdisk.h"

#define SEEK_END L9660_SEEK_END
#define SEEK_SET L9660_SEEK_SET
#define SEEK_CUR L9660_SEEK_CUR

#define l9660_seekdir(dir, pos) (l9660_seek(&(dir)->file, L9660_SEEK_SET, (pos)))
#define l9660_telldir(dir) (l9660_tell(&(dir)->file))

#ifdef L9660_BIG_ENDIAN
#define READ16(v) (((v).be[1]) | ((v).be[0] << 8))
#define READ32(v) (((v).be[3]) | ((v).be[2] << 8) | ((v).be[1]) << 16 | ((v).be[0] << 24))
#else
#define READ16(v) (((v).le[0]) | ((v).le[1] << 8))
#define READ32(v) (((v).le[0]) | ((v).le[1] << 8) | ((v).le[2]) << 16 | ((v).le[3] << 24))
#endif

#ifndef L9660_SINGLEBUFFER
#define HAVEBUFFER(f) (true)
#define BUF(f) ((f)->buf)
#else
#define HAVEBUFFER(f) ((f) == last_file)
#define BUF(f) (gbuf)
static l9660_file *last_file;
static char gbuf[2048];
#endif

static inline uint16_t fsectoff(l9660_file *f) { return f->position % 2048; }

static inline uint32_t fsector(l9660_file *f) { return f->position / 2048; }

static inline unsigned aligneven(unsigned v) { return v + (v & 1); }

static inline uint32_t fnextsectpos(l9660_file *f) {
    return (f->position + 2047) & ~2047;
}

static l9660_status buffer(l9660_file *f) {
#ifdef L9660_SINGLEBUFFER
    last_file = f;
#endif

    if (!f->fs->read_sector(f->fs, BUF(f), f->first_sector + f->position / 2048))
        return L9660_EIO;
    else
        return L9660_OK;
}

static l9660_status prebuffer(l9660_file *f) {
    if (!HAVEBUFFER(f) || (f->position % 2048) == 0)
        return buffer(f);
    else
        return L9660_OK;
}

static l9660_status openat_raw(l9660_file *child, l9660_dir *parent,
                               const char *name, bool isdir) {
    l9660_status rv;
    l9660_dirent *dent = NULL;
    if ((rv = l9660_seekdir(parent, 0)))
        return rv;

    do {
        const char *seg = name;
        name = strchrnul(name, '/');
        size_t seglen = name - seg;

        /* ISO9660 stores '.' as '\0' */
        if (seglen == 1 && *seg == '.')
            seg = "\0";

        /* ISO9660 stores ".." as '\1' */
        if (seglen == 2 && seg[0] == '.' && seg[1] == '.') {
            seg = "\1";
            seglen = 1;
        }

        for (;;) {
            if ((rv = l9660_readdir(parent, &dent)))
                return rv;

            /* EOD */
            if (!dent)
                return L9660_ENOENT;
            /* wrong length */
            if (seglen > dent->name_len)
                continue;

            /* check name */
            if (memcmp(seg, dent->name, seglen) != 0)
                continue;

            /* check for a revision tag */
            if (dent->name_len > seglen && dent->name[seglen] != ';')
                continue;

            /* all tests pass */
            break;
        }

        child->fs = parent->file.fs;
        child->first_sector = READ32(dent->sector) + dent->xattr_length;
        child->length = READ32(dent->size);
        child->position = 0;
        parent->file.position = 0;
        if (*name && (dent->flags & DENT_ISDIR) != 0)
            return L9660_ENOTDIR;

        parent = (l9660_dir *)child;
    } while (*name);

    if (isdir) {
        if ((dent->flags & DENT_ISDIR) == 0)
            return L9660_ENOTDIR;
    } else {
        if ((dent->flags & DENT_ISDIR) != 0)
            return L9660_ENOTFILE;
    }

    return L9660_OK;
}

l9660_status l9660_openfs(l9660_fs *fs,bool (*read_sector)(l9660_fs *fs, void *buf,uint32_t sector),uint8_t disk_number) {
    fs->read_sector = read_sector;
    fs->disk_number = disk_number;
#ifndef L9660_SINGLEBUFFER
    l9660_vdesc_primary *pvd = PVD(&fs->pvd);
#else
    last_file = NULL;
  l9660_vdesc_primary *pvd = PVD(gbuf);
#endif
    uint32_t idx = 0x10;
    for (;;) {
        if (!read_sector(fs, pvd, idx))
            return L9660_EIO;
        if (memcmp(pvd->hdr.magic, "CD001", 5) != 0)
            return L9660_EBADFS;

        if (pvd->hdr.type == 1)
            break; // Found PVD
        else if (pvd->hdr.type == 255)
            return L9660_EBADFS;
    }

#ifdef L9660_SINGLEBUFFER
    memcpy(&fs->root_dir_ent, &pvd->root_dir_ent, pvd->root_dir_ent.length);
#endif

    return L9660_OK;
}

l9660_status l9660_fs_open_root(l9660_dir *dir, l9660_fs *fs) {
    l9660_file *f = &dir->file;
#ifndef L9660_SINGLEBUFFER
    l9660_dirent *dirent = &PVD(&fs->pvd)->root_dir_ent;
#else
    l9660_dirent *dirent = &fs->root_dir_ent;
#endif

    f->fs = fs;
    f->first_sector = READ32(dirent->sector);
    f->length = READ32(dirent->size);
    f->position = 0;

    return L9660_OK;
}

l9660_status l9660_opendirat(l9660_dir *dir, l9660_dir *parent,const char *path) {
    return openat_raw(&dir->file, parent, path, true);
}

l9660_status l9660_readdir(l9660_dir *dir, l9660_dirent **pdirent) {
    l9660_status rv;
    l9660_file *f = &dir->file;

    rebuffer:
    if (f->position >= f->length) {
        *pdirent = NULL;
        f->position = 0;
        return L9660_OK;
    }

    if ((rv = prebuffer(f)))
        return rv;
    char *off = BUF(f) + fsectoff(f);
    if (*off == 0) {
        // Padded end of sector
        f->position = fnextsectpos(f);
        goto rebuffer;
    }

    l9660_dirent *dirent = (l9660_dirent *)off;
    f->position += aligneven(dirent->length);

    *pdirent = dirent;
    return L9660_OK;
}

l9660_status l9660_openat(l9660_file *child, l9660_dir *parent,const char *name) {
    return openat_raw(child, parent, name, false);
}

/*! Seek the file to \p offset from \p whence */
l9660_status l9660_seek(l9660_file *f, int whence, int32_t offset) {
    l9660_status rv;
    uint32_t cursect = fsector(f);

    switch (whence) {
        case SEEK_SET:
            f->position = offset;
            break;

        case SEEK_CUR:
            f->position = f->position + offset;
            break;

        case SEEK_END:
            f->position = f->length - offset;
            break;
    }

    if (fsector(f) != cursect && fsectoff(f) != 0) {
        if ((rv = buffer(f)))
            return rv;
    }

    return L9660_OK;
}

uint32_t l9660_tell(l9660_file *f) { return f->position; }

l9660_status l9660_read(l9660_file *f, void *buf, size_t size, size_t *read) {
    l9660_status rv;

    if ((rv = prebuffer(f)))
        return rv;

    uint16_t rem = 2048 - fsectoff(f);
    if (rem > f->length - f->position)
        rem = f->length - f->position;
    if (rem < size)
        size = rem;

    memcpy(buf, BUF(f) + fsectoff(f), size);

    *read = size;
    f->position += size;

    return L9660_OK;
}

bool read_sector(l9660_fs *fs, void *buf, uint32_t sector) {
    return CDROM_Read(sector, 1, buf, fs->disk_number);
}



static void dummy(){}

int isofs_mount(char* src, vfs_node_t node){

}

void isofs_open(void *parent, char* name, vfs_node_t node) {

}

int  isofs_read(void *file, void *addr, size_t offset, size_t size){

}

void isofs_close(void *current){

}

static struct vfs_callback callbacks = {
        .mount   = isofs_mount,
        .unmount = (void *)dummy,
        .mkdir   = (void *)dummy,
        .mkfile  = (void *)dummy,
        .open    = isofs_open,
        .close   = isofs_close,
        .stat    = (void *)dummy,
        .read    = isofs_read,
        .write   = (void *)dummy,
};
