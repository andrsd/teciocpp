// SPDX-FileCopyrightText: 2025 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "enums.h"
#include <vector>
#include <string>
#include <mpi.h>

namespace teciocpp {

/// Tecplot file
class File {
public:
    /// Tecplot file format
    enum class Format {
        /// PLT file
        PLT,
        /// SZPLT file
        SZPLT
    };

    /// Tecplot file type
    enum class Type {
        ///
        FULL,
        GRID,
        SOLUTION
    };

    /// Create an instance of file (no I/O is done)
    ///
    /// @param comm MPI communicator
    /// @param format File format
    /// @param type File type
    File(MPI_Comm comm, Format format = Format::SZPLT, Type type = Type::FULL);

    /// Set if nodes are one-based ot not
    ///
    /// @param state `true` to set one-based indices, `false` for zero-based ones.
    void set_one_based(bool state);

    /// Create a new file
    ///
    /// @param file_name File name
    /// @param title Dataset title
    /// @param var_names Variable names
    void create(const std::string & file_name,
                const std::string & title,
                const std::vector<std::string> & var_names);

    /// Create FE zone
    ///
    /// @param title Zone title
    /// @param type Zone type
    /// @param n_nodes Number of nodes
    /// @param n_cells Number of cells
    /// @param value_locations Location of variable values
    int32_t zone_create_fe(const std::string & title,
                           ZoneType type,
                           int64_t n_nodes,
                           int64_t n_cells,
                           const std::vector<ValueLocation> & value_locations);

    /// Create FE zone
    ///
    /// @param title Zone title
    /// @param type Zone type
    /// @param n_nodes Number of nodes
    /// @param n_cells Number of cells
    /// @param value_locations Location of variable values
    /// @param shared_connectivity Whether the connectivity is shared
    int32_t zone_create_fe(const std::string & zone_title,
                           ZoneType type,
                           int64_t n_nodes,
                           int64_t n_cells,
                           const std::vector<ValueLocation> & value_locations,
                           bool shared_connectivity);

    /// Create FE zone with shared variables
    ///
    /// @param title Zone title
    /// @param type Zone type
    /// @param n_nodes Number of nodes
    /// @param n_cells Number of cells
    /// @param value_locations Location of variable values
    /// @param shared_connectivity Whether the connectivity is shared
    /// @param shared_vars Whether the variables are shared
    int32_t zone_create_fe(const std::string & zone_title,
                           ZoneType type,
                           int64_t n_nodes,
                           int64_t n_cells,
                           const std::vector<ValueLocation> & value_locations,
                           bool shared_connectivity,
                           const std::vector<bool> & shared_vars);

    int32_t zone_create_fe_mixed(const std::string & zone_title,
                                 int64_t n_nodes,
                                 const std::vector<int32_t> & cell_shapes,
                                 const std::vector<int32_t> & grid_orders,
                                 const std::vector<int32_t> & basis_fns,
                                 const std::vector<int64_t> & n_elems,
                                 const std::vector<ValueLocation> & value_locations);

    /// Write variable values into a zone
    ///
    /// @param zone Zone ID (obtained by `zone_create_XYZ`)
    /// @param var Variable index (1-based)
    /// @param partition MPI rank
    /// @param vals Values to write
    void
    zone_var_write(int32_t zone, int32_t var, int32_t partition, const std::vector<double> & vals);

    /// Write variable values into a zone
    ///
    /// @param zone Zone ID (obtained by `zone_create_XYZ`)
    /// @param var Variable index (1-based)
    /// @param partition MPI rank
    /// @param vals Values to write
    void
    zone_var_write(int32_t zone, int32_t var, int32_t partition, const std::vector<float> & vals);

    /// Write zone connectivity
    ///
    /// @param zone Zone ID (obtained by `zone_create_XYZ`)
    /// @param partition MPI rank
    /// @param connectivity Connectivity array
    void
    zone_node_map_write(int32_t zone, int32_t partition, const std::vector<int32_t> & connectivity);

    /// Write zone connectivity
    ///
    /// @param zone Zone ID (obtained by `zone_create_XYZ`)
    /// @param partition MPI rank
    /// @param connectivity Connectivity array
    void
    zone_node_map_write(int32_t zone, int32_t partition, const std::vector<int64_t> & connectivity);

    /// Set unsteady option
    ///
    /// @param zone Zone ID
    /// @param time Solution time
    /// @param strand Strand ID
    void set_unsteady_option(int32_t zone, double time, int32_t strand = 1);

    /// Add auxiliary data
    ///
    /// @param zone Zone ID
    /// @param name Name of the auxiliary data
    /// @param value Value of the auxiliary data
    void add_aux_data(int32_t zone, const std::string & name, const std::string & value);

    ///
    void flush(const std::vector<int32_t> & zones_to_retain);

    /// Close the file
    void close();

private:
    /// MPI communicator
    MPI_Comm comm_;
    /// File format
    Format file_format_;
    /// File type
    Type file_type_;
    /// Dataset title
    std::string title_;
    /// Variable names
    std::vector<std::string> var_names_;
    /// File name
    std::string file_name_;
    /// file handle
    void * handle_;
    /// Are indices one-based or not
    bool one_based_;
};

} // namespace teciocpp
