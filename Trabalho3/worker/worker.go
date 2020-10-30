package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strconv"
	"strings"
	"time"
)

var process int
var port int

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func requestGrant(conn net.Conn) {
	var processString string = strconv.Itoa(process)
	// ask for access
	var message string = "1|" + processString + "\n"
	fmt.Fprintf(conn, message)

	myTurn := false

	for !myTurn {
		message, _ := bufio.NewReader(conn).ReadString('\n')

		if string(message[0]) == "2" && string(message[2]) == processString {
			myTurn = true
		}

	}

}

func releaseGrant(conn net.Conn) {

	// release grant
	var message string = "3|" + strconv.Itoa(process) + "\n"
	fmt.Fprintf(conn, message)

}

func write(conn net.Conn) {

	for {
		requestGrant(conn)
		f, err := os.Create("./resultado.txt")
		check(err)

		txt, err := f.WriteString(time.Now().Format("00:00:00.0000"))
		check(err)
		fmt.Printf("%d", txt)
		f.Close()

		releaseGrant(conn)

		time.Sleep(5 * time.Second)
	}

}

func main() {

	// connect to server
	conn1, err := net.Dial("tcp", "127.0.0.1:8000")

	fmt.Println("Waiting...")
	check(err)

	// ask for id
	fmt.Fprintf(conn1, strconv.Itoa(1)+"\n")

	// wait id
	message, _ := bufio.NewReader(conn1).ReadString('\n')
	fmt.Print("Message Received: ", message)

	message = strings.Replace(message, "\n", "", 1)
	// store id

	pr, _ := strconv.Atoi(message)
	conn1.Close()

	fmt.Print(process)
	port = 8000 + int(pr)

	fmt.Println(int(pr))
	fmt.Println(port)
	conn, err := net.Dial("tcp", "127.0.0.1:"+strconv.Itoa(port))

	write(conn)

}
