###############################################
MNIST MACHINE LEARNING IMPLEMENTATION
###############################################

Full information regarding the mnist library can be found here:
http://yann.lecun.com/exdb/mnist/

Published 12/9/16, Ashwin Aggarwal

This program can be built by simply typing "make" in the command line. Ensure that the CUnit library is installed on your machine in order for the test_ files to work.

mnist.c, mnist.h
	The mnist library implementation exits within these two files. Essentially, all values of a given training dataset are stored within the program and later used to determine \
	a corresponding label. The mnist.h contains prototypes for functions used in this library. mnist.c contains all implementations.


distance.c, distance.h
        Currently there are 3 distance formulas available: euclid, reduced, downsample. In order to make manipulation of this list easier, if a user adds a distance function \
        , they only have to increment the macro D_TYPES to however many distances there are, and add the distance function to the getList() function provided in the .c file.
	Euclidian distance offers the best accuracy compared to the other two.

knn.c, knn.h
        Logic behind algorithm used,  k-nearest-neighbors, to find best labels.

        - Calculate the distance between two images by comparing each individual pixel. Using the given distance function, the algorithm compars every training image with a singular test image. This was stored in raw_scores[] and maintained index arragement.
        - Go through the raw_scores[] array and find the indicies corresponding to the k-lowest scores. These indicies, representing the offset in the entire dataset, were stored in \
        indicies[].
        - Since these are the best possible scores, I stored the corresponding label value for each item in indecies[] into an array called best_labels[]. These had the values 0-9 stored in them \
        - Used qsort() provided by stdio.h to sort the best_labels[] because finding the majority element is easier when all items are stored
        - Using a simple "most popular" algorithm, the  most popular element in best_labels[] was returned. If there was no dominant number, a random number between 0-9 was returned. This is very \
	rare though. 

ocr.c
        Command Line implementation was not fully done -- this current implementation cannot take test_size values. Nor does it work with all possible combinations of k, test-size, and distance.\
        In a future model, all command line optimizations would be completed. There is a function called "advancedstatus" which works under the first combination of the 8 possible combinations with k,\
        test-size, and distance.

        Values Supported v1:
        - Any k
        - Any of the listed distances
        - "all" for test-size
	
	The use of the ocr application goes as follows: ./ocr [train-name] [train-size] [test-name] [k] [distance-scheme]
	where [train-name] is the name of the training dataset binary file; where [train-size] are the amount of items you want to search in a given training dataset; where [test-name] is \
	the testing dataset where the label value is unknown; where [k] is the value of how many nearest neighbors to compare to. The accuracy of this implementation depends on the size of k and \
	the size of the training dataset; where distance-scheme is one of the aforementioned distance calculation methods that will be used when computing the label.

        There is also a status() function that will print out the current status every second.



