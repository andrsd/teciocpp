#include "gtest/gtest.h"
#include <mpi.h>

int
main(int argc, char ** argv)
{
    MPI_Init(&argc, &argv);

    ::testing::InitGoogleTest(&argc, argv);
    testing::GTEST_FLAG(death_test_style) = "threadsafe";
    auto ret = RUN_ALL_TESTS();

    MPI_Finalize();

    return ret;
}
