package main

import (
	"fmt"
	"os"
	"time"

	"github.com/sanity-io/litter"
	"github.com/tlaceby/bedrock/src/analysis"
	"github.com/tlaceby/bedrock/src/parser"
)

func main() {
	sourceBytes, _ := os.ReadFile("./examples/test.br")
	start := time.Now()
	ast := parser.Parse(string(sourceBytes), "test.br")
	duration := time.Since(start)

	litter.Dump(ast)

	checker := analysis.CreateTypechecker(nil)
	checker.Typecheck(ast)

	fmt.Printf("\nDuration: %v\n", duration)
}
