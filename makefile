FLAGS = -g

relation: relation.c
	gcc $(FLAGS) -o relation relation.c

clean:
	$(RM) relation
