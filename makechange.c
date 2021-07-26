/* 7/2021 - burin
    Algorithm to make change by subtracting largest
    denominations first, then iterate to smaller
*/

#include <stdio.h>
#include <string.h> //memset
#include "makechange.h"

/*Count out change for cents_in by iterating through denominations
    largest -> smallest
*/
int make_change(uint32_t cents_in, uint32_t* change_counts, uint16_t* denominations, uint8_t num_denominations) {

    memset(change_counts, 0, sizeof(uint32_t) * num_denominations); //TODO, verify the size

    for (int i=0; i< num_denominations; i++) {
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

int change_checksum(uint32_t cents_in, uint32_t* currency_array, uint16_t* denominations, uint8_t num_denominations) {

    uint32_t checksum = 0;

    //Count up change array and compare to input
    for (int i=0; i< num_denominations; i++) {
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
