#include <stdio.h>
#include "keyboard.h"

// phiscall memory for the keyboard



/*
    - For keyboard we use scancodes to capture what was pressed
    - so basically we need input and output for this
*/

static void mappingKeys(void) {
    char ascii_normal[256] = {0};
    // all keys
    ascii_normal[scanA] = 'a';
    ascii_normal[scanB] = 'b';
    ascii_normal[scanC] = 'c';
    ascii_normal[scanD] = 'd';
    ascii_normal[scanE] = 'e';
    ascii_normal[scanF] = 'f';
    ascii_normal[scanG] = 'g';
    ascii_normal[scanH] = 'h';
    ascii_normal[scanI] = 'i';
    ascii_normal[scanJ] = 'j';
    ascii_normal[scanK] = 'k';
    ascii_normal[scanL] = 'l';
    ascii_normal[scanM] = 'm';
    ascii_normal[scanN] = 'n';
    ascii_normal[scanO] = 'o';
    ascii_normal[scanP] = 'p';
    ascii_normal[scanQ] = 'q';
    ascii_normal[scanR] = 'r';
    ascii_normal[scanS] = 's';
    ascii_normal[scanT] = 't';
    ascii_normal[scanU] = 'u';
    ascii_normal[scanV] = 'v';
    ascii_normal[scanW] = 'w';
    ascii_normal[scanX] = 'x';
    ascii_normal[scanY] = 'y';
    ascii_normal[scanZ] = 'z';

    // punctuations
    ascii_normal[scanSemicolon] = ';';
    ascii_normal[scanQuote] = '\'';
    ascii_normal[scanComma] = ',';
    ascii_normal[scanDot] = '.';
    ascii_normal[scanSlash] = '/';
    ascii_normal[scanBackQuote] = '`';

    // special keys
    // ascii_normal[scanESC] = '\0';
    // ascii_normal[scanTab] = '\0';
    // ascii_normal[scanCapsLock] = '\0';

    // ascii_normal[scanLeftShift] = '\0';
    // ascii_normal[scanRightShift] = '\0';
    // ascii_normal[scanRightCtrl] = '\0';
    // ascii_normal[scanLeftCtrl] = '\0';
    // ascii_normal[scanDelete] = '\0';

    // // editing keys
    // ascii_normal[scanUpArrow] = '\n';
    // ascii_normal[scanDownArrow] = '\n';
    // ascii_normal[scanLeftArrow] = '\n';
    // ascii_normal[scanRightArrow] = '\n';


    
}

// static void especialKeys(char *args){

//     if (scancode)
//     if (args){
//         switch (*args) {
//             case scanESC:
//                 break;
            
//             case scanTab:

//         };
//     }


// }

int main(){
    mappingKeys();

    return 0;
}