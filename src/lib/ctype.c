#include "../include/ctype.h"
 
// Fancy Smancy - Turns out you can add decimals/ints to characters!! Jesus.
char toLower(char ch) {
    if (ch >= 'A' && ch <= 'Z') {
        return ch + 32;
    }
    return ch;
}

void strToLower(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        str[i] = toLower(str[i]);
        i++;
    }
}