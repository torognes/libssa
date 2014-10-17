# libssa
# 
# Smith-Waterman database searches with Inter-sequence Parallel Execution
# 
# Copyright (C) 2008-2012 Torbjørn Rognes, University of Oslo, 
# Oslo University Hospital and Sencel Bioinformatics AS
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
# 
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 
# Contact: Torbjørn Rognes <torognes@ifi.uio.no>, 
# Department of Informatics, University of Oslo, 
# PO Box 1080 Blindern, NO-0316 Oslo, Norway

# Makefile for libssa

# non compilable files, some targets depend on
DEPS := Makefile
# objects to compile
OBJS := 
# files some targets depend on, like header files
USR_OBJS :=
# tests
TESTS := 
# files to clean, that are not in OBJS or TESTS 
TO_CLEAN := libssa.a

# All of the sources participating in the build are defined here
-include tests/subdir.mk
-include tests/algo/subdir.mk
-include src/subdir.mk
-include src/algo/subdir.mk
-include src/util/subdir.mk

MPI_COMPILE := `mpicxx --showme:compile`
MPI_LINK := `mpicxx --showme:link`

COMMON := -g -pg
# add "-fprofile-arcs -ftest-coverage" to COMMON for code coverage

LIBS := -lpthread
TEST_LIBS := -lcheck -lm -lrt
LINKFLAGS := $(COMMON)

# Intel options
#CXX := icpc
#CXXFLAGS := -Wall -Wno-missing-declarations -fast -xSSE2 $(COMMON)

# GNU options
CXX := gcc
# -Wno-write-strings removes the `deprecated conversion from\
 string constant to char*` warnings
CXXFLAGS := -Wall -O0 -std=c99 -march=native $(COMMON)

PROG := libssa libssa_check libssa_example

.SUFFIXES := .o .c

%.o : %.c $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $< -L. -lsdb 

all : init $(PROG)

#mpilibssa.o : src/libssa.cc $(OBJS) $(DEPS)
#	$(CXX) $(CXXFLAGS) -DMPIlibssa $(MPI_COMPILE) -c -o $@ src/libssa.cc

init:
	@echo 'Copying file libsdb.a'
	cp ../libsdb/libsdb.a .

libssa : $(OBJS) $(USR_OBJS) $(DEPS)
	@echo 'Building target: $@'
#	$(CXX) $(LINKFLAGS) -o $@ $(OBJS) $(LIBS)
	ar -cvq libssa.a $(DEPS) $(OBJS)
	@echo 'Finished building target: $@'

#mpilibssa : mpilibssa.o $(OBJS) $(DEPS)
#	@echo 'Building target: $@'
#	$(CXX) $(LINKFLAGS) -o $@ mpilibssa.o $(OBJS) $(LIBS) $(MPI_LINK)
#	@echo 'Finished building target: $@'

libssa_check : $(TESTS) $(OBJS) $(USR_OBJS) $(DEPS)
	@echo 'Building target: $@'
	$(CXX) $(LINKFLAGS) -o $@ $(OBJS) $(TESTS) $(TEST_LIBS) $(LIBS) -L. -lsdb
	@echo 'Finished building target: $@'
	
libssa_example : $(OBJS) $(USR_OBJS) $(DEPS)
	@echo 'Building target: $@'
	$(CXX) $(CXXFLAGS) $(LINKFLAGS) -o $@ ./src/libssa_example.c $(LIBS) -L. -lssa -lsdb
	@echo 'Finished building target: $@'

# clean created files
clean:
	-rm -f $(OBJS) $(TESTS) $(TO_CLEAN) $(PROG) libsdb.a gmon.out
	
	
# to clean later *.gcov tests/*.gcda tests/*.gcno src/*.gcda src/*.gcno

# run tests
check:
	./libssa_check

# run example
example:
	./libssa_example
