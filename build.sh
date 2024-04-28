#!/bin/bash

build() {
  make -j10
}

clean() {
  make clean
}

run() {
  build
  ./bedrock run ./examples/test.br
}

# Main
case "$1" in
  run)
    run
    ;;
  build)
    build
    ;;
  clean)
    clean
    ;;
  *)
    build
    ;;
esac