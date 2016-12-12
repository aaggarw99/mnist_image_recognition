#pragma once
#ifndef distance_h
#define distance_h
#include "mnist.h"
#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#define D_TYPES 3


typedef double (*distance_t)(mnist_image_handle image1, mnist_image_handle image2);
struct dist_holder{
    char name[64];
    distance_t fx;
};

typedef struct dist_holder dist_holder;

double euclid(mnist_image_handle image1, mnist_image_handle image2);
double reduced(mnist_image_handle image1, mnist_image_handle image2);
double downsample(mnist_image_handle image1, mnist_image_handle image2);
//double crop(mnist_image_handle image1, mnist_image_handle image2);
//double threshold(mnist_image_handle image1, mnist_image_handle image2);

distance_t create_distance_function(const char * schemename);

// returns the list in an array of dist_holder
struct dist_holder * getList();

// will print the list in a concatanated string
char * printList();


#endif /* distance_h */
