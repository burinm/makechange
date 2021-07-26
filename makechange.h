/* 7/2021 - burin
*/
#ifndef __MAKECHANGE_H__
#define __MAKECHANGE_H__

#include <stdint.h>

typedef struct {
   uint32_t dollars;
   uint8_t cents;
} currency_t;

int make_change(uint32_t cents_in, uint32_t* change_counts, uint16_t* denominations, uint8_t num_denominations);
int change_checksum(uint32_t cents_in, uint32_t* currency_array, uint16_t* denominations, uint8_t num_denominations);

#endif
