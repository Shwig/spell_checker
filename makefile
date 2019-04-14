CFLAGS = -g -std=gnu99 -lpthread -pthread -Wall -Wextra -Werror
objects = spell.o spell_utils.o socket_utils.o

print_list : $(objects)
	cc -o a $(CFLAGS) $(objects)

.PHONY : clean
clean :
	rm a $(objects)
