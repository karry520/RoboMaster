.PHONY:all clean

##
PWD_DIR=$(shell pwd)
SRC_DIR=$(PWD_DIR)/src
OBJ_DIR=$(PWD_DIR)/obj
MAIN_DIR=$(PWD_DIR)/main
LIB_DIR=$(PWD_DIR)/lib
INC_DIR=$(PWD_DIR)/inc

##
CC=g++
CFLAG=-Wall -g -I $(INC_DIR) $(shell pkg-config --cflags opencv) -L $(shell pkg-config --libs opencv) -lwiringPi 

##
export PWD_DIR SRC_DIR OBJ_DIR MAIN_DIR LIB_DIR INC_DIR CC CFLAG

##
all:
	make -C $(SRC_DIR)
	make -C $(MAIN_DIR)
	make -C $(OBJ_DIR)
##
clean:
	$(RM) -rf $(OBJ_DIR)/*.o
	$(RM) robomaster
