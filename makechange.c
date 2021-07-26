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
#include <stdlib.h> //malloc, free
#include <string.h> //memset


#define DOLLAR      (100)

/* good hint - https://stackoverflow.com/questions/23983906/printing-multi-byte-characters-in-terminal-using-c

    In order to find the three bytes quickly for my post I just typed this in a python console: u"\u72d7".encode('UTF-8')
*/
#define CENT_SIGN   "\xc2\xa2" //TODO - UTF 8 only? 

#define NUM_DENOMINATIONS   (11)
uint16_t denominations[NUM_DENOMINATIONS] =
        { 10000, 5000, 2000, 1000, 500, 100, //bills
          50, 25, 10 , 5, 1};                //coins 

//Return results into this array, use same ordering as demoninations[] 
uint32_t change_counts[NUM_DENOMINATIONS];

//Count out change for cents_in
int make_change(uint32_t cents_in);

int main(int argc, char* argv[]) {

    memset(change_counts, 0, sizeof(change_counts));
    

    printf("Initalized with %d denominations:\n", NUM_DENOMINATIONS);
    for (int i=0; i< NUM_DENOMINATIONS; i++) {
        if (denominations[i] >= DOLLAR) {
            //This  will be slow, but is just for test
            printf("[%d] $%d dollar bill\n", i+1, denominations[i] / DOLLAR);
        } else {
            printf("[%d] %d%s coin\n", i+1, denominations[i], CENT_SIGN);
        }
    } 

    make_change(1125);

    printf("Change:\n");
    for (int i=0; i< NUM_DENOMINATIONS; i++) {
        if (change_counts[i] > 0) {
            if (denominations[i] >= DOLLAR) {
                //This  will be slow, but is just for output
                printf("%d $%d dollar bills\n", change_counts[i], denominations[i] / DOLLAR);
            } else {
                printf("%d %d%s coin\n", change_counts[i], denominations[i], CENT_SIGN);
            }
        }
    } 


return 0;
} 

int make_change(uint32_t cents_in) {

    for (int i=0; i< NUM_DENOMINATIONS; i++) {
        if (denominations[i] > cents_in) {
            continue;
        }

        
        uint16_t temp_divisor = denominations[i];
        while (cents_in >= temp_divisor) {
            change_counts[i]++;             
            cents_in -= temp_divisor;
            //TODO - error handle infinite loop
        }
    }


}
