package main

import (
	"time"

	"../connection"
)

func main() {

	connection.StartServerMode()

	for i := 0; i < 5; i++ {
		time.Sleep(1000 * time.Millisecond)
	}

}
