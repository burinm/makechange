/* 7/2021 - burin

    Make change with standard United States currency.
    Return the number and type of denominations, using the
    lowest total count

    Assumptions:
        [1] Largest bill we have is $100 dollar bill
        [2] Smallest denomination is a penny (.01 dollars)
        [3] We do not have any 2 dollar bills
        [4] Max transaction 2^32 pennies - $42949672.96 
            Use $42949671 dollars as check (so max is $42949671.99)

        [5] Also, assume this is an embedded system with no
        floating point math. So everything will be done in whole
        pennies

    Good hint on printing special characters to console:
        https://stackoverflow.com/questions/23983906/printing-multi-byte-characters-in-terminal-using-c

        In order to find bytes quickly type this in a python console:
            u"<<copy/paste cent sign here>>".encode('UTF-8')

    TODO: Define error handling return values (remove magic numbers)
    TODO: Split up main/change logic into seperate compile units
    TODO: Make denominations definitions modular
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> //malloc, free, strtol
#include <string.h> //memset
#include <ctype.h> //isalnum
#include <errno.h> //errno



#define MAX_TRANSACTION_DOLLARS 42949671
#define DOLLAR      (100)
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

int make_change(uint32_t cents_in);
int process_currency_string(char* s, currency_t *c);
int change_checksum(uint32_t cents_in, uint32_t* currency_array);

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("usage: make_change <dollars.cents>\n");
        return 0;
    }

    //TODO - sanitize input

    printf("[Initalized with %d denominations]\n", NUM_DENOMINATIONS);
    for (int i=0; i< NUM_DENOMINATIONS; i++) {
        if (denominations[i] >= DOLLAR) {
            //This  will be slow, but is just for test
            printf("\t(%d) $%d dollar bill\n", i+1, denominations[i] / DOLLAR);
        } else {
            printf("\t(%d) %d%s coin\n", i+1, denominations[i], CENT_SIGN);
        }
    }
    printf("\n");

    currency_t amount = {0,0};
    process_currency_string(argv[1], &amount);

    printf("[Dollars in: %u] [Cents in: %u]\n", amount.dollars, amount.cents);

    uint32_t total_cents = 0;

    if (amount.dollars <= MAX_TRANSACTION_DOLLARS) {
        total_cents = amount.dollars * DOLLAR + amount.cents;
        make_change(total_cents);
    } else {
        printf("%u.%u too large\n", amount.dollars, amount.cents);
        return -1;
    }

    if (change_checksum(total_cents, change_counts) != 0) {
        return -1;
    }

    printf("\n[Change]\n");
    for (int i=0; i< NUM_DENOMINATIONS; i++) {
        if (change_counts[i] > 0) {
            if (denominations[i] >= DOLLAR) {
                //This will be slow, but is just for output
                printf("\t%u x $%u dollar bills\n", change_counts[i], denominations[i] / DOLLAR);
            } else {
                printf("\t%u x %u%s coin\n", change_counts[i], denominations[i], CENT_SIGN);
            }
        }
    } 


return 0;
} 

/*Count out change for cents_in by iterating through denominations
    largest -> smallest
*/
int make_change(uint32_t cents_in) {

    memset(change_counts, 0, sizeof(change_counts));
    for (int i=0; i< NUM_DENOMINATIONS; i++) {
        if (denominations[i] > cents_in) {
            continue;
        }

        /* To avoid division, keep subtracting the current
            denomination until the remainder is smaller than
            the denomination. Keep count of how many
            subtractions were made.
       */ 
        uint16_t temp_divisor = denominations[i];
        while (cents_in >= temp_divisor) {
            change_counts[i]++;             
            cents_in -= temp_divisor;
            //TODO - error handle infinite loop
        }
    }

return 0;
}

/* Process input string of form <dollars.cents> */
int process_currency_string(char* s, currency_t *c) {
    if (s == NULL || c == NULL) {
        return -3;
    }

    c->dollars = 0;
    c->cents = 0;

    /* Make input permissable as possible. Anything close to <number>.<number>
    
        This basically does the REGEX ^.*([:number:]*)\.([:number:]*).*$

        It returns (dollars) (cents) - in which either might have nothing
        in them. The idea is that any character other than a number or '.'
        is junk and ignored.
    */

    //Mark the start of each number
    char* dollar_start = NULL;
    char* cent_start = NULL;

    //Switch this on when we find a '.'
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
                //Since strtol uses '\0', mark end of dollar number
                *s = '\0'; //TODO - Do all systems make argv[] mutable?
                is_cents_flag = 1;
            }
        }
        s++;
    }


    if (dollar_start) {
        c->dollars = (uint32_t)strtol(dollar_start, NULL, 10);
        if (!errno) {
            //ok
        } else {
            c->dollars = 0;
            return -1;
        }
    }

    if (cent_start) {
        c->cents = (uint8_t)strtol(cent_start, NULL, 10);
        if (!errno) {
            //ok
        } else {
            c->cents = 0;
            return -1;
        }
    }

return 0;
}

int change_checksum(uint32_t cents_in, uint32_t* currency_array) {

    uint32_t checksum = 0;

    //Count up change array and compare to input
    for (int i=0; i< NUM_DENOMINATIONS; i++) {
        checksum += denominations[i] * currency_array[i];
    }

    if (checksum == cents_in) {
        printf("\tChecksum passed: %u (checksum) == %u (in)\n", checksum, cents_in);
        return 0;
    } else {
        printf("\tChecksum failed: %u (checksum) == %u (in)\n", checksum, cents_in);
        return -1;
    }

    printf("\n");
}
