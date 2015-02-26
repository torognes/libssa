/*
 * test_search63.c
 *
 *  Created on: 18 Jul 2014
 *      Author: kaos
 */

#include "../../tests.h"

#include "../../../src/cpu_config.h"
#include "../../../src/matrices.h"
#include "../../../src/query.h"
#include "../../../src/db_iterator.h"
#include "../../../src/algo/searcher.h"
#include "../../../src/algo/8/search_8.h"
#include "../../../src/algo/8/search_8_util.h"
#include "../../../src/util/debug_tools.h"
#include "../../../src/util/util_sequence.h"

#define MATCH 5
#define MISMATCH -4

static p_s8info setup_simd_util_test( char * query_string, int hit_count ) {
    mat_init_constant_scoring( MATCH, MISMATCH );
    init_symbol_translation( NUCLEOTIDE, FORWARD_STRAND, 3, 3 );

    p_query query = query_read_from_string( "short query", query_string );

    p_search_data sdp = s_create_searchdata( query, hit_count );

    return search_8_init( sdp );
}

static void exit_simd_util_test( p_s8info s ) {
    search_8_exit( s );
    it_exit();
    mat_free();
    ssa_db_close();

    reset_compute_capability();
}

static void test_dprofile_8( int8_t * dprofile, int channels, sequence dseq ) {
    for( int i = 0; i < SCORE_MATRIX_DIM; ++i ) {
        for( int j = 0; j < CDEPTH_8_BIT; ++j ) {
            for( int k = 0; k < channels; k++ ) {
                int8_t val = dprofile[channels * CDEPTH_8_BIT * i + channels * j + k];

                if( k == 0 && i != 0 ) {
                    if( i == dseq.seq[j] ) {
                        ck_assert_int_eq( MATCH, val );
                    }
                    else {
                        ck_assert_int_eq( MISMATCH, val );
                    }
                }
                else {
                    ck_assert_int_eq( -1, val );
                }
            }
        }
    }
}

START_TEST (test_sse_simple)
    {
        set_max_compute_capability( COMPUTE_ON_SSE2 );

        p_s8info s = setup_simd_util_test( "AT", 1 );

        uint16_t dseq_search_window[CDEPTH_8_BIT * CHANNELS_8_BIT_SSE];
        memset( dseq_search_window, 0, sizeof(uint16_t) * CDEPTH_8_BIT * CHANNELS_8_BIT_SSE );

        sequence dseq = us_prepare_sequence( "AATG", 4, 0, 0 );

        for( int i = 0; i < CDEPTH_8_BIT; ++i ) {
            dseq_search_window[i * CHANNELS_8_BIT_SSE] = dseq.seq[i];
        }

        dprofile_fill_8_sse41( s->dprofile, dseq_search_window );

        test_dprofile_8( (int8_t*) s->dprofile, CHANNELS_8_BIT_SSE, dseq );

        exit_simd_util_test( s );
    }END_TEST

START_TEST (test_avx_simple)
    {
        set_max_compute_capability( COMPUTE_ON_AVX2 );

        p_s8info s = setup_simd_util_test( "AT", 1 );

        uint16_t dseq_search_window[CDEPTH_8_BIT * CHANNELS_8_BIT_AVX];
        memset( dseq_search_window, 0, sizeof(uint16_t) * CDEPTH_8_BIT * CHANNELS_8_BIT_AVX );

        sequence dseq = us_prepare_sequence( "AATG", 4, 0, 0 );

        for( int i = 0; i < CDEPTH_8_BIT; ++i ) {
            dseq_search_window[i * CHANNELS_8_BIT_AVX] = dseq.seq[i];
        }

        dprofile_fill_8_avx2( s->dprofile, dseq_search_window );

        test_dprofile_8( (int8_t*) s->dprofile, CHANNELS_8_BIT_AVX, dseq );

        exit_simd_util_test( s );
    }END_TEST

void add_8_simd_utilities_TC( Suite *s ) {
    TCase *tc_core = tcase_create( "8 bit SIMD utilities" );
    tcase_add_test( tc_core, test_sse_simple );
    tcase_add_test( tc_core, test_avx_simple );

    suite_add_tcase( s, tc_core );
}
