/* 7/2021 - burin
*/

#ifndef __DENOMINATIONS__
#define __DENOMINATIONS__

#include <stdint.h>

uint16_t denominations[] =
        { 10000, 5000, 2000, 1000, 500, 100, //bills
          50, 25, 10 , 5, 1};                //coins 

uint8_t NUM_DENOMINATIONS = (uint8_t)(sizeof(denominations) / sizeof(uint16_t));

#endif
