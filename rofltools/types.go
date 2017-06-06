package rofltools

/*
	Notes: .rofl files do not contain in-game chat. Probably means that chat messages are generated/carried by something else.
	This means we can't analyze flame. Oh well.
 */

/*
	Header for the whole .rofl is made of the magic string, file signature, header length, file length, metadata offset,
	metadata length, payload header offset, payload header length, and the payload offset

	The 288 bytes of the header are broken up like so:
		6, 256, 2, 4, 4, 4, 4, 4, 4
 */
type roflHeader struct {
	HeaderLength      int
	FileLength        int
	MetaOffset        int
	MetaLength        int
	PayloadHeadOffset int
	PayloadHeadLength int
	PayloadOffset     int
}

/*
	Header for the metadata
	It is 34 bytes long, plus the length of the encryption key.
	Broken up like so:
		8, 4, 4, 4, 4, 4, 4, 2 + length of key
 */
type payloadHeader struct {
	GameID            int
	GameLength        int
	KeyframeCount     int
	ChunkCount        int
	EndStartupChunkID int
	StartGameChunkID  int
	KeyframeInterval  int
	KeyLength         int
	Key               int
}

/*
	Header for individual chunks. This is the same between data chunks and keyframe chunks.
	17 bytes long, broken up like so:
		4, 1, 4, 4, 4
	The 5th byte is either 0 or 1. 0 indicates keyframe, 1 indicates data
	Payload headers are the same format.
 */
type chunkHeader struct {
	ChunkID     int
	IsGameData  bool
	ChunkLength int
	NextChunkID int
	Offset      int
}

func newRoflHeader() *roflHeader {
	return &roflHeader{}
}

func newPayloadHeader() *payloadHeader {
	return &payloadHeader{}
}

func newChunkHeader() *chunkHeader {
	return &chunkHeader{}
}
