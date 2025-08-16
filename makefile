CC      = gcc
CFLAGS  = -Wall -Werror -std=c11

tripPlan : tripPlan.o WGraph.o
	$(CC) $(CFLAGS) -o tripPlan tripPlan.o WGraph.o

tripPlan.o : tripPlan.c WGraph.h
	$(CC) $(CFLAGS) -c tripPlan.c

Graph.o : WGraph.c WGraph.h
	$(CC) $(CFLAGS) -c WGraph.c

run1: tripPlan
	@echo "Running with test1.txt..."
	@./tripPlan < test1.txt

run2: tripPlan
	@echo "Running with test2.txt..."
	@./tripPlan < test2.txt

run3: tripPlan
	@echo "Running with test3.txt..."
	@./tripPlan < test3.txt

run4: tripPlan
	@echo "Running with test4.txt..."
	@./tripPlan < test4.txt

run5: tripPlan
	@echo "Running with test5.txt..."
	@./tripPlan < test5.txt

.PHONY: all clean run1 run2 run3 run4 run5
clean : 
	rm -f *.o tripPlan