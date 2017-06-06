package main

import (
	"fmt"
	"os"
	"encoding/binary"
)

var f *os.File

func main() {
	var rofl_header roflHeader

	f, err := os.Open(os.Args[1])
	if err != nil {
		panic(err)
	}

	binary.Read(f, binary.LittleEndian, &rofl_header)

	f.Close()
}
