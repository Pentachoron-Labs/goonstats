#ifndef __ROFL_H
#define __ROFL_H

#include <stdint.h>

#define STATIC_ASSERT(e) extern void STATIC_ASSERT_(int [(e)?1:-1])


/* File structures */

#pragma pack(1)

typedef struct {
    uint8_t     magic[6];           /* RIOT\0\0 */
    uint8_t     signature[256];
    uint16_t    file_header_length; /* should be 288 */
    uint32_t    file_length;
    uint32_t    meta_offset;        /* should be 288 */
    uint32_t    meta_length;
    uint32_t    header_offset;
    uint32_t    header_length;      /* should be 66 */
    uint32_t    data_offset;
} rofl_file_header;

STATIC_ASSERT(sizeof(rofl_file_header) == 288);

typedef struct {
    uint64_t    gameid;
    uint32_t    game_length;
    uint32_t    keyframe_count;
    uint32_t    chunk_count;
    uint32_t    unknown1;           /* "end startup chunk ID"? */
    uint32_t    unknown2;           /* "start game chunk ID"? */
    uint32_t    keyframe_interval;  /* should be 60000 */
    uint16_t    cryptkey_length;    /* should be 32 */
    uint8_t     cryptkey[32];
} rofl_header;

STATIC_ASSERT(sizeof(rofl_header) == 66);

typedef struct {
    long game_length;
    int  game_version[4];
    int  last_chunk_id;
    int  last_keyframe_id;
} rofl_metadata;


/* Exported functions */

extern int rofl_read_file_header(FILE *, rofl_file_header *);

#endif /* __ROFL_H */
