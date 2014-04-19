#
# MHCTrajectorySampler Makefile
#

CC=g++
CCFLAGS=-std=c++0x -Wall

HSRC=src/include/*
CPPSRC=src/*.cpp src/*/*.cpp

OUT=test

$(OUT): $(CPPSRC) $(HSRC)
	$(CC) $(CCFLAGS) $(CPPSRC) -o $(OUT)
