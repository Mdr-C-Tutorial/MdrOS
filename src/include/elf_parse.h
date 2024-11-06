#pragma once

#include "ctypes.h"

typedef enum ElfParseResult_Tag {
  EntryPoint,
  InvalidElfData,
  ElfContainsNoSegments,
  FailedToGetSegmentData,
  AllocFunctionNotProvided,
} ElfParseResult_Tag;

typedef struct ElfParseResult {
  ElfParseResult_Tag tag;
  union {
    struct {
      size_t entry_point;
    };
  };
} ElfParseResult;

typedef struct Segment {
  size_t address;
  size_t size;
  const uint8_t *data;
} Segment;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct ElfParseResult parse_elf(const uint8_t *elf_data,
                                size_t elf_size,
                                void (*mapping_callback)(struct Segment segment));

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus
