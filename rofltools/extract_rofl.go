package rofltools

const (
	HEADER_SIZE = 288
	ROFL_MAGIC  = "RIOT\x00\x00"
)

var (
	magic_bytes = []byte(ROFL_MAGIC)
)
