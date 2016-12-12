#include "knn.h"
#include <CUnit/Basic.h>

#define TRAIN_NAME "train"
#define TEST_NAME "t10k"
static int init_suite(void)
{
        return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
static int clean_suite(void)
{
        return 0;
}
static void test_get_nearest_k() {
        mnist_dataset_handle dts = mnist_open(TRAIN_NAME, 1);
        mnist_dataset_handle testDTS = mnist_open(TEST_NAME, 1);
        mnist_image_handle firstImageTest = mnist_image_begin(testDTS);
        distance_t e = create_distance_function("euclid");
        int k = 5;
        CU_ASSERT_NOT_EQUAL(get_nearest_k(dts, firstImageTest, e, k), -1);

        k = 61000;
        CU_ASSERT_EQUAL(get_nearest_k(dts, firstImageTest, e, k), -1);
        
        k = 0;
        CU_ASSERT_EQUAL(get_nearest_k(dts, firstImageTest, e, k), -1);
        
        k = -5;
        CU_ASSERT_EQUAL(get_nearest_k(dts, firstImageTest, e, k), -1);
        
        e = NULL;
        k = 5;
        CU_ASSERT_EQUAL(get_nearest_k(dts, firstImageTest, e, k), -1);
        e = create_distance_function("euclid");
        dts = NULL;
        CU_ASSERT_EQUAL(get_nearest_k(dts, firstImageTest, e, k), -1);

        e = create_distance_function("blah");
        dts = mnist_open(TRAIN_NAME, 1);
        CU_ASSERT_EQUAL(get_nearest_k(dts, firstImageTest, e, k), -1);

        e = create_distance_function("reduced");
        CU_ASSERT_NOT_EQUAL(get_nearest_k(dts, firstImageTest, e, k), -1);

        firstImageTest = MNIST_IMAGE_INVALID;
        CU_ASSERT_EQUAL(get_nearest_k(dts, firstImageTest, e, k), -1);
        
        // ensure two images from the same dataset aren't being tested
        firstImageTest = mnist_image_begin(dts);
        CU_ASSERT_EQUAL(get_nearest_k(dts, firstImageTest, e, k), -1);
}



int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite, clean_suite);
   if (NULL == pSuite)
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if (
           (NULL == CU_add_test(pSuite, "get_nearest_k()\n", test_get_nearest_k))
      )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}



