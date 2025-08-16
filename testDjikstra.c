#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "WGraph.h"

int main() {
    Graph g = newGraph(8);
    Edge e1 = {0,1,1};
    Edge e2 = {0,2,3};
    Edge e3 = {1,2,1};
    Edge e4 = {1,3,4};
    Edge e5 = {2,3,1};
    Edge e6 = {2,4,6};
    Edge e7 = {3,4,1};
    Edge e8 = {3,7,3};
    Edge e9 = {3,5,1};
    Edge e10 = {4,7,4};
    Edge e11 = {5,6,5};
    Edge e12 = {6,1,2};
    Edge e13 = {6,3,3};

    insertEdge(g,e1);
    insertEdge(g,e2);
    insertEdge(g,e3);
    insertEdge(g,e4);
    insertEdge(g,e5);
    insertEdge(g,e6);
    insertEdge(g,e7);
    insertEdge(g,e8);
    insertEdge(g,e9);
    insertEdge(g,e10);
    insertEdge(g,e11);
    insertEdge(g,e12);
    insertEdge(g,e13);
    
    Path s = Djikstra(g,0,6);
    printf("%d\n",numsteps(s));
    for (int i = 0; i< numsteps(s);i++) {
        printf("%d",steps(s,i));
    }
    printf("\n%d",distance(s));
    return 0;
}