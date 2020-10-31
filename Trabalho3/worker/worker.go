package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strconv"
	"time"
)

var Process int
var Port int

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func killMyself() {
	os.Exit(0)
}

func requestGrant(Process int, conn net.Conn) {
	fmt.Println("Request Grant")

	var processString string = strconv.Itoa(Process)

	// ask for access
	var message string = "1|" + processString + "\n"
	fmt.Fprintf(conn, message)

	myTurn := false

	// test if process time
	for !myTurn {
		// read message from buffer
		message, _, _ := bufio.NewReader(conn).ReadLine()

		// test message
		if string(message) != "" {

			// if message if for this process change myTurn to True
			if string(message[0]) == "2" && string(message[2]) == processString {
				myTurn = true
			}

			// if receive message if 5 kill process
			if string(message[0]) == "5" && string(message[2]) == processString {
				killMyself()
			}

		}
	}

}

func releaseGrant(Process int, conn net.Conn) {
	fmt.Println("Release Grant")
	var message string = "3|" + strconv.Itoa(Process) + "\n"
	fmt.Fprintf(conn, message)
}

func write(Process int, conn net.Conn) {

	for {
		requestGrant(Process, conn)
		f, err := os.OpenFile("./resultado.txt", os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
		check(err)

		f.WriteString(strconv.Itoa(Process) + " " + time.Now().Format("02/1/06 15:04:05.9999") + "\n")

		time.Sleep(1 * time.Second)

		f.Close()

		releaseGrant(Process, conn)
	}

}

func main() {

	// connect to server

	var initialConn net.Conn

	fmt.Println("Waiting...")
	for initialConn == nil {
		initialConn, _ = net.Dial("tcp", "127.0.0.1:8000")
	}
	fmt.Println("Connection stablished")

	// ask for id
	fmt.Fprintf(initialConn, strconv.Itoa(1)+"\n")

	// wait id
	message, _, _ := bufio.NewReader(initialConn).ReadLine()

	// store id
	Process, _ := strconv.Atoi(string(message))
	fmt.Println("Process :", Process)

	//close connection to gain access
	initialConn.Close()

	// store new Port
	Port = 8000 + int(Process)
	fmt.Println("Port: ", Port)

	// Wait for new connection
	var conn net.Conn
	fmt.Println("Waiting new connection...")
	for conn == nil {
		conn, _ = net.Dial("tcp", "127.0.0.1:"+strconv.Itoa(Port))
	}
	fmt.Println("Connection stablished")

	write(Process, conn)

}
