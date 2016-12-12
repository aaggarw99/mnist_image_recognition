#include "mnist.h"
#include "distance.h"
#include <unistd.h>

// adapted and modified from tutorialspoint.com
int compare_function(const void * a, const void * b) {
    
    int x = *((int*)a);
    int y = *((int*)b);
    return x-y;
     
}
// returns -1 on error
int get_nearest_k(mnist_dataset_handle dts,mnist_image_handle img, distance_t distanceFunction, int k) {
    if(dts == getParent(img)) {
        return -1;
    }
    
    if(k <= 0) {
        return -1;
    }
    if(dts == NULL) {
        return -1;
    }
    if(!img) {
        return -1;
    }

    if(!distanceFunction) {
        return -1;
    }
    // gets the first image of the training set
    mnist_image_handle f = mnist_image_begin(dts);
    // creates a double array that will store all the raw values calculated by the distance function
    double * raw_scores = (double *) malloc(mnist_image_count(dts)*sizeof(double));
    int i = 0;
    // fills raw_scores and gets the total count, i, of how many images there are.
    while(true) {
        *(raw_scores+i) = distanceFunction(f, img);
        f = mnist_image_next(f);
        if(f == NULL) {
            break;
        }
        i++;
    }
    int totalImages = i;
    // cornercase
    if(k>totalImages) {
        return -1;
    }
    // this array holds the indicies for the k-lowest scores. In other words, the offsets for the k-lowest scores
    int * indicies = (int*) malloc(sizeof(int)*k);
    double biggestNumber =0;
    // finds biggest number in the raw_scores
    for(int count = 0; count < totalImages; count++) {
        if(raw_scores[count] > biggestNumber) {
            biggestNumber = raw_scores[count];
        }
    }
    // adds one to biggest number so we know no value will exceed it in the scores
    biggestNumber++;
    
    bool found = false;
    double lowest = biggestNumber;
    int m = 0;
    // runs k times
    for(int j = 0; j < k; j++) {
        // goes through the dataset image_count times
        lowest = biggestNumber;
        found = false;
        for(m = 0; m < totalImages; m++) {
            if((double) *(raw_scores+m) <= lowest) {
                // ensures no duplicates
                for(int a = 0; a < j; a++) {
                    if(*(indicies+a) == m) {
                        found = true;
                        break;
                    }
                }
                if(found == false) {
                    *(indicies+j) = m;
                    lowest = (double) *(raw_scores+m);
                }
            }
        }
    }
    // contains the best labels
    int * best_labels = (int*) malloc(sizeof(int)*k);
    for(int i = 0; i < k; i++) {
        *(best_labels+i) = getLabelD(dts, indicies[i]);
    }
    
    // sort best labels. This will make choosing a majority easier
    qsort(best_labels, k, sizeof(int), compare_function);
    
    // adapted from stackoverflow.com
    // finds most popular element in a sorted array
    int count = 0;
    int tempCount = 0;
    int temp = 0;
    int popular = *(best_labels);
    for(int i = 0; i < k; i++) { // goes through entire array with runtime complexity O(n)
        temp = *(best_labels+i); // sets temp to every item once in the array
        tempCount = 0; // tells us the number of occurances of temp
        for(int u = i+1; u < k; u++) { // // checks the array again for all duplicate labels
            if(temp == *(best_labels+u)) {
                tempCount++;
            }
        }
        // if tempCount is bigger than count, set popular to tempCount
        if(tempCount > count) {
            popular = temp;
            count = tempCount;
        }
    }
    
    return popular;
}

