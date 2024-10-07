#pragma once

#define PAGE_SIZE 4096

#define INDEX_FROM_BIT(a) (a / (8*4))
#define OFFSET_FROM_BIT(a) (a % (8*4))

#include "multiboot.h"
#include "ctypes.h"

typedef struct page{
    uint32_t present: 1;
    uint32_t rw: 1;
    uint32_t user: 1;
    uint32_t accessed: 1;
    uint32_t dirty: 1;
    uint32_t unused: 7;
    uint32_t frame: 20;
}__attribute__((packaged)) page_t;

void page_init(multiboot_t *mboot);