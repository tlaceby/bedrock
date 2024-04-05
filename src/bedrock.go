package main

import (
	"fmt"
	"os"
	"time"

	"github.com/sanity-io/litter"
	"github.com/tlaceby/bedrock/src/parser"
)

func main() {
	sourceBytes, _ := os.ReadFile("./examples/test.br")
	source := string(sourceBytes)
	start := time.Now()
	ast := parser.Parse(source)
	duration := time.Since(start)

	litter.Dump(ast)
	fmt.Printf("Duration: %v\n", duration)
}
