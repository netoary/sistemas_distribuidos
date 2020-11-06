package main

import (
	"bufio"
	"container/list"
	"fmt"
	"net"
	"os"
	"strconv"
	"strings"
	"sync"
)

type Process struct {
	connection (net.Conn)
	times      (int)
}

func (self *Process) Inc() {
	self.times++
}
func (self Process) currentValue() int {
	return self.times
}

var numProcess int = 1

var processes map[int]*Process

var requests list.List

var mutex = sync.RWMutex{}

func giveGrant(processNumber int) {
	var message string = "2|" + strconv.Itoa(processNumber) + "|" + "\n"
	fmt.Fprintf(processes[processNumber].connection, message)
	processes[processNumber].Inc()
}

func killProcess() {
	for processNumber, process := range processes {
		var message string = "5|" + strconv.Itoa(processNumber) + "|" + "\n"
		fmt.Fprintf(process.connection, message)
	}
	os.Exit(0)
}

func printListofRequests() {
	mutex.Lock()
	for e := requests.Front(); e != nil; e = e.Next() {
		fmt.Print(e.Value)
		fmt.Print(" ")
	}
	fmt.Print("\n")
	mutex.Unlock()
}

func printRequestsOfProcesses() {
	for processNumber, process := range processes {
		fmt.Println(strconv.Itoa(processNumber) + " " + strconv.Itoa(process.currentValue()))
	}
	fmt.Print("\n")
}

func listenTerminal() {
	reader := bufio.NewReader(os.Stdin)

	for {
		fmt.Print("Enter text: ")
		byte, _ := reader.ReadByte()

		switch string(byte) {
		case "1":
			printListofRequests()
		case "2":
			printRequestsOfProcesses()
		case "3":
			killProcess()
		}
	}
}

func createConnectionWithProccess(processNumber int) {

	addr := ":" + strconv.Itoa(8000+processNumber)
	ln, _ := net.Listen("tcp", addr)

	// Connection Accepted with new process
	conn, _ := ln.Accept()

	p := Process{
		connection: conn,
		times:      0,
	}

	processes[processNumber] = &p

	c := bufio.NewReader(conn)
	for {
		message, _, _ := c.ReadLine()
		if len(message) > 0 {
			if string(message[0]) == "1" {
				mutex.Lock()
				if requests.Len() == 0 {
					giveGrant(processNumber)
				}
				processId := strings.Split(string(message), "|")
				id, _ := strconv.Atoi(processId[1])
				requests.PushBack(id)
				mutex.Unlock()
			}

			if string(message[0]) == "3" {
				mutex.Lock()
				requests.Remove(requests.Front())
				if requests.Len() != 0 {
					id := requests.Front().Value.(int)
					giveGrant(id)
				}
				mutex.Unlock()
			}
		}

	}

}

func main() {

	processes = make(map[int]*Process)

	go listenTerminal()

	// listen on port 8000
	ln, _ := net.Listen("tcp", ":8000")

	for {
		conn, _ := ln.Accept()

		// get message, output
		message, _, _ := bufio.NewReader(conn).ReadLine()

		if string(message) == "1" {
			// send to procces its id
			fmt.Fprintf(conn, strconv.Itoa(numProcess)+"\n")

			// close actual connection to make it possible to another process to connect
			conn.Close()

			// create a unique connection with new process
			go createConnectionWithProccess(numProcess)

			// increment id for next process
			numProcess++
		}
	}
}
