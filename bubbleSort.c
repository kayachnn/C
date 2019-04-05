/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: cihan
 *
 * Created on April 4, 2019, 12:11 AM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */

void swap(int* , int* );
void diziAl(int* [], int );
void diziYaz(int* [], int );
void bubbleSort(int* [], int n);


int main(void) {
    
    int n;
    printf("dizinin eleman sayisi");
    scanf("%d",&n);
    
    int *array;
    array = (int *)malloc(sizeof(int) *n);
    
    diziAl(array,n); 
    bubbleSort(array,n);
    diziYaz(array,n);
    

    return 0;
}

void swap(int *x, int *y){
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

void diziAl(int *arr[],int n){
    for(int i=0;i<n;i++){
        printf("dizi[%d]= ",i+1);
        scanf("%d",(arr + i));
    }
}

void diziYaz(int *arr[],int n){
    for(int i=0;i<n;i++){
        printf("%d ",*(arr +i));
    }
}

void bubbleSort(int *arr[],int n){
    int i;
    int j;
    for(i=0;i<n;i++){
        for(j=0;j<n-i-1;j++){
            if(*(arr + j) > *(arr + j+1)){
                swap((arr + j),(arr +j+1));
            }
        }
        
    }
}