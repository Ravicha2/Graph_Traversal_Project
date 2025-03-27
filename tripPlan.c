#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "WGraph.h"

char** getlandmark(int numlandmarks){
    char landmarkName[31];
    char **landmarks = malloc(numlandmarks*sizeof(char *));
    for (int i = 0; i < numlandmarks; i++) {
        scanf("%30s",landmarkName);
        landmarks[i] = malloc(strlen(landmarkName)+1);
        strcpy(landmarks[i],landmarkName);
    }
    return landmarks;
}


Graph walkingLink(Graph g,int numlandmarks,int links, char **landmarks) {
    char from[31];
    char to[31];
    int time = 0;
    int eFrom = -1;
    int eTo = -1;
    Edge e;
    for (int i = 0; i < links; i++) {
        scanf("%30s",from);
        scanf("%30s",to);
        scanf("%d",&time);
        eFrom = -1;
        eTo = -1;
        for (int j =0;j < numlandmarks; j++) {
            if (strcmp(from,landmarks[j]) == 0) {
                eFrom = j;
            }
            if (strcmp(to,landmarks[j]) == 0) {
                eTo = j;
            }
        }
        e.v = eFrom; e.w = eTo; e.weight = time;
        insertEdge(g, e);
    }
    return g;
}

int main(void) {
    int numlandmarks;
    int links;
    printf("%s","Number of landmarks: ");
    scanf("%d",&numlandmarks);
    char** landmarks = getlandmark(numlandmarks);
    Graph g = newGraph(numlandmarks);
    printf("%s","Number of walking links: ");
    scanf("%d",&links);
    g = walkingLink(g,numlandmarks,links,landmarks);
    showGraph(g);


    free(landmarks);
    free(g);
    pritnf("%s","Happy travels!");
    return 0;
}