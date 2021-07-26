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

    Known bugs:
        input of m.n does not equal m.n0, instead is m.0n
            (example .9 = 9 cents, not 90 cents)

        Not a bug perse, but '$' needs to be escaped in the shell
            i.e., \$54.32
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> //strtol
#include <ctype.h> //isalnum
#include <errno.h> //errno
#include "driver.h"
#include "makechange.h"

//These are compiled in from Makefile
extern uint8_t NUM_DENOMINATIONS;
extern uint16_t denominations[];


int main(int argc, char* argv[]) {


    uint8_t num_denominations = NUM_DENOMINATIONS;

    if (num_denominations == 0) {
        printf("denominations not compiled into unit. <error>\n");
        return -4;
    }

    if (argc != 2) {
        printf("usage: change <dollars.cents>\n");
        return 0;
    }


    //TODO - sanitize input

    //Return results into this array, use same ordering as demoninations[]
    uint32_t change_counts[num_denominations];

    printf("[Initalized with %d denominations]\n", num_denominations);
    for (int i=0; i< num_denominations; i++) {
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

    //Check input for overflow
    uint32_t total_cents = 0;
    if (amount.cents > MAX_CENTS) {
        printf("%u cents too large, max cents is %u\n", amount.cents, MAX_CENTS);
        return -1;
    }

    if (amount.dollars <= MAX_TRANSACTION_DOLLARS) {
        total_cents = amount.dollars * DOLLAR + amount.cents;
        make_change(total_cents, change_counts, denominations, num_denominations);
    } else {
        printf("%u.%u too large\n", amount.dollars, amount.cents);
        return -1;
    }

    //Check results
    if (change_checksum(total_cents, change_counts, denominations, num_denominations) != 0) {
        return -1;
    }

    //All good, print out results
    printf("\n[Change]\n");
    for (int i=0; i< num_denominations; i++) {
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
