#pragma once

#include "ctypes.h"

typedef struct {
    void (*Read)(char drive, uint8_t *buffer, uint32_t number,uint32_t lba);
    void (*Write)(char drive, uint8_t *buffer, uint32_t number,uint32_t lba);
    int flag;
    uint32_t size; // 大小
    uint32_t  sector_size;
    char DriveName[50];
    char disk_id[50];
} vdisk;

int init_vdisk();
int register_vdisk(vdisk vd);
int logout_vdisk(int drive);
int rw_vdisk(int drive, uint32_t lba, uint8_t *buffer, uint32_t number, int read);
bool have_vdisk(int drive);
vdisk *get_disk(int drive);
uint32_t disk_Size(int drive);

bool SetDrive(uint8_t *name);
uint32_t GetDriveCode(uint8_t *name);
bool DriveSemaphoreTake(uint32_t drive_code);
void DriveSemaphoreGive(uint32_t drive_code);
void Disk_Read(uint32_t lba, uint32_t number, void *buffer,char drive);
bool CDROM_Read(uint32_t lba, uint32_t number, void *buffer,char drive);
bool DiskReady(char drive);
int getReadyDisk();
void Disk_Write(uint32_t lba, uint32_t number, void *buffer,char drive);