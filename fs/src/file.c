#include "debug.h"

int main(void) {

    char buff[120] = "This is a test to see if print works";
    int number = 1;
    if (number == 1){
        kprintf_str("hello this is just a number\n");
        kprintf_str(buff);
        kprintf_int(number);
    }
    return 0;
}