#pragma once

#define PAGE_SIZE 4096

#define INDEX_FROM_BIT(a) (a / (8*4))
#define OFFSET_FROM_BIT(a) (a % (8*4))

#include "multiboot.h"
#include "ctypes.h"

typedef struct page{
    uint8_t present: 1;
    uint8_t rw: 1;
    uint8_t user:1;
    uint8_t pwt:1; // 0回写模式 ; 1 直写模式
    uint8_t pcd:1; // 为1时禁止该页缓冲
    uint8_t accessed: 1;
    uint8_t dirty: 1;
    uint8_t pat: 1;
    uint8_t global: 1;
    uint8_t ignored: 3;
    uint32_t frame: 20;
}__attribute__((packaged)) page_t;

typedef struct page_table {
    page_t pages[1024];
}__attribute__((packaged)) page_table_t;

typedef struct page_directory {
    page_table_t volatile*tables[1024];
    uint32_t table_phy[1024];
    uint32_t physicalAddr;
}__attribute__((packaged)) page_directory_t;

void switch_page_directory(page_directory_t *dir);
void page_init(multiboot_t *mboot);