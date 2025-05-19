// SPDX-FileCopyrightText: 2025 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

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

    /// Create a file
    ///
    /// @param comm MPI communicator
    /// @param format File format
    /// @param type File type
    File(MPI_Comm comm, Format format = Format::SZPLT, Type type = Type::FULL);

    /// Set dataset title
    ///
    /// @param title Dataset title
    void set_title(const std::string & title);

    void set_variable_names(const std::vector<std::string> & var_names);

    void write(const std::string & file_name);

    void create(const std::string & file_name);
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
    ///
    std::vector<std::string> var_names_;
    ///
    std::string file_name_;
    /// file handle
    void * handle_;
};

} // namespace teciocpp
