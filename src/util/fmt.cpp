#include "fmt.h"

string utils::space(size_t padding) {
    string str = "";

    for (size_t i = 0; i < padding; i++) {
        str += ' ';
    }

    return str;
}