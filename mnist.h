#pragma once
#ifndef MNIST_H
#define MNIST_H

#include <stdbool.h>
#include <arpa/inet.h>


#define IMAGE_MAG_NUMBER 2051
#define LABEL_MAG_NUMBER 2049
#define labelPath "-labels-idx1-ubyte"
#define imagePath "-images-idx3-ubyte"


// structs defined in header because it is important for users to understand the structure of the dataset and images.
typedef struct mnist_dataset_t { // linked list
    //all labels stored in this pointer
    u_int8_t * labels;
    
    //all images pixels stored in this pointer
    u_int8_t * images;
    struct mnist_image_t * firstImage;
    
}*mnist_dataset_handle;

typedef struct mnist_image_t{ //node
    //offset
    u_int32_t offset;
    // parent
    mnist_dataset_handle parent;
    struct mnist_image_t * next;
}*mnist_image_handle;


#define MNIST_DATASET_INVALID ((mnist_dataset_handle) 0)

/// Define struct mnist_image_t in your mnist.c file


#define MNIST_IMAGE_INVALID ((mnist_image_handle) 0)

/// Open the given MNIST formatted dataset;
/// Returns MNIST_INVALID if the dataset could not be opened,
/// and a mnist_dataset_handle on success.
/// The handle must be freed using mnist_free().
///
/// For dataset 'name', the following files must exist:
///   - name-labels-idx1-ubyte
///   - name-images-idx3-ubyte
/// in other words, to open the dataset represented by the files above,
/// the user would call mnist_open("name")
///
/// The open function checks the 'magic number' at the beginning of the file
/// and returns MNIST_DATASET_INVALID if the number does not match
/// expectations.
mnist_dataset_handle mnist_open (const char * name, double fraction);


/// Create a new empty dataset
/// This only creates the in-memory representation of the dataset.
/// No actual files need to be created.
/// x and y refer to the image size for this dataset.
/// Returns MNIST_DATASET_INVALID if an error occurred.
///
/// In case of success, the resulting handle needs to be freed
/// using mnist_free.

mnist_dataset_handle mnist_create(unsigned int x, unsigned int y);

/// Free the memory associated with the handle

void mnist_free (mnist_dataset_handle handle);

// gets the parent of a given handle. Returns that dataset.
mnist_dataset_handle getParent(mnist_image_handle h);


/// Return the number of images in the dataset
/// Returns <0 if handle is equal to MNIST_DATASET_INVALID
int mnist_image_count (const mnist_dataset_handle handle);

/// Return the size of each image in the dataset; Stores the size in X and Y.
/// Sets *x and *y to 0 if handle == MNIST_DATASET_INVALID
void mnist_image_size (const mnist_dataset_handle handle,
                       unsigned int * x, unsigned int * y);

/// Return a handle to the first image in the dataset;
/// Note: there is no requirement to free the returned handle.
/// If there are no images in the dataset, or if handle ==
/// MNIST_DATASET_INVALID, or any other error occurs,
/// return MNIST_IMAGE_INVALID
mnist_image_handle mnist_image_begin (const mnist_dataset_handle handle);

/// Return a pointer to the data for the image. The data should not be copied
/// and the user of the data should not modify or free it. The return pointer
/// should point to image_size_x * image_size_y bytes, in the same order as
/// specified in the MNIST file format.
/// The pointer must remain valid even after mnist_image_next() is called.
/// However, when mnist_free() is called, all pointers returned by this
/// function become invalid as well.
u_int8_t * mnist_image_data (const mnist_image_handle h);

/// Obtain the label of the image.
/// Return <0 if handle is equal to MNIST_IMAGE_INVALID.
int mnist_image_label (const mnist_image_handle h);

/// Return the handle of the next image in the dataset, or MNIST_IMAGE_INVALID
/// if there are no more images.
mnist_image_handle mnist_image_next (const mnist_image_handle h);

// gets the corresponding label given a dataset and an offset
int getLabelD(const mnist_dataset_handle dts, int offset);



#endif

