/*
 * test_search63.c
 *
 *  Created on: 18 Jul 2014
 *      Author: kaos
 */

#include "../tests.h"

#include "../../src/algo/manager.h"
#include "../../src/algo/aligner.h"

#include "../../src/util.h"
#include "../../src/libssa.h"
#include "../../src/util/minheap.h"
#include "../../src/matrices.h"
#include "../../src/query.h"

extern p_search_data init_searchdata(p_query query);

START_TEST (test_manager_simple_sw)
    {
        init_symbol_translation(NUCLEOTIDE, FORWARD_STRAND, 3, 3);
        mat_init_constant_scoring(1, -1);

        p_query query = query_read("./tests/testdata/one_seq.fas");
        ssa_db_init_fasta("./tests/testdata/test.fas");

        init_for_sw();
        p_alignment_list alist = m_run(query, 3);
        // TODO test alist

        ck_assert_int_eq(3, alist->len);

        a_free(alist);

        mat_free();
        query_free(query);
    }END_TEST

START_TEST (test_manager_simple2_sw)
    {
        init_symbol_translation(NUCLEOTIDE, FORWARD_STRAND, 3, 3);
        mat_init_constant_scoring(1, -1);

        p_query query = query_read("./tests/testdata/one_seq.fas");
        ssa_db_init_fasta("./tests/testdata/test2.fas");

        init_for_sw();
        p_alignment_list alist = m_run(query, 1);

        ck_assert_int_eq(1, alist->len);

        a_free(alist);

        mat_free();
        query_free(query);
    }END_TEST

START_TEST (test_init_search_data)
    {
        init_symbol_translation(NUCLEOTIDE, FORWARD_STRAND, 3, 3);

        p_query query = query_read("./tests/testdata/one_seq.fas");

        p_search_data sdp = init_searchdata(query);

        // query data
        ck_assert_int_eq(1, sdp->q_count);

        seq_buffer buf = sdp->queries[0];

        ck_assert_str_eq(&query->nt[0].seq, &buf.seq.seq);
        ck_assert_int_eq(query->nt[0].len, buf.seq.len);
        ck_assert_int_eq(0, buf.frame);
        ck_assert_int_eq(0, buf.strand);

        ck_assert_int_eq(0, sdp->queries[1].seq.len);
        ck_assert_int_eq(0, sdp->queries[2].seq.len);
        ck_assert_int_eq(0, sdp->queries[3].seq.len);
        ck_assert_int_eq(0, sdp->queries[4].seq.len);
        ck_assert_int_eq(0, sdp->queries[5].seq.len);

        // other data
        ck_assert_ptr_ne(NULL, sdp->hearray);

        // TODO test profile and thread data

        query_free(query);
    }END_TEST

START_TEST (test_init_search_data2)
    {
        init_symbol_translation(NUCLEOTIDE, COMPLEMENTARY_STRAND, 3, 3);

        p_query query = query_read("./tests/testdata/one_seq.fas");

        p_search_data sdp = init_searchdata(query);

        // query data
        ck_assert_int_eq(1, sdp->q_count);

        seq_buffer buf = sdp->queries[0];
        ck_assert_str_eq(&query->nt[1].seq, &buf.seq.seq);
        ck_assert_int_eq(query->nt[1].len, buf.seq.len);
        ck_assert_int_eq(0, buf.frame);
        ck_assert_int_eq(1, buf.strand);

        ck_assert_int_eq(0, sdp->queries[1].seq.len);
        ck_assert_int_eq(0, sdp->queries[2].seq.len);
        ck_assert_int_eq(0, sdp->queries[3].seq.len);
        ck_assert_int_eq(0, sdp->queries[4].seq.len);
        ck_assert_int_eq(0, sdp->queries[5].seq.len);

        query_free(query);
    }END_TEST

START_TEST (test_init_search_data3)
    {
        init_symbol_translation(TRANS_QUERY, FORWARD_STRAND, 3, 3);

        p_query query = query_read("./tests/testdata/one_seq.fas");

        p_search_data sdp = init_searchdata(query);

        // query data
        ck_assert_int_eq(3, sdp->q_count);

        seq_buffer buf = sdp->queries[0];
        ck_assert_str_eq(&query->aa[0].seq, &buf.seq.seq);
        ck_assert_int_eq(query->aa[0].len, buf.seq.len);
        ck_assert_int_eq(0, buf.frame);
        ck_assert_int_eq(0, buf.strand);

        buf = sdp->queries[1];
        ck_assert_str_eq(&query->aa[1].seq, &buf.seq.seq);
        ck_assert_int_eq(query->aa[1].len, buf.seq.len);
        ck_assert_int_eq(1, buf.frame);
        ck_assert_int_eq(0, buf.strand);

        buf = sdp->queries[2];
        ck_assert_str_eq(&query->aa[2].seq, &buf.seq.seq);
        ck_assert_int_eq(query->aa[2].len, buf.seq.len);
        ck_assert_int_eq(2, buf.frame);
        ck_assert_int_eq(0, buf.strand);

        ck_assert_int_eq(0, sdp->queries[3].seq.len);
        ck_assert_int_eq(0, sdp->queries[4].seq.len);
        ck_assert_int_eq(0, sdp->queries[5].seq.len);

        // other data
        ck_assert_ptr_ne(NULL, sdp->hearray);

        query_free(query);
    }END_TEST

START_TEST (test_init_search_data4)
    {
        init_symbol_translation(TRANS_QUERY, BOTH_STRANDS, 3, 3);

        p_query query = query_read("./tests/testdata/one_seq.fas");

        p_search_data sdp = init_searchdata(query);

        // query data
        ck_assert_int_eq(6, sdp->q_count);

        seq_buffer buf = sdp->queries[0];
        ck_assert_str_eq(&query->aa[0].seq, &buf.seq.seq);
        ck_assert_int_eq(query->aa[0].len, buf.seq.len);
        ck_assert_int_eq(0, buf.frame);
        ck_assert_int_eq(0, buf.strand);

        buf = sdp->queries[1];
        ck_assert_str_eq(&query->aa[1].seq, &buf.seq.seq);
        ck_assert_int_eq(query->aa[1].len, buf.seq.len);
        ck_assert_int_eq(1, buf.frame);
        ck_assert_int_eq(0, buf.strand);

        buf = sdp->queries[2];
        ck_assert_str_eq(&query->aa[2].seq, &buf.seq.seq);
        ck_assert_int_eq(query->aa[2].len, buf.seq.len);
        ck_assert_int_eq(2, buf.frame);
        ck_assert_int_eq(0, buf.strand);

        buf = sdp->queries[3];
        ck_assert_str_eq(&query->aa[3].seq, &buf.seq.seq);
        ck_assert_int_eq(query->aa[3].len, buf.seq.len);
        ck_assert_int_eq(0, buf.frame);
        ck_assert_int_eq(1, buf.strand);

        buf = sdp->queries[4];
        ck_assert_str_eq(&query->aa[4].seq, &buf.seq.seq);
        ck_assert_int_eq(query->aa[4].len, buf.seq.len);
        ck_assert_int_eq(1, buf.frame);
        ck_assert_int_eq(1, buf.strand);

        buf = sdp->queries[5];
        ck_assert_str_eq(&query->aa[5].seq, &buf.seq.seq);
        ck_assert_int_eq(query->aa[5].len, buf.seq.len);
        ck_assert_int_eq(2, buf.frame);
        ck_assert_int_eq(1, buf.strand);

        // other data
        ck_assert_ptr_ne(NULL, sdp->hearray);

        query_free(query);
    }END_TEST

START_TEST (test_init_search_data5)
    {
        init_symbol_translation(AMINOACID, BOTH_STRANDS, 3, 3);

        p_query query = query_read("./tests/testdata/one_seq.fas");

        p_search_data sdp = init_searchdata(query);

        // query data
        ck_assert_int_eq(1, sdp->q_count);

        seq_buffer buf = sdp->queries[0];
        ck_assert_str_eq(&query->aa[0].seq, &buf.seq.seq);
        ck_assert_int_eq(query->aa[0].len, buf.seq.len);
        ck_assert_int_eq(0, buf.frame);
        ck_assert_int_eq(0, buf.strand);

        ck_assert_int_eq(0, sdp->queries[1].seq.len);
        ck_assert_int_eq(0, sdp->queries[2].seq.len);
        ck_assert_int_eq(0, sdp->queries[3].seq.len);
        ck_assert_int_eq(0, sdp->queries[4].seq.len);
        ck_assert_int_eq(0, sdp->queries[5].seq.len);

        // other data
        ck_assert_ptr_ne(NULL, sdp->hearray);

        query_free(query);
    }END_TEST

void addManagerTC(Suite *s) {
    TCase *tc_core = tcase_create("manager");
    tcase_add_test(tc_core, test_manager_simple_sw);
    tcase_add_test(tc_core, test_manager_simple2_sw);
    tcase_add_test(tc_core, test_init_search_data);
    tcase_add_test(tc_core, test_init_search_data2);
    tcase_add_test(tc_core, test_init_search_data3);
    tcase_add_test(tc_core, test_init_search_data4);
    tcase_add_test(tc_core, test_init_search_data5);

    suite_add_tcase(s, tc_core);
}
