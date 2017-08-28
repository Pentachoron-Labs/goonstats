#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>

#include"decode.h"


int main(int argc, char** argv){
	FILE* keyframe;			
	float blockTime = 0.0f;
	char blockP[4];
	Block b;
	
	keyframe = fopen(argv[1], "rb");	
	for(int blockCount = 0; blockCount < 3; blockCount++){
		
		populateBlockFromFile(&b, blockTime, blockP, keyframe);
		printBlockMetadata(&(b.metadata));
		//printBlock(&b);
		free(b.content);
		blockTime = b.absoluteTimestamp;
		memcpy(blockP, b.blockParam, 4);
	}
	fclose(keyframe);
}

/*
 * Parses the bit flags in the marker byte.
 * ROFLs are little-endian, thankfully
 * If bit 1 is 0, timestamp is absolute (in s); otherwise it is relative to last block (in ms)
 * If bit 2 is 0, the block contains a type byte; otherwise blocktype is the same as the previous block
 * If bit 3 is 0, the header contains 4 blockparam bytes; otherwise only 1 blockparam byte
 * If bit 4 is 0, the header contains 4 contentlength bytes, otherwise only 1 contentlength byte
 * Bits 5-8 describe the kind of packet contained in the block. Not really necessary to parse here
 */
void populateBlockMetadata(BlockMeta* meta, FILE* dataFile){
	fread(&(meta->marker), 1, 1, dataFile);
	
	meta->timeFormat = ((meta->marker)&1) ? RELATIVE_LAST_BLOCK_TIME : ABSOLUTE_TIME;
	meta->containsTypeByte = ((meta->marker)&2) ? 1 : 0;
	meta->blockParamBytes = ((meta->marker)&4) ? 1 : 4;
	meta->contentLengthBytes = ((meta->marker)&8) ? 1 : 4;
}


/*
 * Populate a block from a given data file.. Seek_cur is: %d\n", (int) ftell(dataFile));
 * Block format as follows:
 * marker + [time] + [content length] + (type) + [blockparam] + [content]
 * 1 byte | 1 or 4 | 1 or 4 bytes     | 0 or 1 | 1 or 4 bytes |  0 to n bytes
 */
void populateBlockFromFile(Block* block, float prevTime, char prevBlockparam[4], FILE* dataFile){
	//file stream points to just before the start of the block
	populateBlockMetadata(&(block->metadata), dataFile);
	//Marker byte has been read in
	printf("Read metadata. Seek_cur is: %d\n", (int) ftell(dataFile));
	
	//Now read in the timestamp (1 or 4 bytes)
	if(block->metadata.timeFormat == ABSOLUTE_TIME){
		fread(&(block->absoluteTimestamp), 4, 1, dataFile);
	}else{
		uint8_t millisDelta;
		fread(&millisDelta, 1, 1, dataFile);
		block->absoluteTimestamp = prevTime + (float) millisDelta;
	}
	printf("Read timestamp. Seek_cur is: %d\n", (int) ftell(dataFile));
	
	//read in the content length bytes
	fread(&(block->contentLength), block->metadata.contentLengthBytes, 1, dataFile);
	printf("Read content length. Seek_cur is: %d\n", (int) ftell(dataFile));
	
	//read in the (potential) type byte
	if(block->metadata.containsTypeByte){
		fread(&(block->type), 1, 1, dataFile);
		printf("Read type byte. Seek_cur is: %d\n", (int) ftell(dataFile));
	}else{
		block->type = 0;
	}
	
	//read in the blockparam
	fread(block->blockParam, block->metadata.blockParamBytes, 1, dataFile);
	if(block->metadata.blockParamBytes == 1){
			block->blockParam[1] = 0;
			block->blockParam[2] = 0;
			block->blockParam[3] = 0;
	}
	printf("Read blockparam. Seek_cur is: %d\n", (int) ftell(dataFile));
	//malloc and read in the content
	block->content = (char *) malloc(block->contentLength);
	fread(block->content, block->contentLength, 1, dataFile);
	
	printf("Read %d bytes of block content. Seek_cur is %d\n", block->contentLength, (int) ftell(dataFile));
}

void printBlock(Block* b){
	printf("Block timestamp: %f\n", b->absoluteTimestamp);
	printf("Block type byte: %02x \n", b->type);
	printf("Block content length: %d\n", b->contentLength);
	
	for(int i = 0; i< b->contentLength || i<16; i++){
			if(i%4 == 0){
				printf("\n");
			}
			printf("%02x ", b->content[i]);
	}
	printf("\n");
}

void printBlockMetadata(BlockMeta* bm){
	printf("--------\n");
	printf("Marker Byte: %02x\n", bm->marker);
	printf("Timestamp format: %d\n", bm->timeFormat);
	printf("Content length bytes: %d\n", bm->contentLengthBytes);
	printf("Type bytes: %d\n", bm->containsTypeByte);
	printf("Blockparam bytes: %d\n", bm->blockParamBytes);
	printf("END BLOCK META \n");
}
