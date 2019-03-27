CFLAGS = -g -std=c99 -lpthread -Wall -Wextra -Werror -Wno-unused 
objects = spell.o

print_list : $(objects)
	cc -o a $(CFLAGS) $(objects)

.PHONY : clean
clean :
	rm a $(objects)
