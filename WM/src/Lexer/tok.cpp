#include "tok.h"

bool tok_is_const (Token tok) {
    return tok.type == DEC_NUMBER || tok.type == INT_NUMBER;
}

bool tok_is_identifier (Token tok) {
    return tok.type == IDENTIFIER;
}
