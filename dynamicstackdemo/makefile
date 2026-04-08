dynamicstackdemo: main.o dynamicstack.o dynamicstack.h
	gcc  -Wall main.o dynamicstack.o -o $@
%.o: %.c dynamicstack.h
	gcc -c -Wall $< -o $@
clean:
	rm dynamicstack.o main.o dynamicstackdemo
