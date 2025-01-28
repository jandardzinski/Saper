CFLAGS = -Wall -Wextra -std=c99
TARGET = saper

saper: generacja_planszy.c wczytywanie_planszy.c przebieg_gry.c main.c
	cc $(CFLAGS) generacja_planszy.c wczytywanie_planszy.c przebieg_gry.c main.c -o $(TARGET)

gdb: generacja_planszy.c wczytywanie_planszy.c przebieg_gry.c main.c
	cc -ggdb $(CFLAGS) generacja_planszy.c wczytywanie_planszy.c przebieg_gry.c main.c -o $(TARGET)

run: 
	./$(TARGET)

test: 
	./$(TARGET) test

clean:
	rm -f $(TARGET)
