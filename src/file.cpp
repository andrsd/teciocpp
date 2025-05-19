// SPDX-FileCopyrightText: 2025 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "teciocpp/file.h"
#include "teciocpp/exception.h"
#include "teciocpp/utils.h"
#include "TECIO.h"
#include <cassert>

namespace teciocpp {

namespace {

int32_t
file_format(File::Format format)
{
    if (format == File::Format::PLT)
        return FILEFORMAT_PLT;
    else if (format == File::Format::SZPLT)
        return FILEFORMAT_SZL;
    else
        throw Exception("Unknown file format");
}

int32_t
file_type(File::Type type)
{
    if (type == File::Type::FULL)
        return FILETYPE_FULL;
    else if (type == File::Type::GRID)
        return FILETYPE_GRID;
    else if (type == File::Type::SOLUTION)
        return FILETYPE_SOLUTION;
    else
        throw Exception("Unknown file type");
}

} // namespace

File::File(MPI_Comm comm, Format format, Type type) :
    comm_(comm),
    file_format_(format),
    file_type_(type),
    handle_(nullptr)
{
#if !defined TECIOCPP_WITH_MPI
    throw Exception("teciocpp was not built with MPI support");
#endif
}

void
File::set_title(const std::string & title)
{
    this->title_ = title;
}

void
File::set_variable_names(const std::vector<std::string> & var_names)
{
    this->var_names_ = var_names;
}

void
File::create(const std::string & file_name)
{
    assert(this->handle_ == nullptr);

    this->file_name_ = file_name;
    auto var_list = utils::join(" ", this->var_names_);
    if (tecFileWriterOpen(this->file_name_.c_str(),
                          this->title_.c_str(),
                          var_list.c_str(),
                          file_format(this->file_format_),
                          file_type(this->file_type_),
                          0,
                          nullptr,
                          &this->handle_) != 0)
        throw Exception("Failed to open file for writing");

#if defined TECIOCPP_WITH_MPI
    int32_t main_rank { 0 };
    // initialize an MPI communicator for each rank
    if (tecMPIInitialize(this->handle_, this->comm_, main_rank) != 0)
        throw Exception("Failed to create an MPI communicator for each rank");
#endif
}

void
File::write(const std::string & file_name)
{
    create(file_name);
    close();
}

void
File::close()
{
    assert(this->handle_ != nullptr);

    if (tecFileWriterClose(&this->handle_) != 0)
        throw Exception("Failed to generate file '{}'", this->file_name_);
    this->handle_ = nullptr;
}

} // namespace teciocpp
