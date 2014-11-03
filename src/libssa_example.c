/*
 * libssa_example.c
 *
 *  Created on: Sep 4, 2014
 *      Author: Jakob Frielingsdorf
 */

#include "libssa.h"

#include <stdio.h>

int main(int argc, char**argv) {
    init_score_matrix(BLOSUM62);
    init_gap_penalties(4, 2);
    init_symbol_translation(NUCLEOTIDE, FORWARD_STRAND, 3, 3);

//    set_threads(2);

//    init_db_fasta("tests/testdata/test.fas");
    init_db_fasta("tests/testdata/AF091148.fas");
//    init_db_fasta("tests/testdata/Rfam_11_0.fasta");

    p_query query = init_sequence_fasta("tests/testdata/one_seq.fas");

    printf("Do local alignment using Smith-Waterman:\n");
    p_alignment_list alist = sw_align(query, 5);

    printf("Nr of alignments: %ld\n", alist->len);

    for (int i = 0; i < alist->len; i++) {
        alignment_p a = alist->alignments[i];

        printf("alignment %d: %s\n", i, a->alignment);
    }
    free_alignment(alist);

    printf("Do global alignment using Needleman-Wunsch:\n");
    alist = nw_align(query, 5);

    printf("Nr of alignments: %ld\n", alist->len);

    for (int i = 0; i < alist->len; i++) {
        alignment_p a = alist->alignments[i];

        printf("alignment %d: %s\n", i, a->alignment);
    }
    free_alignment(alist);

    free_db();
    free_sequence(query);

    ssa_exit();

    return 0;
}
