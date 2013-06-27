#The compiler
CC = gcc
#the flags to be used in compilation
CFLAGS = -std=c11 -O2 -Wall -Werror
#the flags to be used in linking
LFLAGS = 
#the o files, if you add more c files you will need to put the corresponding ofiles here
OFILES = driver.o student_cache.o
EXE_NAME = simulator

.PHONY: clean

#the main target
$(EXE_NAME):$(OFILES)
	$(CC) -o $@ $(LFLAGS) $(OFILES)
driver.o:student_cache.h driver.c
#you may need to change this target if you add more headers
student_cache.o:student_cache.c student_cache.h

clean:
	rm -f $(EXE_NAME)
	rm -f $(OFILES)

#you may need to add more targets if you add more c files
