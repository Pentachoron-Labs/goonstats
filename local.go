package main

import (
	"fmt"
	"os"
	"encoding/binary"
)

var f *os.File

func main() {
	var rofl_header roflHeader
	var payload_header payloadHeader
	var payload_list []payloadEntry
	var payload_length int

	f, err := os.Open(os.Args[1])
	if err != nil {
		panic(err)
	}

	/* Read headers */

	binary.Read(f, binary.LittleEndian, &rofl_header)
	/* todo: parse metadata */
	f.Seek(rofl_header.PayloadHeaderOffset, SEEK_SET)
	binary.Read(f, binary.LittleEndian, &payload_header)
	payload_length = payload_header.KeyframeCount + payload_header.ChunkCount
	make([]payloadEntry, payload_length, payload_length)
	binary.Read(f, binary.LittleEndian, payload_list)

	f.Close()
}
