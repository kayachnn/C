

/* 
 * File:   selectionSort.c
 * Author: cihanKaya
 * mail: kayachnn@gmail.com
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>



/*
 * 
 */

void diziAl(int *[],int);
void diziYAz(int *[],int);
void swap(int *,int *);
void selectionSort(int* [], int n);


int main(void){
    
    
    int n;
    printf("dizinizin eleman sayisi\n");
    scanf("%d",&n);
   
    int *array;
    array = (int *)malloc(n*sizeof(int));
    
    diziAl(array,n);
    selectionSort(array,n);
    diziYaz(array,n);
    
    return 0;
}



void diziAl(int *array[],int n){
    for(int i=0;i<n;i++){
        printf("dizi[%d]= ",i+1);
        scanf("%d",(array+i));
    }
}


void diziYaz(int *array[],int n){
    for(int i=0;i<n;i++){
        printf("%d ",*(array +i));
    }
}

void swap(int *x,int *y){//swap islemini yapar
    int tmp = *x;
    *x =*y;
    *y = tmp;
}

void selectionSort(int *array[],int n){
    //her seferinde minimumu bulup 
    //i. indisin oldugu elemanla swap yapar
    int i;
    int j;
    int min_index;
    
    for(i=0;i<n-1;i++){
        min_index = i;
        for(j=i+1;j<n;j++){
            if(*(array +j) < *(array + i)){
                min_index=j;
            }
        }
        swap((array +i),(array + min_index));
    }
}
