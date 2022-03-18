/*
 * file: switch.h
 * 
 * contains prototypes, enums and structs for switch.c
 */

enum bool {FALSE, TRUE};

/* entry for forwarding table */
struct FT_entry {
   int valid;
   int dst;
   int port;
};

void fwd_table_add();

void switch_main(int switch_id);

