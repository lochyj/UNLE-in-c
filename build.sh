clang -I ./include/ -c ./src/main.c -o ./build/main.o
clang -I ./include/ -c ./src/draw_circle.c -o ./build/draw_circle.o
clang -I ./include/ -c ./src/algorithms.c -o ./build/algorithms.o
clang -O2 -o ./build/unle ./build/main.o ./build/draw_circle.o ./build/algorithms.o -lSDL2 -lSDL2_ttf -lm
#clang -g -o ./build/unle ./build/main.o ./build/draw_circle.o ./build/algorithms.o -lSDL2 -lSDL2_ttf -lm
#valgrind ./build/unle
./build/unle
