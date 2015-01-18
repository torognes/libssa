/*
 * test_search63.c
 *
 *  Created on: 18 Jul 2014
 *      Author: kaos
 */

#include "../../tests.h"

#include "../../../src/util/util.h"
#include "../../../src/libssa.h"
#include "../../../src/matrices.h"
#include "../../../src/db_iterator.h"
#include "../../../src/query.h"
#include "../../../src/util/minheap.h"
#include "../../../src/algo/16/search_16.h"
#include "../../../src/algo/search.h"

START_TEST (test_nw_align_simd_simple)
    {
        init_symbol_translation( NUCLEOTIDE, FORWARD_STRAND, 3, 3 );
        mat_init_constant_scoring( 1, -1 );

        p_query query = query_read_from_string( "short query", "AT" );

        ssa_db_init_fasta( "./tests/testdata/short_db.fas" );
        it_init( 1 );

        gapO = 0;
        gapE = 1;

        p_s16info s16info = search16_init( gapO, gapE );

        search16_init_query( s16info, query->nt[0].seq, query->nt[0].len );

        p_db_chunk chunk = it_new_chunk();
        it_next_chunk(chunk);

        p_minheap heap = minheap_init( 1 );

        search16_nw( s16info, chunk, heap, 1 );

        ck_assert_int_eq( 0, heap->array[0].score );

        search16_exit( s16info );

        minheap_exit(heap);
        it_free();
        mat_free();
        query_free( query );
    }END_TEST

START_TEST (test_nw_simd_blosum62)
    {
        init_symbol_translation( NUCLEOTIDE, FORWARD_STRAND, 3, 3 );
        mat_init_buildin( BLOSUM62 );

        p_query query = query_read_from_file( "./tests/testdata/NP_009305.1.fas" );

        ssa_db_init_fasta( "./tests/testdata/AF091148.fas" );
        it_init( 1403 );
        gapO = 1;
        gapE = 1;

        p_s16info s16info = search16_init( gapO, gapE );

        search16_init_query( s16info, query->nt[0].seq, query->nt[0].len );

        p_db_chunk chunk = it_new_chunk();
        it_next_chunk(chunk);

        p_minheap heap = minheap_init( 3 );

        search16_nw( s16info, chunk, heap, 1 );

        minheap_sort( heap );

        ck_assert_int_eq( 219, heap->array[0].score );
        ck_assert_int_eq( 215, heap->array[1].score );
        ck_assert_int_eq( 214, heap->array[2].score );

        search16_exit( s16info );

        minheap_exit(heap);
        it_free();
        mat_free();
        query_free( query );
    }END_TEST

START_TEST (test_nw_simd_more_sequences)
    {
        init_symbol_translation( NUCLEOTIDE, FORWARD_STRAND, 3, 3 );
        mat_init_constant_scoring( 1, -1 );

        p_query query = query_read_from_file("./tests/testdata/one_seq.fas");

        ssa_db_init_fasta( "./tests/testdata/test.fas" );
        it_init( 5 );

        gapO = 1;
        gapE = 1;

        p_s16info s16info = search16_init( gapO, gapE );

        search16_init_query( s16info, query->nt[0].seq, query->nt[0].len );

        p_db_chunk chunk = it_new_chunk();
        it_next_chunk(chunk);

        p_minheap heap = minheap_init( 5 );

        search16_nw( s16info, chunk, heap, 1 );

        minheap_sort( heap );

        ck_assert_int_eq( -43, heap->array[0].score );
        ck_assert_int_eq( -50, heap->array[1].score );
        ck_assert_int_eq( -52, heap->array[2].score );
        ck_assert_int_eq( -52, heap->array[3].score );
        ck_assert_int_eq( -147, heap->array[4].score );

        search16_exit( s16info );

        minheap_exit(heap);
        it_free();
        mat_free();
        query_free( query );
    }END_TEST


void addNeedlemanWunschSIMD16TC( Suite *s ) {
    TCase *tc_core = tcase_create( "NeedlemanWunschSIMD16" );
    tcase_add_test( tc_core, test_nw_align_simd_simple );
    tcase_add_test( tc_core, test_nw_simd_blosum62 );
    tcase_add_test( tc_core, test_nw_simd_more_sequences );

    suite_add_tcase( s, tc_core );
}
