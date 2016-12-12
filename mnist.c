#include "mnist.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>


void fill_dataset(mnist_dataset_handle dts, u_int8_t * imageHolder, u_int8_t * labelHolder, double fraction){
    
    dts->images = imageHolder;
    dts->labels = labelHolder;
    
    int count = fraction*mnist_image_count(dts);
    unsigned int x=0;
    unsigned int y = 0;
    mnist_image_size(dts, &x, &y);
    
    if(count<=0 || x<=0 || y<=0) {
        dts->firstImage = MNIST_IMAGE_INVALID;
        return;
    }
    
    // give head value
    mnist_image_handle first = (mnist_image_handle) malloc(sizeof(struct mnist_image_t));
    first->offset = 0;
    first->parent = dts;
    first->next = MNIST_IMAGE_INVALID;
    dts->firstImage = first;
    
    // fill every image
    mnist_image_handle previous = NULL;
    for(int i = 1; i < count; i++) {
        
        previous = first;
        first = (mnist_image_handle) malloc(sizeof(struct mnist_image_t));
        first->offset = i;
        //printf("offset: %d\n", first->offset);
        first->next = MNIST_IMAGE_INVALID;
        first->parent = dts;
        previous->next = first;
    }
}

mnist_dataset_handle mnist_open(const char * name, double fraction) {
    FILE * imageFile;
    FILE * labelFile;
    char * root = (char *) malloc(sizeof(char) * 64);
    strcpy(root, name);
    strcat(root, imagePath);
    
    char * root2 = (char *) malloc(sizeof(char) * 64);
    strcpy(root2, name);
    strcat(root2, labelPath);
    
    if( ((imageFile = fopen(root, "rb")) != NULL) && ((labelFile = fopen(root2, "rb")) != NULL) ) {
        size_t lengthOfImageFile = 0;
        fseek(imageFile, 0, SEEK_END);
        lengthOfImageFile = ftell(imageFile);
        rewind(imageFile);
        
        size_t lengthOfLabelFile = 0;
        fseek(labelFile, 0, SEEK_END);
        lengthOfLabelFile = ftell(labelFile);
        rewind(labelFile);
        
        lengthOfImageFile = (lengthOfImageFile-16) * fraction;
        lengthOfLabelFile = (lengthOfLabelFile-8) * fraction;
        lengthOfImageFile += 16;
        lengthOfLabelFile += 8;
        if(!lengthOfImageFile || !lengthOfLabelFile) {
            return MNIST_DATASET_INVALID;
        }
        // make dummy image and label pointers
        int l1 = (int) (lengthOfImageFile*sizeof(u_int8_t));
        u_int8_t * imageTemp = (uint8_t *)malloc(l1);
        int l2 = (int) (lengthOfLabelFile*sizeof(uint8_t));
        u_int8_t * labelTemp = (uint8_t *)malloc(l2);        


        if(!imageTemp || !labelTemp) {
            return MNIST_DATASET_INVALID;
        }
        size_t a = fread(imageTemp, sizeof(u_int8_t), lengthOfImageFile, imageFile);
        if(a == 0){
            free(imageTemp);
            free(labelTemp);
            fclose(imageFile);
            fclose(labelFile);
            free(root);
            free(root2);
            return MNIST_DATASET_INVALID;
        }
        size_t b = fread(labelTemp, sizeof(u_int8_t), lengthOfLabelFile, labelFile);
        if(b == 0){
            free(imageTemp);
            free(labelTemp);
            fclose(imageFile);
            fclose(labelFile);
            free(root);
            free(root2);
            return MNIST_DATASET_INVALID;
        }
        u_int32_t magI = 0;
        u_int32_t magL = 0;
        magI = (u_int32_t) ((u_int32_t*) imageTemp)[0];
        magL = (u_int32_t) ((u_int32_t*) labelTemp)[0];
        magI = ntohl(magI);
        magL = ntohl(magL);
        // ensures the image and label files match up
        if(magI != IMAGE_MAG_NUMBER && magL != LABEL_MAG_NUMBER) {
            return MNIST_DATASET_INVALID;
        }
        mnist_dataset_handle head = (mnist_dataset_handle) malloc(sizeof(struct mnist_dataset_t));

        
        fill_dataset(head, imageTemp, labelTemp, fraction);

        free(root);
        free(root2);
        fclose(imageFile);
        fclose(labelFile);
        return head;
    }else {
        printf("That doesn't exist!");
        return MNIST_DATASET_INVALID;
    }
    
    
}

mnist_dataset_handle mnist_create(unsigned int x, unsigned int y){
    // preliminary checks
    if(x == 0 || y == 0) {
        return MNIST_DATASET_INVALID;
    }
    // new dataset
    mnist_dataset_handle dts = (mnist_dataset_handle) malloc(sizeof(mnist_dataset_handle));
    // create temp image and label holders
    u_int32_t * imageHolder = (u_int32_t*) malloc(16*sizeof(u_int8_t));
    u_int32_t * labelHolder = (u_int32_t*) malloc(8*sizeof(u_int8_t));
    if(!imageHolder || !labelHolder) {
        free(imageHolder);
        free(labelHolder);
        return MNIST_DATASET_INVALID;
    }
    labelHolder[0] = htonl(LABEL_MAG_NUMBER);
    labelHolder[1] = htonl(0);
    
    imageHolder[0] = htonl(IMAGE_MAG_NUMBER);
    imageHolder[1] = htonl(0);
    imageHolder[2] = htonl(x);
    imageHolder[3] = htonl(y);
    
    // fill
    
    fill_dataset(dts, (u_int8_t*)imageHolder, (u_int8_t*)labelHolder, 1);
    return dts;
}

void mnist_free (mnist_dataset_handle handle){
    if(handle != NULL) {
        mnist_image_handle tempHead = handle->firstImage;
        mnist_image_handle previous = MNIST_IMAGE_INVALID;
        
        while(tempHead!=MNIST_IMAGE_INVALID) {
            previous = tempHead;
            tempHead = previous->next;
            free(previous);
        }
        
        free(handle->images);
        free(handle->labels);
        free(handle);
        
        
    }
}
int mnist_image_count (const mnist_dataset_handle handle){
    if(handle == MNIST_DATASET_INVALID) {
        return -1;
    }
    u_int32_t count = ntohl( ((u_int32_t*)(handle->labels))[1]);
    return count;
}

void mnist_image_size (const mnist_dataset_handle handle, unsigned int * x, unsigned int * y){


    if(handle == MNIST_DATASET_INVALID ){
        *x = 0;
        *y = 0;
    }else {
        *x = (unsigned int) ntohl(((u_int32_t*)(handle->images))[2]);
        *y = (unsigned int) ntohl(((u_int32_t*)(handle->images))[3]);
    }
}

mnist_image_handle mnist_image_begin (const mnist_dataset_handle handle){
    if(handle == MNIST_DATASET_INVALID){
        return MNIST_IMAGE_INVALID;
    }
    if(handle->firstImage == NULL) {
        return MNIST_IMAGE_INVALID;
    }
    if(mnist_image_count(handle) <= 0) {
        return MNIST_IMAGE_INVALID;
    }
    return handle->firstImage;
}
u_int8_t * mnist_image_data (const mnist_image_handle h) {
    if(h == MNIST_IMAGE_INVALID) {
        return NULL;
    }
    
    u_int8_t * imageHolder = h->parent->images;
    
    u_int32_t offset = h->offset;
    
    unsigned int x = 0;
    unsigned int y = 0;
    u_int32_t size = 0;
    mnist_image_size(h->parent, &x, &y);
    size = x*y;
    u_int8_t * image_info = (imageHolder)+16+offset*size;
    return image_info;
}
int mnist_image_label (const mnist_image_handle h){
    if(h == MNIST_IMAGE_INVALID){
        return -1;
    }
    
    u_int8_t * labelHolder = h->parent->labels;
    
    //u_int8_t offset = h->offset;
    
    return (int) *(labelHolder+h->offset+8);
    
}

int getLabelD(const mnist_dataset_handle dts, int offset) {
    if(dts == MNIST_DATASET_INVALID) {
        return -1;
    }
    
    u_int8_t * labelHolder = dts->labels;
    return (int) *(labelHolder+offset+8);
}
mnist_image_handle mnist_image_next (const mnist_image_handle h){
    if(h == MNIST_IMAGE_INVALID) {
        return MNIST_IMAGE_INVALID;
    }
    if(h->next != NULL) {
        return h->next;
    }else {
        return MNIST_IMAGE_INVALID;
    }
}


mnist_dataset_handle getParent(mnist_image_handle h){
    if(h == NULL) {
        return MNIST_DATASET_INVALID;
    }

    return h->parent;
}


