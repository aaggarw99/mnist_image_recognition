###############################################
MNIST MACHINE LEARNING IMPLEMENTATION
##############################################

Published 12/9/16, Ashwin Aggarwal

mnist.c, mnist.h
        the biggest chance to mnist.c was the addition of the "fraction" parameter which would allow manipulation of the dataset size straight from reading it \
        there is also a fill_dataset() function that reads in the entire data from the binary files and stores them into the respective dataset

distance.c, distance.h
        Currently there are 3 distance formulas available: euclid, reduced, downsample. In order to make manipulation of this list easier, if a user adds a distance function \
        , they only have to increment the macro D_TYPES to however many distances there are, and add the distance function to the getList() function provided in the .c file.

knn.c, knn.h
        Logic behind algorithm used,  k-nearest-neighbors, to find best labels.

        - Calculate each score, using the given distance function, of every training image with a singular test image. This was stored in raw_scores[] and maintained index arragement.
        - Go through the raw_scores[] array and find the indicies corresponding to the k-lowest scores. These indicies, representing the offset in the entire dataset, were stored in \
        indicies[].
        - Since these are the best possible scores, I stored the corresponding label value for each item in indecies[] into an array called best_labels[]. These had the values 0-9 stored in them \
        - Used qsort() provided by stdio.h to sort the best_labels[] because finding the majority element is easier when all items are stored
        - Adapting an algorithm from stackoverflow.com, I found the most popular element in best_labels[], if there was no dominance, it chose a random value, then returned popular

ocr.c
        Command Line implementation was not fully done -- this current implementation cannot take test_size values. Nor does it work with all possible combinations of k, test-size, and distance.\
        In a future model, all command line optimizations would be completed. There is a function called "advancedstatus" which works under the first combination of the 8 possible combinations with k,\
        test-size, and distance.

        Values Supported v1:
        - Any k
        - Any of the listed distances
        - "all" for test-size


        There is also a status() function that will print out the current status every second.



