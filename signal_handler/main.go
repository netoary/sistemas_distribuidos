package main

import (
	"fmt"
	"os"
	"os/signal"
	"runtime"
	"syscall"
)

func main() {
	runtime.GOMAXPROCS(1)
	sigs := make(chan os.Signal, 1)
	done := make(chan bool, 1)

	signal.Notify(sigs, syscall.SIGINT, syscall.SIGUSR1, syscall.SIGUSR2)

	go func(sigs <-chan os.Signal) {
		for {
			if sig, ok := <-sigs; ok {
				if sig == syscall.SIGUSR1 {
					fmt.Println("Tenta denovo vacilao")
				} else if sig == syscall.SIGUSR2 {
					fmt.Println("Quase lá")
				} else {
					done <- true
				}
			}
		}

	}(sigs)

	fmt.Println("awaiting signal")
	<-done
	fmt.Println("exiting")
}
