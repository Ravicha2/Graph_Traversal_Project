// Weighted Directed Graph ADT
// Adjacency Matrix Representation ... COMP9024 25T1
#include "WGraph.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct GraphRep {
   int **edges;  // adjacency matrix storing positive weights
		 // 0 if nodes not adjacent
   int nV;       // #vertices
   int nE;       // #edges
} GraphRep;

typedef struct ShortestPath{
   int *steps;
   int numstep;
   int *stepweight;
}ShortestPath;


Graph newGraph(int V) {
   assert(V >= 0);
   int i;

   Graph g = malloc(sizeof(GraphRep));
   assert(g != NULL);
   g->nV = V;
   g->nE = 0;

   // allocate memory for each row
   g->edges = malloc(V * sizeof(int *));
   assert(g->edges != NULL);
   // allocate memory for each column and initialise with 0
   for (i = 0; i < V; i++) {
      g->edges[i] = calloc(V, sizeof(int));
      assert(g->edges[i] != NULL);
   }

   return g;
}

int numOfVertices(Graph g) {
   return g->nV;
}

// check if vertex is valid in a graph
int validV(Graph g, Vertex v) {
   return (g != NULL && v >= 0 && v < g->nV);
}

void insertEdge(Graph g, Edge e) {
   assert(g != NULL && validV(g,e.v) && validV(g,e.w));

   if (g->edges[e.v][e.w] == 0) {   // edge e not in graph
      g->edges[e.v][e.w] = e.weight; //weight instead of 1
      g->nE++;
   }
}

void removeEdge(Graph g, Edge e) {
   assert(g != NULL && validV(g,e.v) && validV(g,e.w));

   if (g->edges[e.v][e.w] != 0) {   // edge e in graph
      g->edges[e.v][e.w] = 0;
      g->nE--;
   }
}

int adjacent(Graph g, Vertex v, Vertex w) {
   assert(g != NULL && validV(g,v) && validV(g,w));

   return g->edges[v][w];
}

void showGraph(Graph g) {
    assert(g != NULL);
    int i, j;

    printf("Number of vertices: %d\n", g->nV);
    printf("Number of edges: %d\n", g->nE);
    for (i = 0; i < g->nV; i++)
       for (j = 0; j < g->nV; j++)
	  if (g->edges[i][j] != 0)
	     printf("Edge %d - %d: %d\n", i, j, g->edges[i][j]);
}

void freeGraph(Graph g) {
   assert(g != NULL);

   int i;
   for (i = 0; i < g->nV; i++)
      free(g->edges[i]);
   free(g->edges);
   free(g);
}

int minDistance(Graph g, int dist[], bool visited[]) {
    int min = INT16_MAX;
    int minIndex = -1;
    for (int i = 0; i < numOfVertices(g); i++) {
        if (!visited[i] && dist[i] <= min) {
            min = dist[i];
            minIndex = i;
        }
    }
    return minIndex;
}

Path Djikstra(Graph g, Vertex v, Vertex w) {
   int dist[numOfVertices(g)];
   bool visited[numOfVertices(g)];      
   int pred[numOfVertices(g)];
   int minIndex = v;
   //init dist
   for (int i = 0; i < numOfVertices(g); i++) {
      dist[i] = INT16_MAX;
      pred[i] = -1;
      visited[i] = false;
   }
   //find shortest path
   dist[v] = 0;
   for (int j = 0; j < numOfVertices(g)-1; j++) {
      minIndex = minDistance(g,dist,visited);
      visited[minIndex] = true;
      if (minIndex == -1) break;

      for (int k=0; k < numOfVertices(g); k++) {
         if (g->edges[minIndex][k]>0 && dist[minIndex] != INT16_MAX) {       //djikstra
            if (dist[minIndex] + g->edges[minIndex][k] < dist[k]){
               dist[k] = dist[minIndex] + g->edges[minIndex][k];
               pred[k] = minIndex;
            }
         }        
      }  
   }
   // find pred of dest then find path back to src through pred
   int reversedsteps[numOfVertices(g)];
   int backtrack = w;
   int r = 0;

   // Create path object
    Path p = malloc(sizeof(struct ShortestPath));
    if (p == NULL) return NULL;

   while (backtrack >=0) {
      reversedsteps[r] = backtrack;
      backtrack = pred[backtrack];
      r++;
   }

   p->steps = malloc((r)*sizeof(int));
   assert(p->steps != NULL);
   p->stepweight = malloc((r)*sizeof(int));
   assert(p->stepweight != NULL);

   for (int i = r-1; i >= 0; i--){
      p->steps[r-i-1] = reversedsteps[i];
      p->stepweight[r-i-1] = dist[reversedsteps[i]];
   }
   p->numstep = r;
   return p;
}

int numsteps(Path p) {
    return p->numstep;
}

int steps(Path p, int i) {
   return p->steps[i];
}

int distance(Path p,int i) {
   return p->stepweight[i];
}

void resetPath(Path p) {
   free(p->steps);
}