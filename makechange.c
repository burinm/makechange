/* 7/2021 - burin

    Make change with standard United States currency.
    Return the number and type of denominations, using the
    lowest total count

    Assumptions:
        Largest bill we have is 100 dollar
        Smallest denomination is a penny (.01 dollars)
        We do not have any 2 dollar bills
        Max transaction 2^32 pennies - $42949672.96 

    Also, assume this is an embedded system with no
    floating point math. So everything will be done in whole
    pennies
*/

#include <stdint.h>
#include <stdio.h>

#define DOLLAR  100
#define CENT_SIGN   '¢' //TODO - make portable

int denominations[11] = { 10000, 5000, 2000, 1000, 500, 100, //bills
                            50, 25, 10 , 5, 1};              //coins 

int main(int argc, char* argv[]) {

    uint8_t num_denominations = sizeof(denominations) / sizeof(int);

    printf("Initalized with %d denominations:\n", num_denominations);
    for (int i=0; i< num_denominations; i++) {
        if (denominations[i] >= DOLLAR) {
            //This  will be slow, but is just for test
            printf("[%d] $%d dollar bill\n", i+1, denominations[i] / DOLLAR);
        } else {
            printf("[%d] %d%c coin\n", i+1, denominations[i], CENT_SIGN);
        }


    } 


return 0;
} 

int make_change(uint32_t cents) {


}
