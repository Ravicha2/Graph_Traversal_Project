#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "WGraph.h"

/*-------------helper function--------------------*/

int getduration(ferryschedule *f){        //duration traveled by ferry
    int dep = atoi(f->dep_time);          //convert XXXX format to compute time in minutes
    int arv = atoi(f->arv_time);
    int hourtomin = (((arv/100)-(dep/100))*60)+((arv%100)-(dep%100));
    if (hourtomin > 0) {
        return hourtomin;
    }
    return 0;
}

char *getArrival(char *departAt, int duration) { //use departure time in XXXX and the edge
    static char arriveAt[21];                    //to get arrival time then convert to XXXX format
    int departTime = atoi(departAt);
    int intHr = departTime/100;
    int intMin = (departTime%100) + duration;

    if (intMin >= 60){
        intHr += intMin/60;
        intMin = intMin%60;
    }
    sprintf(arriveAt, "%02d%02d",intHr ,intMin);    //turn int to string in XXXX

    return arriveAt;
}

char** getlandmark(int numlandmarks){        //  pointer to arrays of landmark names
    char landmarkName[31];
    char **landmarks = malloc(numlandmarks*sizeof(char *));     
    assert(landmarks != NULL);

    for (int i = 0; i < numlandmarks; i++) {
        scanf("%30s",landmarkName);
        landmarks[i] = malloc(strlen(landmarkName)+1);      
        assert(landmarks[i] != NULL);

        strcpy(landmarks[i],landmarkName);   //landmarks' Name
    }
    return landmarks;
}

/*----------- Main function-------------------*/

void walkingLink(Graph g,int links, char **landmarks) { //add edges to graph g
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
            if (strcmp(from,landmarks[j]) == 0) {       //compare input with existed landmarks
                eFrom = j;
            }
            if (strcmp(to,landmarks[j]) == 0) {         //compare input with existed landmarks
                eTo = j;
            }
        }
        e.v = eFrom; e.w = eTo; e.weight = time;        //Since walking edges exist on both side
        insertEdge(g, e);
        e.v = eTo; e.w = eFrom; e.weight = time;        //Since walking edges exist on both side
        insertEdge(g, e);
    }
}

ferryDetail ferry(Graph g,int numFerry,char **landmarks){           //array of ferries
    ferryschedule *f = malloc(numFerry*sizeof(ferryschedule));      //including schedule number and time
    assert(f != NULL);                                              //and how can node traversed through(walk or ferry)
    for (int i=0; i < numFerry; i++) {                              
        scanf("%30s",f[i].dep_landmark);                            //ferry schedules stored
        scanf("%4s",f[i].dep_time);
        scanf("%30s",f[i].arv_landmark);
        scanf("%4s",f[i].arv_time);
    }
    
    int *landmarkType = calloc(numOfVertices(g)+numFerry,sizeof(int));      //for checking if landmarks can be traveled by ferry
    assert(landmarkType != NULL);                                           //if 0: can only visited through walking
                                                                            //if 1: can visited by both ferry and walk
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

    ferryDetail takeFerry = {landmarkType,f,numFerry};          //store detail of landmarks with ferry
    return takeFerry;
}

void tripPlan(Graph g,char **landmarks,ferryDetail takeFerry) { // to print result
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
        printf("\n");

        int indexFrom = -1;
        int indexTo = -1;
        for (int i=0;i<numOfVertices(g);i++){
            if (strcmp(fromLandmark,landmarks[i]) == 0) {
                indexFrom = i; //src
            }   
            if (strcmp(tolandmark,landmarks[i]) == 0) {
                indexTo = i; //dest
            }
        }
        Path p = Dijkstra(g,indexFrom,indexTo,takeFerry,departAt,landmarks);
        if (numsteps(p) == 1){
            printf("No route.\n");
        }
        int ferryIndex;
        for (int s =0; s < numsteps(p)-1; s++){
            int duration = adjacent(g,steps(p,s),steps(p,s+1)); 
            
            if (duration>0) {
                char *arriveAt = getArrival(departAt,distance(p,s+1)-distance(p,s));
                
                for (int j=0; j < takeFerry.numFerry; j++) {
                    if ((strcmp(takeFerry.schedules[j].dep_landmark, landmarks[steps(p,s)]) == 0) && (strcmp(takeFerry.schedules[j].arv_landmark, landmarks[steps(p,s+1)]) == 0)) {
                        ferryIndex = j;
                    }
                }
            
                if ((takeFerry.landmarkType[steps(p,s)] == 1) && (takeFerry.landmarkType[steps(p,s+1)] == 1)) {
                    if (atoi(departAt) <= atoi(takeFerry.schedules[ferryIndex].dep_time)) {
                        printf("Ferry %d minute(s):\n",duration);
                        printf("  %s %s\n", takeFerry.schedules[ferryIndex].dep_time,landmarks[steps(p,s)]);     
                        printf("  %s %s\n", takeFerry.schedules[ferryIndex].arv_time,landmarks[steps(p,s+1)]);
                        strcpy(departAt,takeFerry.schedules[ferryIndex].arv_time);
                    }else{
                        printf("No route.\n");
                    }
                }else{
                    printf("Walk %d minute(s):\n",duration);
                    printf("  %s %s\n",departAt,landmarks[steps(p,s)]);     
                    printf("  %s %s\n",arriveAt,landmarks[steps(p,s+1)]); 
                    strcpy(departAt,arriveAt);
                }
            }else{
                printf("No route.\n");
            }
        }
        resetPath(p); //free malloc'd data
    }
}

int main(void) {
    int numlandmarks;                               //initialise
    printf("Number of landmarks: ");
    scanf("%d",&numlandmarks);
    char** landmarks = getlandmark(numlandmarks);   
    ferryDetail takeFerry;
    Graph g = newGraph(numlandmarks);
    
    int links;                                      //add walking path
    printf("Number of walking links: ");
    scanf("%d",&links);
    walkingLink(g,links,landmarks);

    int numFerry;                                   //add ferry
    printf("Number of ferry schedules: ");
    scanf("%d",&numFerry);
    takeFerry = ferry(g,numFerry,landmarks);

    tripPlan(g,landmarks,takeFerry);              //display result

    printf("Happy travels!\n");

    free(takeFerry.landmarkType);                   //free memory
    free(takeFerry.schedules);
    for (int i = 0; i < numlandmarks; i++) {
        free(landmarks[i]);
    }
    free(landmarks);
    freeGraph(g);
    return 0;
}