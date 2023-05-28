#include "main.h"
#include <SDL2/SDL_ttf.h>

/* Important globals */
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool running = true;
Graph_t graph;
TTF_Font* font = NULL;

int window_width = 640;
int window_height = 480;
/* ----------------- */

int main(int argc, char* argv[]) {
    init();

    setup_graph((Color_t){white}, (Color_t){black}, 100, 10, 5);

    float fps = 0;
    while (running) {
        fps = update(fps);
        events();
    }

    cleanup();
    return 0;
}

int update(int fps) {
    uint64_t start = SDL_GetPerformanceCounter();

    SDL_RenderClear(renderer);

    draw_graph(&graph);

    draw_fps(fps);

    SDL_SetRenderDrawColor(renderer, white);
    SDL_RenderPresent(renderer);

    uint64_t end = SDL_GetPerformanceCounter();

    float secondsElapsed = (end - start) / (float)SDL_GetPerformanceFrequency();

    return secondsElapsed;
}

void draw_fps(int fps) {
    char* fps_str = malloc(sizeof(char) * 10);
    sprintf(fps_str, "%d", fps);
    SDL_Color color = {red};
    SDL_Surface* surface = TTF_RenderText_Solid(font, fps_str, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {50, 50, 100, 100};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    free(fps_str);
}

void draw_graph(Graph_t* graph) {
    draw_nodes(graph);
    // draw_edges(&graph);
}

void draw_nodes(Graph_t* graph) {
    for (int i = 0; i < graph->nodeLen; i++) {
        Node_t* node = &graph->nodes[i];
        // SDL_SetRenderDrawColor(renderer, color(node->color));
        // SDL_RenderFillCircle(renderer, node->x, node->y, node->radius);
        SDL_SetRenderDrawColor(renderer, black);
        SDL_RenderDrawCircle(renderer, node->x, node->y, node->radius);
    }
}

void setup_graph(Color_t node_color, Color_t edge_color, int node_len, int edge_len, int node_radius) {
    graph.nodes = malloc(sizeof(Node_t) * node_len);
    graph.edges = malloc(sizeof(Edge_t) * edge_len);
    graph.nodeLen = node_len;
    graph.edgeLen = edge_len;

    for (int i = 0; i < node_len; i++) {
        Node_t node;
        node.color = node_color;
        node.seen = false;
        node.id = "node";
        node.x = rand() % window_width;
        node.y = rand() % window_height;
        node.radius = node_radius;
        graph.nodes[i] = node;
    }

    for (int i = 0; i < edge_len; i++) {
        Edge_t edge;
        edge.color = edge_color;
        edge.value = "edge";
        graph.edges[i] = edge;
    }
}

void init() {
    SDL_Init(SDL_INIT_VIDEO);

    font = TTF_OpenFont("assets/fonts/Roboto-Regular.ttf", 24);

    window = SDL_CreateWindow("UNLE in C!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, white);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    running = false;
}

void events() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }
}