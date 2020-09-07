// Golang program to illustrate the usage of
// io.Pipe() function

// Including main package
package main

// Importing fmt, io, and bytes
import (
	"bytes"
	"fmt"
	"io"
	"math/big"
	"math/rand"
	"os"
	"runtime"
	"strconv"
	"strings"
	"time"
)

func help() {
	fmt.Println("Este programa recebe como parâmetros a quantidade de números que deve ser gerado")
}

func randomInt(min, max int) int {
	return min + rand.Intn(max-min)
}

// Calling main
func main() {

	num, err := strconv.Atoi(os.Args[1])
	if err != nil {
		help()
		os.Exit(3)
	}

	rand.Seed(time.Now().UnixNano())
	runtime.GOMAXPROCS(1)
	// fmt.Printf("%v\n", runtime.GOMAXPROCS(-1))

	// Calling Pipe method
	pipeReader, pipeWriter := io.Pipe()

	go func(num int) {

		var n int = 0
		for i := 0; i < num; i++ {
			r := randomInt(1, 100)

			n += r

			s := fmt.Sprintf("%d ", n)
			fmt.Fprint(pipeWriter, s)

		}
		fmt.Fprint(pipeWriter, "0")

		// Using Close method to close write
		pipeWriter.Close()
	}(num)

	// Creating a buffer
	buffer := new(bytes.Buffer)

	// Calling ReadFrom method and writing
	// data into buffer
	buffer.ReadFrom(pipeReader)

	// Prints the data in buffer
	var result string = buffer.String()

	s := strings.Split(result, " ")
	for _, char := range s {
		i, err := strconv.Atoi(char)
		if err != nil {
			panic(err)
		}
		if i == 0 {
			fmt.Println("Finished")
		} else if big.NewInt(int64(i)).ProbablyPrime(0) {
			fmt.Println(i, "is prime")
		} else {
			fmt.Println(i, "is not prime")
		}
	}

}
