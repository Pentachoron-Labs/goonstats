#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <json-c/json.h>

#include "rofl.h"

#ifdef DEBUG
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...)
#endif

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
    rofl_metadata metadata = {0};
    char *buffer = malloc(length+1);
    json_object *obj;
    char *stats_json = NULL;
    int ret;

    /* Read into the buffer */
    fseek(f, offset, SEEK_SET);
    if (fread(buffer, 1, length, f) != length)
        return 0;
    buffer[length] = 0; /* null term */

    /* Parse */
    obj = json_tokener_parse(buffer);
    if (!obj) {
        free(buffer);
        return 0;
    }

    json_object_object_foreach(obj, key, val) {
        /* char *key */
        /* json_object *val */
        if (!strcmp(key, "gameLength"))
            metadata.game_length = json_object_get_int(val);
        else if (!strcmp(key, "gameVersion"))
            sscanf(json_object_get_string(val), "%d.%d.%d.%d",
                metadata.game_version[0],
                metadata.game_version[1],
                metadata.game_version[2],
                metadata.game_version[3]);
        else if (!strcmp(key, "lastGameChunkId"))
            metadata.last_chunk_id = json_object_get_int(val);
        else if (!strcmp(key, "lastKeyFrameId"))
            metadata.last_keyframe_id = json_object_get_int(val);
        else if (!strcmp(key, "statsJson"))
            stats_json = strdup(json_object_get_string(val));
        else
            /* found an unrecognized member */
            debug("unrecognized metadata key \"%s\"\n", key);
    }

    /* check that we have all of the elements */
    if (metadata.game_length == 0)
        debug("missing or zero \"gameLength\" member\n");
    /* fixme: check version */
    if (metadata.last_chunk_id == 0)
        debug("missing or zero \"lastGameChunkId\" member\n");
    if (metadata.last_keyframe_id == 0)
        debug("missing or zero \"lastKeyFrameId\" member\n");
    if (!stats_json) {
        debug("fatal: missing \"statsJson\" member\n");
        ret = 0;
        goto end;
    }

    json_object_put(obj);   /* free */

    /* parse the statsJson */
    obj = json_tokener_parse(stats_json);
    if (!obj) {
        free(buffer);
        return 0;
    }

    

end:
    json_object_put(obj);   /* free */
    free(buffer);
    return ret;
}
