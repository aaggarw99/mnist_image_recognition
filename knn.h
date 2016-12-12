#pragma once
#ifndef knn_h
#define knn_h

#include "knn.h"
#include "distance.h"
#include "mnist.h"
#include <stdlib.h>
#include <stdio.h>


// returns the best label given a test image
int get_nearest_k(mnist_dataset_handle dts,mnist_image_handle img, distance_t distanceFunction, int k);


#endif /* knn_h */
