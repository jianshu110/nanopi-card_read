###########################################  
#Makefile for mxj
###########################################  
INC=  
LIB=-lmicrohttpd -lcurl -pthread
  
CC=gcc  
CC_FLAG=-Wall  
  
PRG=card_reader_adaptor_bin
OBJ=beep.o gpio_adaptor.o main.o open_door.o
  
$(PRG):$(OBJ)  
	$(CC) $(INC) $(LIB) -o $@ $(OBJ)  
      
.SUFFIXES: .c .o .cpp  
.cpp.o:  
	$(CC) $(CC_FLAG) $(INC) -c $*.cpp -o $*.o  
  
.PRONY:clean  
clean:  
	@echo "Removing linked and compiled files......"  
	rm -f $(OBJ) $(PRG)
