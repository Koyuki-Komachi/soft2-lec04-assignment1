bin/paint: bin/canvas.o bin/com_his_res.o bin/display.o bin/math.o bin/paintapp.o
	gcc bin/canvas.o bin/com_his_res.o bin/display.o bin/math.o bin/paintapp.o -o bin/paint

bin/paintapp.o: src/paintapp.c 
	gcc -c -I./include src/paintapp.c -o bin/paintapp.o

bin/math.o: src/math.c 
	gcc -c src/math.c -o bin/math.o

bin/display.o: src/display.c
	gcc -c -I./include src/display.c -o bin/display.o

bin/com_his_res.o: src/com_his_res.c 
	gcc -c -I./include src/com_his_res.c -o bin/com_his_res.o

bin/canvas.o: src/canvas.c 
	gcc -c -I./include src/canvas.c -o bin/canvas.o

PHONY: clean

clean:
	rm -f bin/*