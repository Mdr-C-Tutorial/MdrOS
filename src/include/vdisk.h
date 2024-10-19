#pragma once

#include "ctypes.h"

typedef struct {
    void (*Read)(char drive, uint8_t *buffer, uint32_t number,uint32_t lba);
    void (*Write)(char drive, uint8_t *buffer, uint32_t number,uint32_t lba);
    int flag;
    unsigned int size; // 大小
    char DriveName[50];
} vdisk;

int init_vdisk();
int register_vdisk(vdisk vd);
int logout_vdisk(char drive);
int rw_vdisk(char drive, uint32_t lba, uint8_t *buffer,uint32_t number, int read);
bool have_vdisk(char drive);
vdisk *get_disk(char drive);
uint32_t disk_Size(char drive);

bool SetDrive(uint8_t *name);
uint32_t GetDriveCode(uint8_t *name);
bool DriveSemaphoreTake(uint32_t drive_code);
void DriveSemaphoreGive(uint32_t drive_code);
void Disk_Read(uint32_t lba, uint32_t number, void *buffer,char drive);
bool CDROM_Read(uint32_t lba, uint32_t number, void *buffer,char drive);
bool DiskReady(char drive);
int getReadyDisk();
void Disk_Write(uint32_t lba, uint32_t number, void *buffer,char drive);