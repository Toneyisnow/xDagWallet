#ifndef XDAG_BLOCK_H
#define XDAG_BLOCK_H

#include <stdint.h>
#include <stdio.h>
#include "hash.h"
#include "system.h"
#include "types.h"

extern xdag_time_t g_xdag_era;

#define MAIN_CHAIN_PERIOD       (64 << 10)
#define MAX_WAITING_MAIN        1
#define DEF_TIME_LIMIT          0 // (MAIN_CHAIN_PERIOD / 2)
#define XDAG_TEST_ERA           0x16900000000ll
#define XDAG_MAIN_ERA           0x16940000000ll
#define XDAG_ERA                g_xdag_era
#define MAIN_START_AMOUNT       (1ll << 42)
#define MAIN_BIG_PERIOD_LOG     21
#define MAIN_TIME(t)            ((t) >> 16)
#define MAX_LINKS               15

enum xdag_field_type {
	XDAG_FIELD_NONCE,        //0
	XDAG_FIELD_HEAD,         //1
	XDAG_FIELD_IN,           //2
	XDAG_FIELD_OUT,          //3
	XDAG_FIELD_SIGN_IN,      //4
	XDAG_FIELD_SIGN_OUT,     //5
	XDAG_FIELD_PUBLIC_KEY_0, //6
	XDAG_FIELD_PUBLIC_KEY_1, //7
	XDAG_FIELD_HEAD_TEST     //8
};

#define XDAG_BLOCK_FIELDS 16

struct xdag_field {
	union {
		struct {
			union {
				struct {
					uint64_t transport_header;
					uint64_t type;
					xdag_time_t time;
				};
				xdag_hashlow_t hash;
			};
			union {
				xdag_amount_t amount;
				xdag_time_t end_time;
			};
		};
		xdag_hash_t data;
	};
};

struct xdag_block {
	struct xdag_field field[XDAG_BLOCK_FIELDS];
};

#define xdag_type(b, n) ((b)->field[0].type >> ((n) << 2) & 0xf)

#ifdef __cplusplus
extern "C" {
#endif

	// start of regular block processing
	extern int xdag_blocks_start(void);

	// checks and adds block to the storage. Returns non-zero value in case of error.
	extern int xdag_add_block(struct xdag_block *b);

	// returns our first block. If there is no blocks yet - the first block is created.
	extern int xdag_get_our_block(xdag_hash_t hash);

	// calls callback for each own block
	extern int xdag_traverse_our_blocks(void *data, int (*callback)(void*, xdag_hash_t, xdag_amount_t, xdag_time_t, int));

	// calls callback for each block
	extern int xdag_traverse_all_blocks(void *data,int (*callback)(void *data, xdag_hash_t hash, xdag_amount_t amount, xdag_time_t time));

	// create and publish a block
	extern int xdag_create_block(struct xdag_field *fields, int inputsCount, int outputsCount, xdag_amount_t fee,
		xdag_time_t send_time, xdag_hash_t newBlockHashResult);

	// returns current balance for specified address or balance for all addresses if hash == 0
	extern xdag_amount_t xdag_get_balance(xdag_hash_t hash);

	// sets current balance for the specified address
	extern int xdag_set_balance(xdag_hash_t hash, xdag_amount_t balance);

	// calculates current supply by specified count of main blocks
	extern xdag_amount_t xdag_get_supply(uint64_t nmain);

	// returns position and time of block by hash
	extern int64_t xdag_get_block_pos(const xdag_hash_t hash, xdag_time_t *time);

	// returns a number of the current period, period is 64 seconds
	extern xdag_time_t xdag_main_time(void);

	// returns the number of the time period corresponding to the start of the network
	extern xdag_time_t xdag_start_main_time(void);

	// returns a number of key by hash of block or -1 if block is not ours
	extern int xdag_get_key(xdag_hash_t hash);

	// reinitialization of block processing
	extern int xdag_blocks_reset(void);

	// cleanup blocks
	extern int xdag_blocks_finish(void);

	// calculate difficulty from hash
	xdag_diff_t xdag_hash_difficulty(xdag_hash_t hash);

	// get all transactions of specified address, and return total number of transactions
	extern int xdag_get_transactions(xdag_hash_t hash, void *data, int (*callback)(void*, int, xdag_hash_t, xdag_amount_t, xdag_time_t));

#ifdef __cplusplus
};
#endif

#endif
