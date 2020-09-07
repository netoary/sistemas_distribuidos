package main

import (
	"fmt"
	"os"
	"strconv"
	"syscall"
)

var signals = [3]string{"INT", "R1", "R2"}

func help() {
	fmt.Println("Este programa recebe como parâmetros o número do processo destino e um dos sinais: INT, R1 ou R2")
}
func main() {

	if len(os.Args) != 3 {
		help()
		os.Exit(3)
	}

	pid, err := strconv.Atoi(os.Args[1])
	if err != nil {
		help()
		os.Exit(3)
	}

	signal_input := os.Args[2]

	inArray := false
	for _, a := range signals {
		if a == signal_input {
			inArray = true
		}
	}

	var signal syscall.Signal
	if inArray == false {
		help()
		os.Exit(3)
	} else {
		switch signal_input {
		case "INT":
			signal = syscall.SIGINT
		case "R1":
			signal = syscall.SIGUSR1
		case "R2":
			signal = syscall.SIGUSR2
		}
	}

	process, err := os.FindProcess(pid)
	if err != nil {
		help()
		os.Exit(3)
	}

	err = syscall.Kill(pid, signal)

	fmt.Println(process)
	fmt.Println(err)
	fmt.Println(pid)
	fmt.Println(signal)

	fmt.Println("Enviado com sucesso")
}

//go build envia_sinal.go
//./envia_sinal a b c d
