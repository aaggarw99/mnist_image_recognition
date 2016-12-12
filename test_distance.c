#include "distance.h"
#include <CUnit/Basic.h>
#define TEST_NAME "t10k"
#define TRAIN_NAME "train"

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */

// static mnist_dataset_handle my_mdh;
// static mnist_image_handle my_mih;

static int init_suite(void)
{
    // my_mdh = mnist_open(TEST_T10K);
    // my_mih = mnist_image_begin(my_mdh);
    return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
static int clean_suite(void)
{
    // mnist_free(my_mdh);
    return 0;
}

static void test_euclid() {
        mnist_image_handle img1 = NULL;
        mnist_image_handle img2 = NULL;
        CU_ASSERT_EQUAL((euclid(img1, img2)), -1);
        mnist_dataset_handle dts = mnist_open(TRAIN_NAME, 1);
        img1 = mnist_image_begin(dts);
        CU_ASSERT_EQUAL(euclid(img1, img1), 0);
}

static void test_reduced() {
        mnist_image_handle img1 = NULL;
        mnist_image_handle img2 = NULL;
        CU_ASSERT_EQUAL((reduced(img1, img2)), -1);
}

static void test_factory() {
        CU_ASSERT_NOT_EQUAL(create_distance_function("euclid"), NULL);
        CU_ASSERT_NOT_EQUAL(create_distance_function("reduced"), NULL);
        CU_ASSERT_NOT_EQUAL(create_distance_function("downsample"), NULL);
        CU_ASSERT_EQUAL(create_distance_function("invalid"), NULL);
}
static void test_getList() {
        CU_ASSERT_NOT_EQUAL(getList(), NULL);
}




/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
    CU_pSuite pSuite = NULL;
    
    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry()){
        return CU_get_error();
        }    
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
        (NULL == CU_add_test(pSuite, "factory\n", test_factory)) ||
        (NULL == CU_add_test(pSuite, "euclid()\n", test_euclid)) ||
        (NULL == CU_add_test(pSuite, "reduced()\n", test_reduced)) ||
        (NULL == CU_add_test(pSuite, "getList()\n", test_getList))

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

