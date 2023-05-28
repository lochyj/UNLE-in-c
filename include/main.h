#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "draw.h"

/* Color macros */
#define white 255, 255, 255, 255
#define black 0, 0, 0, 255
#define red 255, 0, 0, 255
#define green 0, 255, 0, 255
#define blue 0, 0, 255, 255
/* ------------ */

// define a macro that expands Color_t to r, g, b, a

#define color(color) color.r, color.g, color.b, color.a

typedef struct {
    int r;
    int g;
    int b;
    int a;
} Color_t;

typedef struct {
    int x;
    int y;
    int dx;
    int dy;
    int radius;
    Color_t color;
    bool seen;
    char* id;
} Node_t;

typedef struct {
    Node_t* start;
    Node_t* end;
    Color_t color;
    char* value;
} Edge_t;

typedef struct {
    Node_t* nodes;
    Edge_t* edges;
    int nodeLen;
    int edgeLen;
} Graph_t;

void init();
void cleanup();
void events();
void draw_nodes(Graph_t* graph);
void draw_graph(Graph_t* graph);
void setup_graph(Color_t node_color, Color_t edge_color, int node_len, int edge_len, int node_radius);
int  update(int frame_time);
void draw_fps(int fps);
