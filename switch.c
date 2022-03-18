/*
 * file: switch.c
 *
 * code for a switch node
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <unistd.h>
#include <fcntl.h>

#include "main.h"
#include "net.h"
#include "man.h"
#include "host.h"
#include "packet.h"
#include "switch.h"

#define MAX_FT_ENTRIES 100 // maximum number of entries in a forwarding table


void switch_main(int switch_id) {
   /* forwarding table
    * col [0] = valid
    * col [1] = destination (host ID)
    * col [2] = port #
    */
   int forwarding_table [3] [MAX_FT_ENTRIES];

   /* initialize forwarding table */
   for (int i=0; i < MAX_FT_ENTRIES; i++) {
      forwarding_table[0][i] = 0;
   }
}
