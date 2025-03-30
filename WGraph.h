// Weighted Graph ADT interface ... COMP9024 25T1

typedef struct GraphRep *Graph;
typedef struct ShortestPath *Path;
// vertices are ints
typedef int Vertex;

// edges are pairs of vertices (end-points) with a weight
typedef struct Edge {
   Vertex v;
   Vertex w;
   int    weight;
} Edge;

Graph newGraph(int);
int   numOfVertices(Graph);
void  insertEdge(Graph, Edge);
void  removeEdge(Graph, Edge);
int   adjacent(Graph, Vertex, Vertex);  // returns weight, or 0 if not adjacent
void  showGraph(Graph);
void  freeGraph(Graph);

Path Djikstra(Graph,Vertex,Vertex);
int numsteps(Path p);
int steps(Path p,int i);
int distance(Path p);
void resetPath(Path p);