#include <gmock/gmock.h>
#include <mpi.h>
#include "teciocpp/file.h"

using namespace teciocpp;

TEST(FileTest, empty)
{
    MPI_Comm comm = MPI_COMM_WORLD;
    int argc = 1;
    char * argv[] = { const_cast<char *>("test") };
    char ** argv_ptr = argv;
    MPI_Init(&argc, &argv_ptr);

    File f(comm);
    f.set_title("Title");
    f.create("test.szplt");
    f.close();

    MPI_Finalize();
}
