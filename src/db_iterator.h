/*
 * db_iterator.h
 *
 *  Created on: Oct 12, 2014
 *      Author: Jakob Frielingsdorf
 */

#ifndef DB_ITERATOR_H_
#define DB_ITERATOR_H_

#include "libssa_datatypes.h"

void it_free();

void it_init(unsigned long size);

p_seqinfo it_get_sequence(unsigned long id);

sequence it_translate_sequence(p_seqinfo info, int f, int s);

p_db_chunk it_new_chunk();

void it_next_chunk(p_db_chunk chunk);

void it_free_chunk(p_db_chunk chunk);

void it_reset_chunk_counter();

#endif /* DB_ITERATOR_H_ */
