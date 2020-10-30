package main

import (
	"fmt"
	"os"
	"time"

	"../connection"
)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func write() {
	connection.StartServerMode()

	f, err := os.Create("./resultado.txt")
	check(err)

	defer f.Close()

	txt, err := f.WriteString(time.Now().Format("00:00:00.0000"))
	check(err)
	fmt.Printf("%d", txt)

	f.Close()

}

func main() {
	write()
}
