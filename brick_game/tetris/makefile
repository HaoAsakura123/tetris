CC := gcc
CFLAGS := -I inc -std=c99 
OBJS_FSMSWITCHCASE := out/frogger.o out/fsm.o out/backend.o out/frontend.o
OBJS_FSMTABLE := out/frogger.o out/fsm_matrix.o out/backend.o out/frontend.o

frogger: $(OBJS_FSMSWITCHCASE)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

frogger_fsmtable: $(OBJS_FSMTABLE)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

out/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf game
zxc: clean
	$(CC) $(CFLAGS) main.c -o game -lncurses
clang:
	clang-format -i *.c
