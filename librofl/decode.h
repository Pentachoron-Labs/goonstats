/*
 * Describes the number of bytes associated with each timestamp format in a block
 */
typedef enum blockTimestampBytes{ABSOLUTE_TIME=4, RELATIVE_LAST_BLOCK_TIME=1} BlockTimestampFormat;

typedef struct{
	char marker;
	BlockTimestampFormat timeFormat;
	int contentLengthBytes;
	int containsTypeByte;
	int blockParamBytes;
}BlockMeta;

/*
 * Block holds the metadata read from the first byte, the header, and content of a block
 */
typedef struct{
	BlockMeta metadata;
	char type;
	float absoluteTimestamp;
	int contentLength;
	
	//BlocParam is either 1 or 4 bytes long, so we just give it four bytes here
	//	and choose to leave bytes 1, 2, 3 as 0x00 if there is only 1 byte
	char blockParam[4];
	char* content;
}Block;	
	
/*
 * Fills in the given BlockMeta from a given data file.
 */
void populateBlockMetadata(BlockMeta* meta, FILE* dataFile);


/*
 * Populates a block with data from a file.
 * Block metadata must be set.
 */
void populateBlockFromFile(Block* block, float prevTime, char prevBlockparam[4], FILE* dataFile);

void printBlock(Block* block);

void printBlockMetadata(BlockMeta* blockMeta);
