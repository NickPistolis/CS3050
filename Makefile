
PROGRAM_NAME = cs3050project
OBJECT_FILES = $(PROGRAM_NAME).o
CFLAGS = -O2 -Wall -g -Wextra -pedantic -std=c11
$(PROGRAM_NAME) : $(OBJECT_FILES)
	gcc $(CFLAGS) -o $@ $(OBJECT_FILES)

$(PROGRAM_NAME).o : $(PROGRAM_NAME).c input_error.h
	gcc $(CFLAGS) -c $<

clean :
	$(RM) $(PROGRAM_NAME)
	$(RM) $(OBJECT_FILES)
	$(RM) *~ *.bak

run :
	@$(MAKE) && ./$(PROGRAM_NAME) $(ARGS)