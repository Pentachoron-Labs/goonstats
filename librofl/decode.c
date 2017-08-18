#include<stdio.h>

#include"decode.h"


int main(int argc, char** argv){
		char typeByte;
		
		FILE* keyframe;
		keyframe = fopen(argv[1], "rb");
		
		fread(&typeByte, 1, 1, keyframe);
		BlockMeta m;
		populateBlockMetadata(typeByte, &m);
		
		printf("%d", m.timeFormat);
		
		fclose(keyframe);
}

/*
 * Parses the bit flags in the marker byte.
 * ROFLs are little-endian, thankfully
 * If bit 1 is 0, timestamp is absolute (in s); otherwise it is relative to last block (in ms)
 * If bit 2 is 0, the block contains a type byte; otherwise no type byte
 * If bit 3 is 0, the header contains 4 blockparam bytes; otherwise only 1 blockparam byte
 * If bit 4 is 0, the header contains 4 contentlength bytes, otherwise only 1 contentlength byte
 * Bits 5-8 describe the kind of packet contained in the block. Not really necessary to parse here
 */
void populateBlockMetadata(char marker, BlockMeta* meta){
	meta->timeFormat = (marker&1) ? RELATIVE_LAST_BLOCK_TIME : ABSOLUTE_TIME;
	meta->containsTypeByte = !(marker&2);
	meta->blockParamBytes = (marker&4) ? 1 : 4;
	meta->contentLengthBytes = (marker&8) ? 1 : 4;
}
