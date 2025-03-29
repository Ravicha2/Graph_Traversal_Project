#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
typedef struct{
    int *landmarkType;
    ferryschedule *schedules;
    int numFerry;
}ferryDetail;

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

char *getArrival(char *departAt, int duration) {
    static char arriveAt[21];
    int departTime = atoi(departAt);
    int intHr = departTime/100;
    int intMin = (departTime%100) + duration;

    if (intMin > 60){
        intHr += intMin/60;
        intMin = intMin%60;
    }

    sprintf(arriveAt, "%02d%02d",intHr ,intMin);

    
    return arriveAt;
}

char** getlandmark(int numlandmarks){
    char landmarkName[31];
    char **landmarks = malloc(numlandmarks*sizeof(char *));
    assert(landmarks != NULL);
    for (int i = 0; i < numlandmarks; i++) {
        scanf("%30s",landmarkName);
        landmarks[i] = malloc(strlen(landmarkName)+1);
        assert(landmarks[i] != NULL);
        strcpy(landmarks[i],landmarkName);
    }
    return landmarks;
}


void walkingLink(Graph g,int links, char **landmarks) {
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
        e.v = eTo; e.w = eFrom; e.weight = time;
        insertEdge(g, e);
    }
    return;
}

ferryDetail ferry(Graph g,int numFerry,char **landmarks){
    ferryschedule *f = malloc(numFerry*sizeof(ferryschedule));
    assert(f != NULL);
    for (int i=0; i < numFerry; i++) {
        scanf("%30s",f[i].dep_landmark);
        scanf("%4s",f[i].dep_time);
        scanf("%30s",f[i].arv_landmark);
        scanf("%4s",f[i].arv_time);
    }

    int *landmarkType = calloc(numOfVertices(g)+numFerry,sizeof(int));
    assert(landmarkType != NULL);
    for (int j=0;j<numFerry;j++) {
        int e_dep = -1;
        int e_arv = -1;
        for (int k=0;k<numOfVertices(g);k++){
            if (strcmp(f[j].dep_landmark,landmarks[k]) == 0) {
                e_dep = k;
                landmarkType[k] = 1;
            }   
            if (strcmp(f[j].arv_landmark,landmarks[k]) == 0) {
                e_arv = k;
                landmarkType[k] = 1;
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
    ferryDetail takeFerry = {landmarkType,f,numFerry};
    return takeFerry;
}

void tripPlan(Graph g,char **landmarks,ferryDetail takeFerry) {
    char fromLandmark[31];
    char tolandmark[31];
    char departAt[5];
    while (true) {
        printf("\nFrom: ");
        scanf("%s",fromLandmark);
        if (strcmp(fromLandmark,"done") == 0) {
            break;
        }    
        printf("To: ");
        scanf("%s",tolandmark);
        if (strcmp(tolandmark,"done") == 0) {
            break;
        }
        printf("Departure time: ");
        scanf("%s",departAt);
        if (strcmp(departAt,"done") == 0) {
            break;
        }
        int indexFrom = -1;
        int indexTo = -1;
        int duration;

        for (int i=0;i<numOfVertices(g);i++){
            if (strcmp(fromLandmark,landmarks[i]) == 0) {
                indexFrom = i;
            }   
            if (strcmp(tolandmark,landmarks[i]) == 0) {
                indexTo = i;
            }
        }
        duration = adjacent(g,indexFrom,indexTo);
        char *arriveAt = getArrival(departAt,duration);
        int ferryIndex = 0;
        for (int j=0; j < takeFerry.numFerry; j++) {
            if ((strcmp(takeFerry.schedules[j].dep_landmark,landmarks[indexFrom]) == 0) && (strcmp(takeFerry.schedules[j].arv_landmark,landmarks[indexTo]) == 0)) {
                ferryIndex = j;
            }
        }

        if (duration > 0){
            if ((takeFerry.landmarkType[indexFrom] == 1) && (takeFerry.landmarkType[indexTo] == 1)) {
                if (atoi(departAt) < atoi(takeFerry.schedules[indexFrom].dep_time)) {
                printf("\nFerry %d minute(s):\n",duration);
                printf("  %s %s\n", takeFerry.schedules[ferryIndex].dep_time,landmarks[indexFrom]);
                printf("  %s %s\n", takeFerry.schedules[ferryIndex].arv_time,landmarks[indexTo]);
                } else{
                    printf("No route.");
                }
            }else{
                printf("\nWalk %d minute(s):\n",duration);
                printf("  %s %s\n",departAt,landmarks[indexFrom]);
                printf("  %s %s\n",arriveAt,landmarks[indexTo]);
            }
        }else{
            printf("No route.");
        }
    }
    return;
}

int main(void) {
    int numlandmarks;
    printf("Number of landmarks: ");
    scanf("%d",&numlandmarks);
    char** landmarks = getlandmark(numlandmarks);
    ferryDetail takeFerry;
    Graph g = newGraph(numlandmarks);
    
    int links;
    printf("Number of walking links: ");
    scanf("%d",&links);
    walkingLink(g,links,landmarks);

    int numFerry;
    printf("Number of ferry schedules: ");
    scanf("%d",&numFerry);
    takeFerry = ferry(g,numFerry,landmarks);

    tripPlan(g,landmarks,takeFerry);

    printf("Happy travels!");

    free(takeFerry.landmarkType);
    free(takeFerry.schedules);
    for (int i = 0; i < numlandmarks; i++) {
        free(landmarks[i]);
    }
    free(landmarks);
    freeGraph(g);
    return 0;
}