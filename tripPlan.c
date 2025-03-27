#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "WGraph.h"

char** getlandmark(int numlandmarks){
    char landmarkName[31];
    char **landmarks = malloc(numlandmarks*sizeof(char *));
    for (int i = 0; i < numlandmarks; i++) {
        scanf("%s",landmarkName);
        landmarks[i] = malloc(strlen(landmarkName)+1);
        strcpy(landmarks[i],landmarkName);
    }
    return landmarks;
}

/*
void walkingLink(int links, char **landmarks) {
    for (int i = 0; i < links; i++) {
        
    }
    return ;
}
*/
int main(void) {
    int numlandmarks;
    int links;
    printf("%s","Number of landmarks: ");
    scanf("%d",&numlandmarks);
    char** landmarks = getlandmark(numlandmarks);
    printf("%s","Number of walking links: ");
    scanf("%d",&links);
    return 0;
}