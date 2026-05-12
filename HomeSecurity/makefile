PROGRAM = homesecurity
COMPILER = gcc
FLAGS =  -lwiringPi -lpthread
SOURCES = project.c threads.c button.c led.c lcd.c reed.c ir.c buzzer.c
OBJECTS = $(SOURCES: %.c = %.o)
HEADERS = $(SOURCES: %.c = %.h)

$(PROGRAM): $(OBJECTS) $(HEADERS)
	$(COMPILER) $(OBJECTS) $(FLAGS) -o $@
%.o: %.c 
	$(COMPILER) -c $< -o $@

project.o : project.c project.h threads.h button.h led.h lcd.h reed.h ir.h buzzer.h
threads.o : threads.c threads.h project.h button.h led.h lcd.h reed.h ir.h buzzer.h 
button.o : button.c button.h
led.o : led.c led.h
lcd.o : lcd.c lcd.h
reed.o : reed.c reed.h
ir.o : ir.c ir.h
buzzer.o : buzzer.c buzzer.h
	
clean:
	rm $(OBJECTS) $(PROGRAM)

