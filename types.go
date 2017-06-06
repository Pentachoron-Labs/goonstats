package main

/* Notes: .rofl files do not contain in-game chat. Probably means
 * that chat messages are generated/carried by something else.
 * This means we can't analyze flame. Oh well.
 */

type roflHeader struct {
	Magic			[6]uint8	/* 52 49 4f 54 00 00 = RIOT\0\0 */
	Signature		[256]uint8	
	HeaderLength		uint16	/* should be 0x120 */
	FileLength		uint32
	MetaOffset		uint32	/* should be 0x120 */
	MetaLength		uint32
	PayloadHeaderOffset	uint32
	PayloadHeaderLength	uint32
	PayloadOffset		uint32
}

/* metadata header */
type metaHeader struct {
	GameID			uint64
	GameLength		uint32	/* ms */
	KeyframeCount		uint32
	ChunkCount		uint32
	EndStartupChunkID	uint32	/* ? */
	StartGameChunkID	uint32	/* ? */
	KeyframeInterval	uint32	/* should be 0xea60 = 60000 */
	EncryptionKeyLength	uint16	/* should be 0x20 */
	EncryptionKey[32]	uint8
}

/* list of entries for chunks and keyframes */
type payloadHeader struct {
	ID	uint32
	Type	uint8	/* 1 for chunk, 2 for keyframe */
	Length	uint32
	NextID	uint32	/* ignored if chunk */
	Offset	uint32	/* counted from payload offset */
}
