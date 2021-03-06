#
# MHCTrajectorySampler Makefile
#

CC=g++
CCFLAGS=-std=c++0x -Wall -O3

HSRC=src/include/*
CPPSRC=src/*.cpp src/*/*.cpp

OUT=sampler

$(OUT): $(CPPSRC) $(HSRC)
	$(CC) $(CCFLAGS) $(CPPSRC) -o $(OUT)
