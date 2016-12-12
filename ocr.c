#include <stdio.h>
#include <stdlib.h>
#include "distance.h"
#include "mnist.h"
#include "knn.h"
#include "time.h"

#define MAX 255;

time_t curr;


void status(distance_t scheme, int pos, int totalImages, int correct) {
    time_t now;
    time(&now);
    char * name = (char*) malloc(sizeof(char)*64);
    struct dist_holder * li = getList();
    for(int i = 0; i < D_TYPES; i++) {
        if(li[i].fx == scheme){
            strcpy(name, li[i].name);
        }
    }
    if(difftime(now, curr) >= 1) {
        printf("[%s] %d/%d ( %3.2f%%) %d/%d ( %3.2f%%)\n", name, pos,totalImages, (double)pos / (double)totalImages* 100,correct, pos, (double)correct/(double)pos*100);
        curr = now;
    }
}
// if all is passed in for k, dist, and size
// call printf();
void advancedstatus(char * k, char * dist, char * size, mnist_dataset_handle dts, mnist_dataset_handle test) {
    
    dist_holder * li = getList();
    mnist_image_handle f = mnist_image_begin(test);
    int q = 0;
    
    // T T T
    if((strcmp(k, "all") == 0) && (strcmp(dist, "all") == 0) && (strcmp(size, "all"))) {
        printf("#distance k 15000 30000 45000 60000");
        int correct = 0;
        for(double i = 0.25; i <= 1.0; i+=0.25) {
            int count = mnist_image_count(dts)*i;
            for(int l = 0; l <= 20; l+=5) {
                if(l == 0) {
                    l = 0;
                }
                
                for(int j = 0; j < D_TYPES; j++) {
                    printf("%s", li[j].name);
                    for(q = 0; q < count; q++) {
                        int lbl = get_nearest_k(dts, f, li[j].fx, l);
                        if(lbl == mnist_image_label(f)) {
                            correct++;
                        }
                        
                        f = mnist_image_next(f);
                    }
                }
                double a = correct/q;
                printf("%2.2f ", a);
                if(l == 1) {
                    l = 0;
                }
                
                
                
            }
            printf("\n");
            
            
        }
    }
    
}
int main(int argc, char * args[]) {
    // input:
    // train-name train-size test-name k distance-scheme
    // 1 2 3 4 5
    // check for too many or too little arguments
    if(argc > 6 || argc <= 0) {
        goto arg;
    }
    double fraction = 0;
    char * test_name = NULL;
    char * train_name = NULL;
    mnist_dataset_handle dts = NULL;
    if(!(train_name = malloc(sizeof(char)*64))){
        goto mem;
    }
    strcpy(train_name, args[1]);
    
    
     dts = mnist_open(train_name, 1);
    if(dts == MNIST_DATASET_INVALID){
        goto invalidDataset;
    }
    int total = mnist_image_count(dts);
    
    // check train-size
    if(strcmp(args[2], "all") == 0) {
        fraction = 1;
    }else if(atoi(args[2]) <= 0 || atoi(args[2]) > total) {
        goto std;
    }else {
        goto std;
    }
    
    if(!(test_name = (char*) malloc(sizeof(char)*64))){
        goto mem;
    }
    free(dts);
    strcpy(test_name, args[3]);
    dts = (mnist_dataset_handle) malloc(sizeof(struct mnist_dataset_t));
    dts = mnist_open(train_name, fraction);

    // check k validity
    if(strtol(args[4], NULL, 10) == 0) {
        goto std;
    }
    if(atoi(args[4]) > total || atoi(args[4]) <= 0) {
        goto std;
    }
    int k = atoi(args[4]);
    
    // check if distance scheme is valid
    distance_t temp = create_distance_function(args[5]);
    if(!temp) {
        goto std;
    }
    distance_t scheme = temp;
    
    // opens test dataset
    mnist_dataset_handle testdts = (mnist_dataset_handle) malloc(sizeof(struct mnist_dataset_t));
    testdts = mnist_open(test_name, 1);
    if(testdts == MNIST_DATASET_INVALID){
        goto invalidDataset;
    }

    
    mnist_image_handle node = (mnist_image_handle) malloc(sizeof(mnist_image_handle));
    node = mnist_image_begin(testdts);
    
    int correct = 0;
    int count = mnist_image_count(testdts)*fraction;
    for(int x = 1; x < count; x++) {
        node = mnist_image_next(node);
        if(node == MNIST_IMAGE_INVALID) {
            break;
        }
        int lbl = get_nearest_k(dts, node, scheme, k);
        // accuracy check here, stored in a
        if(lbl == mnist_image_label(node)) {
            correct++;
        }
        status(scheme, x, fraction*mnist_image_count(dts), correct);
    }
    
    return EXIT_SUCCESS;
std:
    printf("Usage: ./ocr %s %s %s %s %s\nThe following schemes are supported: ", args[1], args[2], args[3], args[4], args[5]);
    printf("%s", printList());
    if(dts != NULL) {
        free(dts);
    }
    if(train_name != NULL){
        free(train_name);
    }
    if(test_name != NULL) {
        free(test_name);
    }
    return EXIT_FAILURE;
arg:
    printf("Incorrect number of arguments!\n");
    return EXIT_FAILURE;

invalidDataset:
    printf("Invalid Dataset!");
    if(dts != NULL) {
        free(dts);
    }
    if(train_name != NULL){
        free(train_name);
    }
    if(test_name != NULL) {
        free(test_name);
    }
    return EXIT_FAILURE;
    
mem:
    printf("Not enough memory!");
    if(dts != NULL) {
        free(dts);
    }
    if(train_name != NULL){
        free(train_name);
    }
    if(test_name != NULL) {
        free(test_name);
    }
    return EXIT_FAILURE;
}

