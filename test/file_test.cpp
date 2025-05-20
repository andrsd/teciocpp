#include <gmock/gmock.h>
#include <mpi.h>
#include <mpi_proto.h>
#include "teciocpp/enums.h"
#include "teciocpp/file.h"

using namespace teciocpp;

TEST(FileTest, zone_fe)
{
    MPI_Comm comm = MPI_COMM_WORLD;
    int argc = 1;
    char * argv[] = { const_cast<char *>("test") };
    char ** argv_ptr = argv;
    MPI_Init(&argc, &argv_ptr);

    int rank;
    MPI_Comm_rank(comm, &rank);

    std::vector<std::string> var_names = { "x", "y", "p", "A" };
    std::vector<double> x = { 0., 0.5, 1., 0., 0.5, 1. };
    std::vector<double> y = { 0., 0., 0., 2., 2., 2. };
    std::vector<double> p = { 0., 0.5, 1., 4., 6.5, 9. };
    std::vector<double> A = { 1.5, 3.4 };
    std::vector<int32_t> connectivity = { 1, 2, 5, 4, 2, 3, 6, 5 };

    int32_t n_nodes = 6;
    int32_t n_cells = 2;
    std::vector<ValueLocation> value_locs = { ValueLocation::NODAL,
                                              ValueLocation::NODAL,
                                              ValueLocation::NODAL,
                                              ValueLocation::ELEMENTAL };

    File f(comm);
    f.create("test_fe.szplt", "Title", var_names);

    auto zn = f.zone_create_fe("", ZoneType::FEQUADRILATERAL, n_nodes, n_cells, value_locs);
    f.zone_var_write(zn, 1, rank, x);
    f.zone_var_write(zn, 2, rank, y);
    f.zone_var_write(zn, 3, rank, p);
    f.zone_var_write(zn, 4, rank, A);
    f.zone_node_map_write(zn, rank, connectivity);

    f.close();

    MPI_Finalize();
}
