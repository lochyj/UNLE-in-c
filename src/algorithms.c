#include "algorithms.h"

void fruchtermanreingold(Graph_t* graph, int width, int height) {
    Node_t* nodes = graph->nodes;
    Edge_t* edges = graph->edges;

    int nodeLen = graph->nodeLen;
    int edgeLen = graph->edgeLen;

    double k = sqrt((width * height) / nodeLen); // 160);

    double accel = 4.0;

    for (int i = 0; i < nodeLen; i++) {
        Node_t* node1 = &nodes[i];
        node1->dx = 0;
        node1->dy = 0;
        for (int j = 0; j < nodeLen; j++) {
            if (i == j) continue;
            Node_t* node2 = &nodes[j];
            double delta_x = node1->x - node2->x;
            double delta_y = node1->y - node2->y;
            double distance = sqrt(delta_x * delta_x + delta_y * delta_y);
            if (distance < 1)
                distance = 1;
            double force = (k * k) / distance;
            node1->dx += delta_x / distance * force;
            node1->dy += delta_y / distance * force;
        }
    }

    // Keep the - 1 otherwise it will segfault
    for (int i = 0; i < edgeLen - 1; i++) {
        Edge_t* edge = &edges[i];
        Node_t* node1 = edge->start;
        Node_t* node2 = edge->end;
        double delta_x = node1->x - node2->x;
        double delta_y = node1->y - node2->y;
        double distance = sqrt(delta_x * delta_x + delta_y * delta_y);
        if (distance < 1)
            distance = 1;
        double force = (distance * distance) / k;
        double x = delta_x / distance * force;
        double y = delta_y / distance * force;
        node1->dx -= x;
        node1->dy -= y;
        node2->dx += x;
        node2->dy += y;
    }

    for (int i = 0; i < nodeLen; i++) {
        Node_t* node = &nodes[i];
        double d = nodeLen * node->connections;
        node->x += accel * (node->dx / d);
        node->y += accel * (node->dy / d);
    }
}
