/*
 * libssa.c
 *
 * LIBSSA - Library for SIMD accelerated optimal Sequence Alignments
 *
 *  Created on: 28 Aug 2014
 *      Author: Jakob Frielingsdorf
 */

#include "libssa.h"
#include "libsdb.h"
#include "util.h"

// #############################################################################
// Configuration data
// ##################
int _verbose = VERBOSE_OFF;
int _nr_threads; // todo get number of CPUs
int _use_simd = SIMD_ON;

// #############################################################################
// External defined functions
// ##########################

// in matrices.c
extern void score_matrix_init_constant_scoring(const int32_t matchscore,
        const int32_t mismatchscore);
extern void score_matrix_read_file(const char * matrix);
extern void score_matrix_read_string(const char * matrix);
extern void score_matrix_read(char* matrixname);
extern void score_matrix_free();

// in query.c
extern void query_init(char * queryname, long strands);

extern char* gencode_names[23];
extern int symtype;
extern int query_gencode;
extern int db_gencode;
extern int query_strands;

// #############################################################################
// Data types
// ##########

// defined in and include from libssa_datatypes.h

// #############################################################################
// Technical initialisation
// ########################
void set_verbose(int debug) {
    _verbose = debug;
}

void set_threads(int nr) {
    _nr_threads = nr;
}

void set_use_simd(int simd) {
    _use_simd = simd;
}

void set_output_file(char* outfile) {
    init_out(outfile);
}

// #############################################################################
// Initialisations
// ################
/**
 * Initialises the used scoring matrix (e.g. BLOSUM62, PAM250, ...). The data
 * is stored internally.
 *
 * @param matrix_name  name of the matrix to initialise
 *
 * List of available matrices:
 *  - blosum45
 *  - blosum50
 *  - blosum62
 *  - blosum80
 *  - blosum90
 *  - pam30
 *  - pam70
 *  - pam250
 */
void init_score_matrix(char* matrix_name) {
    score_matrix_read(matrix_name);
}

/**
 * Initialises the used scoring matrix from a file. The data
 * is stored internally.
 *
 * @param file_name  path to the file of the matrix
 */
void init_score_matrix_file(char* file_name) {
    score_matrix_read_file(file_name);
}

/**
 * Initialises the used scoring matrix from a string. The data
 * is stored internally.
 *
 * @param matrix matrix as a string
 */
void init_score_matrix_string(char* matrix) {
    score_matrix_read_string(matrix);
}

/**
 * Release the memory allocated by function init_score_matrix.
 *
 * @see init_score_matrix
 */
void free_matrix() {
    score_matrix_free();
}

/**
 * Initialises gap penalties used for the alignments.
 *
 * @param  gapO  penalty for opening a gap
 * @param  gapE  penalty for extending a gap
 */
void init_gap_penalties(const int32_t gapO, const int32_t gapE) {
    // TODO
}

/**
 * Initialises the scoring scheme if no scoring matrix is used.
 *
 * Should only be used, if no scoring matrix is used.
 *
 * @param  p    penalty for a mismatch
 * @param  m    reward for a match
 */
void init_scoring(const int32_t p, const int32_t m) {
    score_matrix_init_constant_scoring(p, m);
}

/**
 * Reads a FASTA file containing multiple sequences to compare the query
 * sequence against.
 * The initialised data is stored internally.
 *
 * @param fasta_db_file  path to a file in FASTA format
 */
void init_db_fasta(char* fasta_db_file) {
    sdb_init_fasta(fasta_db_file);
}

/**
 * Release the memory allocated by the function init_db_fasta.
 *
 * @see init_db_fasta
 */
void free_db() {
    sdb_free_db();
}

/**
 * Default is DEFAULT_SYMTYPE = AMINOACID (1)
 *
 * the symbol translation is done on the fly on both sides
 */
void init_symbol_translation(int type, int strands) {
    symtype = type;
    query_strands = strands;

    sdb_init_symbol_translation(type, strands);
}

void init_genetic_codes(int q_gencode, int d_gencode) {
    if ((q_gencode < 1) || (q_gencode > 23) || !gencode_names[q_gencode - 1]) {
        ffatal("Illegal query genetic code specified.");
    }

    if ((d_gencode < 1) || (d_gencode > 23) || !gencode_names[d_gencode - 1]) {
        ffatal("Illegal database genetic code specified.");
    }

    query_gencode = q_gencode;
    db_gencode = d_gencode;
}

/**
 * Reads a FASTA file containing the query sequence.
 *
 * @param fasta_seq_file  path to a file in FASTA format
 * @return pointer to the query profile structure
 */
p_query init_sequence_fasta(char* fasta_seq_file) {
    query_init(fasta_seq_file, 2 /* TODO strands */);
    // TODO read FASTA-file spec and find code in SWIPE/SWARM
    return NULL;
}

/**
 * Release the memory allocated by the functions init_sequence_fasta.
 *
 * @param  p   pointer to the query profile structure
 *
 * @see init_sequence_fasta
 */
void free_sequence(p_query p) {
    free(p);
    // TODO evt do more
}

// #############################################################################
// Alignment
// #########
/**
 * Aligns the query sequence against all sequences in the database using the
 * Smith-Waterman Algorithm.
 *
 * @param  p   pointer to the query profile structure
 * ...
 * @return pointer to the alignment structure
 */
p_alignment sw_align(p_query p /* TODO ...*/) {
    return NULL; // TODO
}

/**
 * Aligns the query sequence against all sequences in the database using the
 * Needleman-Wunsch Algorithm.
 *
 * @param  p   pointer to the query profile structure
 * ...
 * @return pointer to the alignment structure
 */
p_alignment nw_align(p_query p /* TODO ...*/) {
    return NULL; // TODO
}

/**
 * Aligns the query sequence against all sequences in the database using the
 * Needleman-Wunsch Algorithm as described by Sellers (TODO ref).
 *
 * TODO This function might call nw_align in the background.
 *
 * @param  p   pointer to the query profile structure
 * ...
 * @return pointer to the alignment structure
 */
p_alignment nw_sellers_align(p_query p /* TODO ...*/) {
    return NULL; // TODO
}

/**
 * Aligns the query sequence against all sequences in the database using the
 * Needleman-Wunsch Algorithm ignoring gaps in the beginning and/or end.
 *
 * @param  p   pointer to the query profile structure
 * ...
 * @return pointer to the alignment structure
 */
p_alignment nw_ignore_gaps_align(p_query p /* TODO ... ignored gaps...*/) {
    return NULL; // TODO
}

/**
 * Release the memory allocated by the functions sw_align, nw_align,
 * nw_sellers_align and nw_ignore_gaps_align.
 *
 * @param  a   pointer to the alignment structure
 *
 * @see sw_align
 * @see nw_align
 */
void free_alignment(p_alignment a) {
    // TODO
}
