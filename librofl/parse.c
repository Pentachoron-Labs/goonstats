#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "rofl.h"

/**
 * Reads the file header from a .rofl file into the given header struct,
 * performing checks for consistency.
 * 
 * RETURNS: 1 if successful, 0 if there was an error reading the file, or
 * the file is not a valid ROFL file.
 */
int rofl_read_file_header(FILE *f, rofl_file_header *header) {
    static const uint8_t rofl_magic[6] = {'R','O','F','L',0,0};

    fseek(f, 0, SEEK_SET);
    if (fread(&header, sizeof(rofl_file_header), 1, f) != sizeof(rofl_file_header))
        return 0;

    /* check some values */
    if (memcmp(header->magic, rofl_magic, 6)) return 0;
    if (header->file_header_length != 288) return 0;
    if (header->meta_offset != 288) return 0;
    if (header->header_length != 66) return 0;

    return 1;
}
