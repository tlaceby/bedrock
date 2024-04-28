#pragma once

#include <iostream>
#include <string>

#include "../flags.h"

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

#define BOLD_BLACK "\033[1;30m"
#define BOLD_RED "\033[1;31m"
#define BOLD_GREEN "\033[1;32m"
#define BOLD_YELLOW "\033[1;33m"
#define BOLD_BLUE "\033[1;34m"
#define BOLD_MAGENTA "\033[1;35m"
#define BOLD_CYAN "\033[1;36m"
#define BOLD_WHITE "\033[1;37m"

#define BG_BLACK "\033[40m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"

#define UNDERLINE "\033[4m"
#define BOLD "\033[1m"

// namespace colors

#define cyan(str) \
  string(COLORS_ENABLED ? (string(CYAN) + str + string(RESET)) : str)

#define bold(str) \
  string(COLORS_ENABLED ? (string(BOLD) + str + string(RESET)) : str)

#define red(str) \
  string(COLORS_ENABLED ? (string(RED) + str + string(RESET)) : str)

#define green(str) \
  string(COLORS_ENABLED ? (string(GREEN) + str + string(RESET)) : str)

#define yellow(str) \
  string(COLORS_ENABLED ? (string(YELLOW) + str + string(RESET)) : str)

#define blue(str) \
  string(COLORS_ENABLED ? (string(BLUE) + str + string(RESET)) : str)

#define magenta(str) \
  string(COLORS_ENABLED ? (string(MAGENTA) + str + string(RESET)) : str)

#define white(str) \
  string(COLORS_ENABLED ? (string(WHITE) + str + string(RESET)) : str)

#define black(str) \
  string(COLORS_ENABLED ? (string(BLACK) + str + string(RESET)) : str)

#define bold_red(str) \
  string(COLORS_ENABLED ? (string(BOLD_RED) + str + string(RESET)) : str)

#define bold_green(str) \
  string(COLORS_ENABLED ? (string(BOLD_GREEN) + str + string(RESET)) : str)

#define bold_yellow(str) \
  string(COLORS_ENABLED ? (string(BOLD_YELLOW) + str + string(RESET)) : str)

#define bold_blue(str) \
  string(COLORS_ENABLED ? (string(BOLD_BLUE) + str + string(RESET)) : str)

#define bold_magenta(str) \
  string(COLORS_ENABLED ? (string(BOLD_MAGENTA) + str + string(RESET)) : str)

#define bold_cyan(str) \
  string(COLORS_ENABLED ? (string(BOLD_CYAN) + str + string(RESET)) : str)

#define bold_white(str) \
  string(COLORS_ENABLED ? (string(BOLD_WHITE) + str + string(RESET)) : str)

#define bold_black(str) \
  string(COLORS_ENABLED ? (string(BOLD_BLACK) + str + string(RESET)) : str)

#define bg_red(str) \
  string(COLORS_ENABLED ? (string(BG_RED) + str + string(RESET)) : str)

#define bg_green(str) \
  string(COLORS_ENABLED ? (string(BG_GREEN) + str + string(RESET)) : str)

#define bg_yellow(str) \
  string(COLORS_ENABLED ? (string(BG_YELLOW) + str + string(RESET)) : str)

#define bg_blue(str) \
  string(COLORS_ENABLED ? (string(BG_BLUE) + str + string(RESET)) : str)

#define bg_magenta(str) \
  string(COLORS_ENABLED ? (string(BG_MAGENTA) + str + string(RESET)) : str)

#define bg_cyan(str) \
  string(COLORS_ENABLED ? (string(BG_CYAN) + str + string(RESET)) : str)

#define bg_white(str) \
  string(COLORS_ENABLED ? (string(BG_WHITE) + str + string(RESET)) : str)

#define bg_black(str) \
  string(COLORS_ENABLED ? (string(BG_BLACK) + str + string(RESET)) : str)

#define ul(str) \
  string(COLORS_ENABLED ? (string(UNDERLINE) + str + string(RESET)) : str)