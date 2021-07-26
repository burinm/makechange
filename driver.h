/* 7/2021 - burin
*/
#ifndef __DRIVER_H__
#define __DRIVER_H__

#include "makechange.h"

#define MAX_TRANSACTION_DOLLARS (42949671)
#define DOLLAR                  (100U)
#define MAX_CENTS               (99U)
#define CENT_SIGN               "\xc2\xa2" //TODO - UTF 8 only? 


int process_currency_string(char* s, currency_t *c);

#endif
