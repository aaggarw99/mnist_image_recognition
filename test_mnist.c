#include "mnist.h"
#include <CUnit/Basic.h>
#define TEST_T10K "t10k"
#define TEST_TRAIN "train"


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

static void test_mnist_open()
{
        mnist_dataset_handle dts = NULL;
        CU_ASSERT_NOT_EQUAL((dts=mnist_open(TEST_T10K, 1)), MNIST_DATASET_INVALID);
        mnist_free(dts);
        CU_ASSERT_NOT_EQUAL((dts=mnist_open(TEST_TRAIN, 1)), MNIST_DATASET_INVALID);
        mnist_free(dts);
        CU_ASSERT_EQUAL((dts=mnist_open("wrong", 1)), MNIST_DATASET_INVALID);
        mnist_free(dts);
        CU_ASSERT_EQUAL((dts=mnist_open(TEST_TRAIN, -4)), MNIST_DATASET_INVALID);
}

static void test_mnist_create()
{
        mnist_dataset_handle mdh = NULL;
        CU_ASSERT_NOT_EQUAL((mdh=mnist_create(32,32)),MNIST_DATASET_INVALID);
        mnist_free(mdh);
        CU_ASSERT_EQUAL((mdh=mnist_create(0,32)),MNIST_DATASET_INVALID);
        CU_ASSERT_EQUAL((mdh=mnist_create(45, 0)), MNIST_DATASET_INVALID);
}

static void test_mnist_image_count()
{
        mnist_dataset_handle dts = NULL;
        dts=mnist_open(TEST_TRAIN, 1);
        CU_ASSERT_EQUAL(mnist_image_count(dts), 60000);
        mnist_free(dts);

        dts=mnist_create(50,50);
        CU_ASSERT_EQUAL(mnist_image_count(dts), 0);
        mnist_free(dts);

        dts=MNIST_DATASET_INVALID;
        CU_ASSERT_TRUE(mnist_image_count(dts)<0);
        mnist_free(dts);

}
static void test_mnist_image_size()
{
        mnist_dataset_handle dts = NULL;
        unsigned int x = 1;
        unsigned int y = 1;
        mnist_image_size(dts, &x, &y);
        CU_ASSERT_EQUAL(x, 0);
        CU_ASSERT_EQUAL(y, 0);

        dts = mnist_create(50, 50);
        mnist_image_size(dts, &x, &y);
        CU_ASSERT_EQUAL(x, 50);
        CU_ASSERT_EQUAL(y, 50);
        mnist_free(dts);
        // train
        dts = mnist_open(TEST_TRAIN, 1);
        mnist_image_size(dts, &x, &y);
        CU_ASSERT_EQUAL(x, 28);
        CU_ASSERT_EQUAL(y, 28);
        mnist_free(dts);
        dts=MNIST_DATASET_INVALID;
        mnist_image_size(dts, &x, &y);
        CU_ASSERT_EQUAL(x, 0);
        CU_ASSERT_EQUAL(y, 0);
        mnist_free(dts);
}
static void test_mnist_image_begin()
{
        mnist_dataset_handle dts = MNIST_DATASET_INVALID;
        CU_ASSERT_EQUAL(mnist_image_begin(dts), MNIST_DATASET_INVALID);
        mnist_image_handle f = MNIST_IMAGE_INVALID;
        dts = mnist_open(TEST_TRAIN, 1);
        CU_ASSERT_NOT_EQUAL((f=mnist_image_begin(dts)),MNIST_IMAGE_INVALID);
        mnist_free(dts);
}
static void test_mnist_image_data()
{
        mnist_dataset_handle dts = mnist_open(TEST_TRAIN, 1);
        mnist_image_handle f = mnist_image_begin(dts);
        const unsigned char * i = mnist_image_data(f);
        CU_ASSERT_NOT_EQUAL(i, NULL);
        // regular
        f = MNIST_IMAGE_INVALID;
        CU_ASSERT_EQUAL(mnist_image_data(f), NULL);
        mnist_free(dts);
}
static void test_mnist_image_label()
{
        mnist_dataset_handle dts = mnist_open(TEST_TRAIN, 1);
        mnist_image_handle f = mnist_image_begin(dts);
        CU_ASSERT_EQUAL(mnist_image_label(f), 5);
        CU_ASSERT_EQUAL(mnist_image_label(MNIST_IMAGE_INVALID),-1);
        mnist_free(dts);
}
static void test_mnist_image_next()
{
        mnist_dataset_handle dts = mnist_open(TEST_TRAIN, 1);
        mnist_image_handle f = mnist_image_begin(dts);
        CU_ASSERT_NOT_EQUAL(mnist_image_next(f), MNIST_IMAGE_INVALID);
        mnist_free(dts);

}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
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
       (NULL == CU_add_test(pSuite, "mnist_open()\n", test_mnist_open))
       || (NULL == CU_add_test(pSuite, "mnist_create()\n", test_mnist_create))
           || (NULL == CU_add_test(pSuite, "mnist_image_count()\n", test_mnist_image_count))
           || (NULL == CU_add_test(pSuite, "mnist_image_size()\n", test_mnist_image_size))
           || (NULL == CU_add_test(pSuite, "mnist_image_begin()\n", test_mnist_image_begin))
           || (NULL == CU_add_test(pSuite, "mnist_image_data()\n", test_mnist_image_data))
           || (NULL == CU_add_test(pSuite, "mnist_image_label()\n", test_mnist_image_label))
           || (NULL == CU_add_test(pSuite, "mnist_image_next()\n", test_mnist_image_next))
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



