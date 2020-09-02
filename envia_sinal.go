package main

import (
    "fmt"
    "os"
	"strconv"
	"syscall"
)

func help() {
	fmt.Println("Esteprograma recebe como parâmetros o número do processo destino e o sinal que deve ser enviado")
}	
func main() {

	if(len(os.Args)!=3){
        help()
        os.Exit(3)
    }


	pid, err := strconv.Atoi(os.Args[1])
	if err != nil {
		help()
		os.Exit(3)
	}

	signal, err := strconv.Atoi(os.Args[2])
	if err != nil {
		help()
		os.Exit(3)
	}


	process, err := os.FindProcess(pid)
	if err != nil {
		help()
		os.Exit(3)
	}


	err = syscall.Kill(pid, syscall.Signal(signal))
//	if(syscall.Kill(pid)){
 //       help();
//        os.Exit(3);
 //   }

	
	fmt.Println(process)
    fmt.Println(err)
    fmt.Println(pid)
    fmt.Println(signal)


    fmt.Println("Enviado com sucesso")
}

//go build hello.go
//./hello a b c d
