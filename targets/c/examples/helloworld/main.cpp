#include <cstring>
#include <iostream>
#include <libinstrument/instrument.h>


int main () {
    const char * words[] = {"!!!", "Hello", "World", "!!!", 0};
    const char ** ptr = words;


    instr_transaction_start();

    while (*ptr) {
        instr_probe(*ptr, strlen(*ptr), instr_pvar(ptr));
        std::cout << *ptr << " ";
        ptr++;
    }

    instr_oracle(0.5, 0.7);


    instr_transaction_end();

    return 0;
}