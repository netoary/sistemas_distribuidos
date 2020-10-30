package main

import (
	"bufio"
	"container/list"
	"fmt"
	"net"
	"strconv"
)

var process int = 1

var connections map[int]net.Conn

var requests list.List

func giveGrant(processNumber int) {

	var message string = "2|" + strconv.Itoa(processNumber) + "\n"
	fmt.Fprintf(connections[processNumber], message)

}

func createConnectionWithProccess(processNumber int) {

	addr := ":" + strconv.Itoa(8000+processNumber)
	ln, _ := net.Listen("tcp", addr)

	conn, _ := ln.Accept()

	connections[processNumber] = conn

	for {

		message, _ := bufio.NewReader(conn).ReadString('\n')

		if string(message[0]) == "1" {
			if requests.Len() == 0 {
				giveGrant(processNumber)
			}
			id, _ := strconv.Atoi(string(message[2]))
			requests.PushBack(id)
		}

		if string(message[0]) == "3" {
			requests.Remove(requests.Front())
			if requests.Len() != 0 {
				id := requests.Front().Value.(int)
				giveGrant(id)
			}
		}

	}

}

func main() {
	fmt.Println("Start server...")

	// listen on port 8000
	ln, _ := net.Listen("tcp", ":8000")

	for {
		conn, _ := ln.Accept()
		fmt.Println("Connection Accepted")
		// get message, output
		message, _ := bufio.NewReader(conn).ReadString('\n')
		fmt.Print("Message Received:", string(message))

		fmt.Fprintf(conn, strconv.Itoa(process)+"\n")
		fmt.Print("Message sended:", strconv.Itoa(process))

		conn.Close()

		go createConnectionWithProccess(process)

		process++
	}
}
