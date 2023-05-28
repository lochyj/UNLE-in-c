clang -I ./include/ -c ./src/main.c -o ./build/main.o
clang -I ./include/ -c ./src/draw_circle.c -o ./build/draw_circle.o
clang -o ./build/unle ./build/main.o ./build/draw_circle.o -lSDL2 -lSDL2_ttf
./build/unle
