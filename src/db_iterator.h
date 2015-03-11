/*
 Copyright (C) 2014-2015 Jakob Frielingsdorf

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Affero General Public License as
 published by the Free Software Foundation, either version 3 of the
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Affero General Public License for more details.

 You should have received a copy of the GNU Affero General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 Contact: Jakob Frielingsdorf <jfrielingsdorf@gmail.com>
 */

#ifndef DB_ITERATOR_H_
#define DB_ITERATOR_H_

#include "libssa_datatypes.h"

void it_exit();

void it_init( size_t size );

void it_free_sequence( p_sdb_sequence seq );

p_db_chunk it_alloc_chunk( size_t size );
p_db_chunk it_init_new_chunk();

void it_next_chunk( p_db_chunk chunk );

void it_free_chunk( p_db_chunk chunk );

void it_reset_chunk_counter();

#endif /* DB_ITERATOR_H_ */
