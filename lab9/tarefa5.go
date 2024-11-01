package main

import (
	"fmt"
	"math"
)

var tam int = 10000 // Até onde deve verificar a quantidade de primos
var qtd int = 4  // Número de goroutines
var totalPrimosSeq int = 0 

// Função para verificar se um número é primo
func ehPrimo(n int) (bool) {
	if n <= 1 {
		return false
	}
	if n == 2 {
		return true
	}
	if n % 2 == 0 {
		return false
	}
	for i := 3; i <= int(math.Sqrt(float64(n))); i += 2 {
		if n % i == 0 {
			return false
		}
	}
	return true
}

func tarefa(ch1 chan int, ch2 chan int) {
    var totalPrimos int = 0
    for {
        i, mais := <-ch1
        if mais {
            if ehPrimo(i) {
                totalPrimos++
            }
        } else {
            ch2 <- totalPrimos
            return
        }
    }
}

//Verificação da corretude sequencialmente
func sequencial() {

	for i := 1; i <= tam; i++ {
		if ehPrimo(i) { totalPrimosSeq ++ }
	}
}

func main() {

	var primos int // Total de primos encontrados

	ch1 := make(chan int, tam) //Canal com os números para serem testados
	ch2 := make(chan int, qtd) //Canal para retornar a quantidade de primos encontrados

	sequencial()

	// Inicia as goroutines
	for i := 1; i <= qtd; i++ {
		go tarefa(ch1, ch2)
	}

	// Envia números para as goroutines
	for i := 1; i <= tam; i++ {
		ch1 <- i
	}
	close(ch1) // Não há mais números a serem enviados

	// Coleta resultados das goroutines
	for i := 1; i <= qtd; i++ {
		primos += <-ch2 
	}

	if primos == totalPrimosSeq {
		fmt.Println("O total de primos é:", primos)
		fmt.Println("O total de primos obtidos sequencialmente é:", totalPrimosSeq, ". Os valores conferem!")
	} else {
		fmt.Println("Diferença na contagem sequencial e concorrente. Seq:", totalPrimosSeq, ". Conc:", primos)
	}
}

