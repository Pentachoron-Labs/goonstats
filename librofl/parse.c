#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <json-c/json.h>

#include "rofl.h"

/**
 * Reads the file header from a .rofl file into the given header struct,
 * performing checks for consistency.
 * 
 * RETURNS: 1 if successful, 0 if there was an error reading the file, or
 * the file is not a valid ROFL file.
 */
int rofl_read_file_header(FILE *f, rofl_file_header *header) {
    static const uint8_t rofl_magic[6] = {'R','I','O','T',0,0};

    fseek(f, 0, SEEK_SET);
    if (fread(header, sizeof(*header), 1, f) != sizeof(*header))
        return 0;

    /* check some values */
    if (memcmp(header->magic, rofl_magic, sizeof(rofl_magic))) return 0;
    if (header->file_header_length != 288) return 0;
    if (header->meta_offset != 288) return 0;
    if (header->header_length != 66) return 0;
    if (header->header_offset != header->meta_length + header->header_offset) return 0;

    return 1;
}

/**
 * As above, with the game data header.
 */
int rofl_read_header(FILE *f, long offset, rofl_header *header) {
    fseek(f, offset, SEEK_SET);
    if (fread(header, sizeof(*header), 1, f) != sizeof(*header))
        return 0;

    /* check some values */
    if (header->keyframe_interval != 60000) return 0;
    if (header->cryptkey_length != 32) return 0;

    return 1;
}

/**
 * As above, with the JSON metadata header.
 */
int rofl_read_metadata_header(FILE *f, long offset, long length) {
    char *buffer = malloc(length+1);
    json_object *obj;

    /* Read into the buffer */
    fseek(f, offset, SEEK_SET);
    if (fread(buffer, 1, length, f) != length)
        return 0;
    buffer[length] = 0; /* null term */

    /* Parse */
    obj = json_tokener_parse(buffer);
    if (!obj) return 0;

    /* TODO: do something */

    json_object_put(obj);   /* free */
    free(buffer);
    return 1;
}
