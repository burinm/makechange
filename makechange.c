/* 7/2021 - burin

    Make change with standard United States currency.
    Return the number and type of denominations, using the
    lowest total count

    Assumptions:
        Largest bill we have is 100 dollar
        Smallest denomination is a penny (.01 dollars)
        We do not have any 2 dollar bills
        Max transaction 2^32 pennies - $42949672.96 
            Use $42949671 dollars as check (so max is $42949671.99)

    Also, assume this is an embedded system with no
    floating point math. So everything will be done in whole
    pennies
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> //malloc, free, strtol
#include <string.h> //memset
#include <ctype.h> //isalnum
#include <errno.h> //errno



#define MAX_TRANSACTION_DOLLARS 42949671
#define DOLLAR      (100)

/* good hint - https://stackoverflow.com/questions/23983906/printing-multi-byte-characters-in-terminal-using-c

    In order to find the three bytes quickly for my post I just typed this in a python console: u"\u72d7".encode('UTF-8')
*/
#define CENT_SIGN   "\xc2\xa2" //TODO - UTF 8 only? 

#define NUM_DENOMINATIONS   (11)
uint16_t denominations[NUM_DENOMINATIONS] =
        { 10000, 5000, 2000, 1000, 500, 100, //bills
          50, 25, 10 , 5, 1};                //coins 

typedef struct {
   uint32_t dollars;
   uint8_t cents;
} currency_t;

//Return results into this array, use same ordering as demoninations[] 
uint32_t change_counts[NUM_DENOMINATIONS];

//Count out change for cents_in
int make_change(uint32_t cents_in);
int process_currency_string(char* s, currency_t *c);
int change_checksum(uint32_t cents_in, uint32_t* currency_array);

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("usage: make_change <dollars.cents>\n");
        return 0;
    }

    printf("Initalized with %d denominations:\n", NUM_DENOMINATIONS);
    for (int i=0; i< NUM_DENOMINATIONS; i++) {
        if (denominations[i] >= DOLLAR) {
            //This  will be slow, but is just for test
            printf("[%d] $%d dollar bill\n", i+1, denominations[i] / DOLLAR);
        } else {
            printf("[%d] %d%s coin\n", i+1, denominations[i], CENT_SIGN);
        }
    }

    currency_t amount = {0,0};
    process_currency_string(argv[1], &amount);
    uint32_t total_cents = 0;

    if (amount.dollars <= MAX_TRANSACTION_DOLLARS) {
        total_cents = amount.dollars * DOLLAR + amount.cents;
        make_change(total_cents);
    } else {
        printf("%d.%d too large\n", amount.dollars, amount.cents);
        return -1;
    }

    if (change_checksum(total_cents, change_counts) != 0) {
        return -1;
    }

    printf("Change:\n");
    for (int i=0; i< NUM_DENOMINATIONS; i++) {
        if (change_counts[i] > 0) {
            if (denominations[i] >= DOLLAR) {
                //This will be slow, but is just for output
                printf("%d $%d dollar bills\n", change_counts[i], denominations[i] / DOLLAR);
            } else {
                printf("%d %d%s coin\n", change_counts[i], denominations[i], CENT_SIGN);
            }
        }
    } 


return 0;
} 

int make_change(uint32_t cents_in) {

    memset(change_counts, 0, sizeof(change_counts));
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

int process_currency_string(char* s, currency_t *c) {
    if (s == NULL || c == NULL) {
        return -3;
    }

    c->dollars = 0;
    c->cents = 0;

    char* dollar_start = NULL;
    char* cent_start = NULL;

    int is_cents_flag = 0;

    while (*s) {
        if (isalnum(*s)) {
            if (is_cents_flag) { 
                if (cent_start == NULL) {
                    cent_start = s;
                }
            } else {
                if (dollar_start == NULL) {
                    dollar_start = s;
                }
            }
        } else {
            if (*s == '.') {
                *s = '\0';
                is_cents_flag = 1;
            }
        }
        s++;
    }


    if (dollar_start) {
        c->dollars = (uint32_t)strtol(dollar_start, NULL, 10);
        if (!errno) {
            printf("Dollars in:%d\n", c->dollars);
        } else {
            c->dollars = 0;
            return -1;
        }
    }

    if (cent_start) {
        c->cents = (uint8_t)strtol(cent_start, NULL, 10);
        if (!errno) {
            printf("Cents in:%d\n", c->cents);
        } else {
            c->cents = 0;
            return -1;
        }
    }
}

int change_checksum(uint32_t cents_in, uint32_t* currency_array) {

    uint32_t checksum = 0;

    for (int i=0; i< NUM_DENOMINATIONS; i++) {
        checksum += denominations[i] * currency_array[i];
    }

    if (checksum == cents_in) {
        printf("Checksum passed: %u (checksum) == %u (in)\n", checksum, cents_in);
        return 0;
    } else {
        printf("Checksum failed: %u (checksum) == %u (in)\n", checksum, cents_in);
        return -1;
    }
}
