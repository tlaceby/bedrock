#!/bin/bash

build() {
  make -j10
}

clean() {
  make clean
}

debug() {
  make clean
  build
  ./bedrock run ./examples/test.br --debug
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
  debug)
    debug
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