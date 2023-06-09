#include "main.h"
#include <SDL2/SDL_ttf.h>

/* Important globals */
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Graph_t graph;
TTF_Font* font = NULL;

bool running = true;

int window_width = 640;
int window_height = 480;

int numberOfNodes = 20;

Camera_t camera = {0, 0, 1};

bool tracking_mouse = false;
/* ----------------- */

int main(int argc, char* argv[]) {
    init();

    setup_graph(&graph, (Color_t){black}, (Color_t){black}, numberOfNodes, numberOfNodes + 1, 5);

    float fps = 0;
    while (running) {
        fps = update(fps);
        events();
    }

    cleanup();
    return 0;
}

float update(float fps) {
    uint64_t start = SDL_GetPerformanceCounter();

    SDL_RenderClear(renderer);

    fruchtermanreingold(&graph, window_width, window_height);

    draw_graph(&graph);

    draw_fps(fps);

    SDL_SetRenderDrawColor(renderer, white);
    SDL_RenderPresent(renderer);

    uint64_t end = SDL_GetPerformanceCounter();

    float secondsElapsed = (end - start) / (float) SDL_GetPerformanceFrequency();

    return secondsElapsed;
}

void draw_fps(float fps) {
    char fps_str[10];
    sprintf(fps_str, "%f", fps);
    SDL_Color color = {red};
    SDL_Surface* surface = TTF_RenderText_Solid(font, fps_str, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {0, 0, 80, 40};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void draw_graph(Graph_t* graph) {
    draw_nodes(graph);
    draw_edges(graph);
}

// TODO: move this to a image instead of drawing a circle using math every time we need it...
void draw_nodes(Graph_t* graph) {
    for (int i = 0; i < graph->nodeLen - 1; i++) {
        Node_t* node = &graph->nodes[i];

        //SDL_SetRenderDrawColor(renderer, color(node->color));
        //SDL_RenderFillCircle(renderer, node->x, node->y, node->radius);
        SDL_SetRenderDrawColor(renderer, black);
        SDL_RenderDrawCircle(
            renderer,
            (node->x + camera.x) / camera.zoom,
            (node->y + camera.y) / camera.zoom,
            (node->radius) / camera.zoom
        );

    }
}

void draw_edges(Graph_t* graph) {
    for (int i = 0; i < graph->edgeLen - 1; i++) {
        Edge_t* edge = &graph->edges[i];

        if (!edge->initialized)
            continue;

        SDL_SetRenderDrawColor(renderer, color(edge->color));
        SDL_RenderDrawLine(
            renderer,
            (edge->start->x + camera.x) / camera.zoom,
            (edge->start->y + camera.y) / camera.zoom,
            (edge->end->x + camera.x) / camera.zoom,
            (edge->end->y + camera.y) / camera.zoom
        );
    }
}

void setup_graph(Graph_t* graph, Color_t node_color, Color_t edge_color, int node_len, int edge_len, int node_radius) {
    graph->nodes = malloc(sizeof(Node_t) * (node_len + 1));

    for (int i = 0; i < node_len; i++) {
        graph->nodes[i].id = malloc(sizeof(char) * 128);
    }

    graph->edges = malloc(sizeof(Edge_t) * (edge_len + 1));
    graph->nodeLen = node_len;
    graph->edgeLen = edge_len;

    for (int i = 0; i < node_len; i++) {
        Node_t node;
        node.color = node_color;
        node.seen = false;
        node.x = rand() % window_width;
        node.y = rand() % window_height;
        node.connections = 0;
        node.radius = node_radius;
        graph->nodes[i] = node;
    }

    for (int i = 0; i < edge_len; i++) {
        Edge_t edge;
        edge.color = edge_color;
        edge.value = "edge";
        edge.initialized = false;
        graph->edges[i] = edge;
    }

    //Connects nodes in index order
    for (int i = 0; i < edge_len - 1; i++) {
        Node_t* start = &graph->nodes[i];
        Node_t* end = &graph->nodes[i + 1];
        Edge_t* edge = &graph->edges[i];

        edge->initialized = true;
        edge->start = start;
        edge->end = end;

        start->connections++;
        end->connections++;
    }
}

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    font = TTF_OpenFont("assets/fonts/Roboto-Regular.ttf", 12);

    window = SDL_CreateWindow("UNLE in C!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, white);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    // Initialize camera with appropriate values
    camera.x = 0;
    camera.y = 0;
    camera.zoom = 1;

    camera.max_height = window_height * 10;
    camera.max_width = window_width * 10;
    camera.min_height = window_height / 10;
    camera.min_width = window_width / 10;
    // ---
}

void cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void events() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_MOUSEMOTION:
                if (tracking_mouse) {
                    camera.x += event.motion.xrel * camera.zoom;
                    camera.y += event.motion.yrel * camera.zoom;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT:
                        tracking_mouse = true;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT:
                        tracking_mouse = false;
                        break;
                    default:
                        break;
                }
                break;

            case SDL_MOUSEWHEEL:
                if (event.wheel.y > 0) {
                    camera.zoom -= 0.1;
                } else if (event.wheel.y < 0) {
                    camera.zoom += 0.1;
                }
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;

                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }
    // while (SDL_PollEvent(&event)) {
    //     switch (event.type) {
    //         case SDL_QUIT:
    //             running = false;
    //             break;

    //         case SDL_KEYDOWN:
    //             switch (event.key.keysym.sym) {
    //                 case SDLK_ESCAPE:
    //                     running = false;
    //                     break;

    //                 default:
    //                     break;
    //             }
    //             break;

    //         default:
    //             break;
    //     }
    // }
}