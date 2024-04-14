package main

import (
	"fmt"
	"os"
	"time"

	"github.com/tlaceby/bedrock/src/analysis"
	"github.com/tlaceby/bedrock/src/parser"
)

func main() {
	sourceBytes, _ := os.ReadFile("./examples/test.br")
	start := time.Now()
	ast := parser.Parse(string(sourceBytes), "test.br")
	duration := time.Since(start)

	// litter.Dump(ast)
	println()
	globalEnv := analysis.CreateSymbolTable(nil, false, false, false, "global")
	analysis.Typecheck(ast, globalEnv)
	fmt.Printf("\nDuration: %v\n", duration)
}
