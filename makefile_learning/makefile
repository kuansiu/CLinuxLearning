OUT := main
OBJS := main.o add.o
COM_OP := -Wall -g

$(OUT):$(OBJS)
	gcc $^ -o $@
main.o: main.c compute.h
	gcc -c $< -o $@ $(COM_OP)
add.o: add.c compute.h
	gcc -c $< -o $@ $(COM_OP)

.PHONY: clean rebuilt

clean:
	rm -f $(OUT) $(OBJS)

rebuilt:clean $(OUT)

