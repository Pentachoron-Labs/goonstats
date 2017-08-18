/*
 * Describes the number of bytes associated with each timestamp format in a block
 */
typedef enum blockTimestampBytes{ABSOLUTE_TIME=4, RELATIVE_LAST_BLOCK_TIME=1} BlockTimestampFormat;

typedef struct{
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
	char marker;
	char type;
	float absoluteTimestamp;
	int relativeMs;
	char[] blockParam;
	char[] content;
}Block;	
	
void populateBlockMetadata(char marker, BlockMeta* meta);


