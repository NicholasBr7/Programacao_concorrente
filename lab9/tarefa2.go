package main

import (
	"fmt"
)

func tarefa(str chan string) {

	var aux string = ""
	aux = <- str //Recebe a mensagem vinda main
	fmt.Println("Dentro da Goroutine: ", aux)

	str <- "Olá, Main, bom dia, tudo bem?" //Envia a mensagem para a main

	aux = <- str
	fmt.Println("Dentro da Goroutine: ", aux)

	str <- "Certo, entendido."

	fmt.Println("Dentro da Goroutine: finalizando")
	str <- "" //Evita o problema da main finalizar antes da goroutine
}

//Realiza o envio de mensagens para a goroutine
func main() {
	var msg string = "" 
	
	str := make(chan string) //cria um canal de comunicacao nao-bufferizado
	
	go tarefa(str) 	//cria um goroutine que executará a função 'tarefa'

	str <- "Olá, Goroutine, bom dia!" //Envia a mensagem para a goroutine
	msg = <-str //Recebe a mensagem vinda goroutine
	fmt.Println("Dentro da Main: ", msg)

	str <- "Tudo bem! Vou terminar tá?"
	msg = <-str
	fmt.Println("Dentro da Main: ", msg)

	msg = <-str

	fmt.Println("Dentro da Main: finalizando")

}