#include "distance.h"
#include "mnist.h"
#include <stdlib.h>
#include <string.h>
#include "math.h"

struct dist_holder * l = NULL;

struct dist_holder * getList() {
    if(l != NULL) {
        return l;
    }
    l = malloc(sizeof(struct dist_holder)*D_TYPES);
    strcpy(l[0].name, "euclid");
    l[0].fx = &euclid;
    strcpy(l[1].name, "reduced");
    l[1].fx = &reduced;
    strcpy(l[2].name, "downsample");
    l[2].fx = &downsample;
    return l;
}
char * printList() {
    struct dist_holder *  d = getList();
    char * temp = (char *) malloc(sizeof(char)*100);
    for(int i = 0; i < D_TYPES; i++) {
        strcat(temp, d[i].name);
        strcat(temp, " ");
    }
    return temp;
}


distance_t create_distance_function(const char * schemename) {
    
    // fills an array of dist_holder's with all possible distance formulas
    struct dist_holder * t = getList();
    // factory function begins here
    for(int i = 0; i < D_TYPES; i++) {
        if(strcmp(schemename,t[i].name) == 0) {
            return t[i].fx;
        }
    }
    return NULL;
}

double euclid(mnist_image_handle image1, mnist_image_handle image2){
    if(!image1 || !image2) {
        return -1;
    }
    
    long double sum = 0;
    unsigned int x1 = 1;
    unsigned int y1 = 1;
    unsigned int x2 = 1;
    unsigned int y2 = 1;
    // gives me dimensions of given image stored in x and y
    mnist_image_size(getParent(image1), &x1, &y1);
    mnist_image_size(getParent(image1), &x2, &y2);

    if(x1 != x2 || y1 != y2) {
        return -1;
    }
    for(int i = 0; i < x1*y1; i++) {
        // multiplying by itself to simulate pow. pow() is generally used for much larger numbers.
        int diff = (int) *(mnist_image_data(image1)+i) - (int) *(mnist_image_data(image2)+i);
        int square = diff * diff;
        sum += square;
    }
    return sqrt(sum);
}

double reduced(mnist_image_handle image1, mnist_image_handle image2) {
    if(!image1 || !image2) {
        return -1;
    }
    int sum1 = 0;
    int sum2 = 0;
    unsigned int x1 = 1;
    unsigned int y1 = 1;
    unsigned int x2 = 1;
    unsigned int y2 = 1;
    // gives me dimensions of given image stored in x and y
    mnist_image_size(getParent(image1), &x1, &y1);
    mnist_image_size(getParent(image1), &x2, &y2);
    if(x1 != x2 || y1 != y2) {
        return -1;
    }
    // sum of image 1 pixels
    for(int i = 0; i < x1*y1; i++) {
        sum1+= (int) *(mnist_image_data(image1)+i);
    }
    for(int i = 0; i < x1*y1; i++) {
        sum2 += (int) *(mnist_image_data(image2)+i);
    }
    return abs(sum1-sum2);
    
}
double downsample(mnist_image_handle image1, mnist_image_handle image2) {
    if(!image1 || !image2) {
        return -1;
    }
    
    double sum = 0;
    unsigned int x1 = 1;
    unsigned int y1 = 1;
    unsigned int x2 = 1;
    unsigned int y2 = 1;
    // gives me dimensions of given image stored in x and y
    mnist_image_size(getParent(image1), &x1, &y1);
    mnist_image_size(getParent(image1), &x2, &y2);
    if(x1 != x2 || y1 != y2) {
        return -1;
    }
    for(int i = 0; i < x1*y1; i+=2) {
        // multiplying by itself to simulate pow. pow() is generally used for much larger numbers.
        int diff = (int) *(mnist_image_data(image1)+i) - (int) *(mnist_image_data(image2)+i);
        int square = diff * diff;
        sum += square;
    }
    return sqrt(sum);
}

