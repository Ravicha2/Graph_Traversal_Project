#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "WGraph.h"

typedef struct{
    char dep_landmark[31];
    char dep_time[5];
    char arv_landmark[31];
    char arv_time[5];
    int timeinmin;

}ferryschedule;

int getduration(ferryschedule *f){
    int dep = atoi(f->dep_time);
    int arv = atoi(f->arv_time);
    int hourtomin = (((arv/100)-(dep/100))*60)+((arv%100)-(dep%100));
    if (hourtomin > 0) {
        return hourtomin;
    }else{
        printf("Invalid schedule");
    }
    return 0;
}

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


Graph walkingLink(Graph g,int links, char **landmarks) {
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
        for (int j =0;j < numOfVertices(g); j++) {
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

Graph ferry(Graph g,int numFerry,char **landmarks){
    ferryschedule *f = malloc(numFerry*sizeof(ferryschedule));
    for (int i=0; i < numFerry; i++) {
        scanf("%30s",f[i].dep_landmark);
        scanf("%4s",f[i].dep_time);
        scanf("%30s",f[i].arv_landmark);
        scanf("%4s",f[i].arv_time);
    }
    for (int j=0;j<numFerry;j++) {
        int e_dep = -1;
        int e_arv = -1;
        for (int k=0;k<numOfVertices(g);k++){
            if (strcmp(f[j].dep_landmark,landmarks[k]) == 0) {
                e_dep = k;
            }   
            if (strcmp(f[j].arv_landmark,landmarks[k]) == 0) {
                e_arv = k;
            }
        }
        if (e_dep != -1 && e_arv != -1) {
            Edge e = {
                e_dep,
                e_arv,
                getduration(&f[j])
            };
            insertEdge(g,e);
        }
    }
    free(f);

    return g;
}

int main(void) {
    int numlandmarks;
    int links;
    int numFerry;
    printf("Number of landmarks: ");
    scanf("%d",&numlandmarks);
    char** landmarks = getlandmark(numlandmarks);
    Graph g = newGraph(numlandmarks);
    printf("Number of walking links: ");
    scanf("%d",&links);
    g = walkingLink(g,links,landmarks);
    printf("Number of ferry schedules: ");
    scanf("%d",&numFerry);
    g = ferry(g,numFerry,landmarks);
    
    free(landmarks);
    free(g);
    printf("Happy travels!");
    return 0;
}