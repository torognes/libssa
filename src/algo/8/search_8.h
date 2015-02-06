/*
 * search.h
 *
 *  Created on: Oct 18, 2014
 *      Author: Jakob Frielingsdorf
 */

#ifndef SEARCH_8_H_
#define SEARCH_8_H_

#include <stdint.h>
#include <immintrin.h>

#include "../16/search_16.h"
#include "../../libssa_datatypes.h"
#include "../../util/minheap.h"
#include "../../util/linked_list.h"

#define CHANNELS_8_BIT 16
#define CDEPTH_8_BIT 4

struct s8query {
    unsigned long q_len;

    __m128i ** q_table_sse;
    __m256i ** q_table_avx;

    char * seq;
};
typedef struct s8query * p_s8query;

struct s8info {
    __m128i * hearray_sse;
    __m128i * dprofile_sse;

    __m256i * hearray_avx;
    __m256i * dprofile_avx;

    unsigned long maxdlen;
    unsigned long maxqlen;

    uint8_t channels;
    uint8_t cdepth;

    uint8_t penalty_gap_open;
    uint8_t penalty_gap_extension;

    uint8_t q_count;
    p_s8query queries[6];

    p_s16info s16info;
};
typedef struct s8info * p_s8info;

void search_8_init( int search_type );

p_s8info search_8_sse41_init( p_search_data sdp );
void search_8_sse41_exit( p_s8info s );

p_s8info search_8_avx2_init( p_search_data sdp );
void search_8_avx2_exit( p_s8info s );

void dprofile_fill_8_sse41( int8_t * dprofile, uint8_t * dseq_search_window );
void dprofile_fill_8_avx( int8_t * dprofile, uint8_t * dseq_search_window );

int move_db_sequence_window_8( int c, uint8_t ** d_begin, uint8_t ** d_end, uint8_t * dseq_search_window );

void search_8_sse41_sw( p_s8info s, p_db_chunk chunk, p_minheap heap, p_node * overflow_list, int query_id );
void search_8_sse41_nw( p_s8info s, p_db_chunk chunk, p_minheap heap, p_node * overflow_list, int query_id );
void search_8_sse41_nw_sellers( p_s8info s, p_db_chunk chunk, p_minheap heap, p_node * overflow_list, int query_id );

void search_8_avx2_sw( p_s8info s, p_db_chunk chunk, p_minheap heap, p_node * overflow_list, int query_id );
void search_8_avx2_nw( p_s8info s, p_db_chunk chunk, p_minheap heap, p_node * overflow_list, int query_id );
void search_8_avx2_nw_sellers( p_s8info s, p_db_chunk chunk, p_minheap heap, p_node * overflow_list, int query_id );

void search_8( p_db_chunk chunk, p_search_data sdp, p_search_result res );

#endif /* SEARCH_8_H_ */
