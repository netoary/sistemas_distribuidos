package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"strconv"
	"strings"
	"sync"
	"time"
)

type Process struct {
	connection (net.Conn)
	times      (int)
}

var processes map[int]Process

// var Process int
var Port int
var wg sync.WaitGroup

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
			processId := strings.Split(string(message), "|")
			// if message if for this process change myTurn to True
			if string(message[0]) == "2" && string(processId[1]) == processString {
				myTurn = true
			}

			// if receive message if 5 kill process
			if string(message[0]) == "5" && string(processId[1]) == processString {
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

func write(Process int, conn net.Conn, r_input int, k_input int) {
	defer wg.Done()
	for i := 0; i < r_input; i++ {
		requestGrant(Process, conn)
		f, err := os.OpenFile("./resultado.txt", os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
		check(err)

		f.WriteString(strconv.Itoa(Process) + " " + time.Now().Format("02/1/06 15:04:05.9999") + "\n")

		time.Sleep(time.Duration(k_input) * 1 * time.Second)

		f.Close()

		releaseGrant(Process, conn)
	}

}

func main() {
	processes = make(map[int]Process)

	// Recupera os argumentos da linha de comando
	args := os.Args[1:]

	// Checa o número de parâmetros
	if len(args) < 3 {
		log.Fatal("Número de argumentos inválido. Argumentos necessários n, r e k.")
	}

	var n_input, _ = strconv.Atoi(args[0])
	var r_input, _ = strconv.Atoi(args[1])
	var k_input, _ = strconv.Atoi(args[2])

	// // connect to server
	// var initialConn net.Conn

	// fmt.Println("Waiting...")
	// for initialConn == nil {
	// 	initialConn, _ = net.Dial("tcp", "127.0.0.1:8000")
	// }
	// fmt.Println("Connection stablished")

	// // ask for id
	// fmt.Fprintf(initialConn, strconv.Itoa(1)+"\n")

	// // wait id
	// message, _, _ := bufio.NewReader(initialConn).ReadLine()

	// // store id
	// Process, _ := strconv.Atoi(string(message))
	// fmt.Println("Process :", Process)

	// //close connection to gain access
	// initialConn.Close()

	// // store new Port
	// Port = 8000 + int(Process)
	// fmt.Println("Port: ", Port)

	// // Wait for new connection
	// var conn net.Conn
	// fmt.Println("Waiting new connection...")
	// for conn == nil {
	// 	conn, _ = net.Dial("tcp", "127.0.0.1:"+strconv.Itoa(Port))
	// }
	// fmt.Println("Connection stablished")

	var connections []net.Conn
	for i := 0; i < n_input; i++ {
		wg.Add(1)
		go func() {
			defer wg.Done()
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
			processNumber, _ := strconv.Atoi(string(message))
			fmt.Println("Process :", processNumber)

			//close connection to gain access
			initialConn.Close()

			// store new Port
			Port = 8000 + int(processNumber)
			fmt.Println("Port: ", Port)

			// Wait for new connection
			var conn net.Conn
			fmt.Println("Waiting new connection...")
			for conn == nil {
				conn, _ = net.Dial("tcp", "127.0.0.1:"+strconv.Itoa(Port))
			}
			fmt.Println("Connection stablished")
			p := Process{
				connection: conn,
			}

			processes[processNumber] = p
			connections = append(connections, conn)
		}()
		wg.Wait()
	}
	// fmt.Println(processes, r_input, k_input)

	// for i := range connections {
	for processNumber, process := range processes {
		wg.Add(1)
		go write(processNumber, process.connection, r_input, k_input)
		// fmt.Println(connections[i])
	}
	wg.Wait()
	//write(Process, conn, r_input, k_input)

}
