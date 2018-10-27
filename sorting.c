//
//  sorting.c
//
//
//  Created by Bayram Muradov on 11/02/2017.
//  Copyright © 2017 Bayram Muradov. All rights reserved.
//
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//Selection Sort and its helper functions:
void selectionSort(int *arr, int size, int &compCount, int &moveCount) {
    for (int last = size-1; last >= 1; --last) {
        int smallest = indexOfSmallest(arr, last+1, compCount);
        swap(arr[smallest], arr[last], moveCount);
    }
}

void swap(int &x, int &y, int &z) {
    int temp = x;
    x = y;
    y = temp;
    z=z+3;
}

int indexOfSmallest(const int theArray[], int size, int &compCount) {
    int indexSoFar = 0;
    for (int currentIndex=1; currentIndex<size; ++currentIndex)
    {
        compCount++; // making comparison each time the for loop iterates
        if (theArray[currentIndex] < theArray[indexSoFar]) {
            indexSoFar = currentIndex;
        }
    }
    return indexSoFar;
}

//Merge Sort Algorithm and its helper functions
void mergeSort(int *arr, int size, int &compCount, int &moveCount) {
    int first = 0;
    int last = size-1;
    mergesortRec(arr, first, last, compCount, moveCount);
}

void mergesortRec (int *arr, int first, int last, int &compCount, int &moveCount) {
    
    compCount++;
    if (first < last) {
        int mid = (first + last)/2; 	// index of midpoint
        //cout<<"-----"<<endl;
        mergesortRec(arr, first, mid, compCount, moveCount);
        //cout<<"******"<<endl;
        mergesortRec(arr, mid+1, last, compCount, moveCount);
        
        // merge the two halves
        merge(arr, first, mid, last, compCount, moveCount);
        //cout<<"-----"<<endl;
        
    }
}  // end mergesort


void merge( int theArray[], int first, int mid, int last, int &cCount, int &mCount) {
    
    int tempArray[MAX_SIZE]; 	// temporary array
    
    int first1 = first; 	// beginning of first subarray
    int last1 = mid; 		// end of first subarray
    int first2 = mid + 1;	// beginning of second subarray
    int last2 = last;		// end of second subarray
    int index = first1; // next available location in tempArray
    
    while((first1<=last1)&&(first2<=last2)) {
        cCount++;
        if(theArray[first1]>=theArray[first2]) {
            tempArray[index] = theArray[first1];
            mCount++;
            first1++;
        }
        else
        {
            mCount++;
            tempArray[index]=theArray[first2];
            first2++;
        }
        index++;
    }
    
    while(first1<=last1)
    {
        cCount++;
        tempArray[index]=theArray[first1];
        mCount++;
        first1++;
        index++;
    }
    
    while(first2<=last2)
    {
        cCount++;
        tempArray[index]=theArray[first2];
        mCount++;
        first2++;
        index++;
    }
    for(index=first; index<=last; index++) {
        cCount++;
        theArray[index]=tempArray[index];
        mCount++;
    }
}  // end merge

//Quick Sort algorithm and its helper functions
void quickSort(int *arr, int size, int &compCount, int &moveCount) {
    int first=0;
    int last=size-1;
    quicksortRec(arr, first, last, compCount, moveCount);
}


void quicksortRec(int theArray[], int first, int last, int &compCount, int &mCount) {
    // Precondition: theArray[first..last] is an array.
    // Postcondition: theArray[first..last] is sorted.
    
    int pivotIndex= theArray[0];
    
    compCount++;
    if (first < last) {
        
        // create the partition: S1, pivot, S2
        partition(theArray, first, last, pivotIndex, compCount, mCount);
        
        // sort regions S1 and S2
        quicksortRec(theArray, first, pivotIndex-1, compCount, mCount);
        quicksortRec(theArray, pivotIndex+1, last, compCount, mCount);
    }
}

void partition(int theArray[], int first, int last, int &pivotIndex, int &c, int &m) {
    // Precondition: theArray[first..last] is an array; first <= last.
    // Postcondition: Partitions theArray[first..last] such that:
    //   S1 = theArray[first..pivotIndex-1] < pivot
    //   theArray[pivotIndex] == pivot
    //   S2 = theArray[pivotIndex+1..last] >= pivot
    
    
    //we will choose pivot as the first element of theArray
    int pivot = theArray[first];
    
    // initially, everything but pivot is in unknown
    int lastS1 = first;           // index of last item in S1
    int firstUnknown = first + 1; // index of first item in unknown
    
    // move one item at a time until unknown region is empty
    for (  ; firstUnknown <= last; ++firstUnknown) {
        // Invariant: theArray[first+1..lastS1] < pivot
        //            theArray[lastS1+1..firstUnknown-1] >= pivot
        
        // move item from unknown to proper region
        c+=2; //makes 2 comparisons @ each time the loop itaretes
        if (theArray[firstUnknown] > pivot) {  	// belongs to S1
            ++lastS1;
            swap(theArray[firstUnknown], theArray[lastS1], m);
        }	// else belongs to S2
    }
    // place pivot in proper position and mark its location
    swap(theArray[first], theArray[lastS1], m);
    pivotIndex = lastS1;
}

