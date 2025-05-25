#include <gmock/gmock.h>
#include <mpi.h>
#include <mpi_proto.h>
#include "teciocpp/enums.h"
#include "teciocpp/file.h"

using namespace teciocpp;

TEST(FileTest, zone_fe)
{
    MPI_Comm comm = MPI_COMM_WORLD;

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
}

TEST(FileTest, zone_fe_transient)
{
    MPI_Comm comm = MPI_COMM_WORLD;

    int rank;
    MPI_Comm_rank(comm, &rank);

    std::vector<std::string> var_names = { "x", "y", "p" };
    std::vector<double> x = { 0., 0.5, 1., 0., 0.5, 1. };
    std::vector<double> y = { 0., 0., 0., 2., 2., 2. };
    std::vector<std::vector<double>> p = {
        { 0., 0.5, 1., 4., 6.5, 9. },
        { 1., 1.5, 2., 5., 7.5, 10. },
        { 2., 2.5, 3., 6., 8.5, 11. },
    };
    std::vector<int32_t> connectivity = { 1, 2, 5, 4, 2, 3, 6, 5 };
    std::vector<bool> shared_vars = { true, true, false };

    int32_t n_nodes = 6;
    int32_t n_cells = 2;
    std::vector<ValueLocation> value_locs = { ValueLocation::NODAL,
                                              ValueLocation::NODAL,
                                              ValueLocation::NODAL };

    File f(comm);
    f.create("test_fe_trans.szplt", "Title", var_names);

    // time step 1
    auto zn = f.zone_create_fe("", ZoneType::FEQUADRILATERAL, n_nodes, n_cells, value_locs, false);
    f.set_unsteady_option(zn, 0.);
    f.zone_var_write(zn, 1, rank, x);
    f.zone_var_write(zn, 2, rank, y);
    f.zone_var_write(zn, 3, rank, p[0]);
    f.zone_node_map_write(zn, rank, connectivity);
    f.flush({ 1 });

    // time step 2
    zn = f.zone_create_fe("",
                          ZoneType::FEQUADRILATERAL,
                          n_nodes,
                          n_cells,
                          value_locs,
                          true,
                          shared_vars);
    f.set_unsteady_option(zn, 0.25);
    f.zone_var_write(zn, 3, rank, p[1]);
    f.flush({ 1 });

    // time step 3
    zn = f.zone_create_fe("",
                          ZoneType::FEQUADRILATERAL,
                          n_nodes,
                          n_cells,
                          value_locs,
                          true,
                          shared_vars);
    f.set_unsteady_option(zn, 0.45);
    f.zone_var_write(zn, 3, rank, p[2]);
    f.flush({ 1 });

    f.close();
}
