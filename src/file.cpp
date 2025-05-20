// SPDX-FileCopyrightText: 2025 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "teciocpp/file.h"
#include "teciocpp/enums.h"
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

int32_t
zone_type(ZoneType type)
{
    switch (type) {
    case ZoneType::ORDERED:
        return ZONETYPE_ORDERED;
    case ZoneType::FELINE:
        return ZONETYPE_FELINESEG;
    case ZoneType::FETRIANGLE:
        return ZONETYPE_FETRIANGLE;
    case ZoneType::FEQUADRILATERAL:
        return ZONETYPE_FEQUADRILATERAL;
    case ZoneType::FETETRAHEDRON:
        return ZONETYPE_FETETRAHEDRON;
    case ZoneType::FEBRICK:
        return ZONETYPE_FEBRICK;
    case ZoneType::FEPOLYGON:
        return ZONETYPE_FEPOLYGON;
    case ZoneType::FEPOLYHEDRON:
        return ZONETYPE_FEPOLYHEDRON;
    default:
        throw Exception("Unknown zone type");
    }
}

int32_t
value_location(ValueLocation val)
{
    switch (val) {
    case ValueLocation::NODAL:
        return 1;
    case ValueLocation::ELEMENTAL:
        return 0;
    default:
        throw Exception("Unknown value location");
    }
}

template <typename T, typename Fn>
std::vector<int32_t>
convert(const std::vector<T> & vals, Fn fn)
{
    std::vector<int32_t> res(vals.size());
    std::transform(vals.begin(), vals.end(), res.begin(), fn);
    return res;
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
File::create(const std::string & file_name,
             const std::string & title,
             const std::vector<std::string> & var_names)
{
    assert(this->handle_ == nullptr);

    this->file_name_ = file_name;
    this->title_ = title;
    this->var_names_ = var_names;
    auto var_list = utils::join(" ", this->var_names_);
    if (tecFileWriterOpen(this->file_name_.c_str(),
                          this->title_.c_str(),
                          var_list.c_str(),
                          file_format(this->file_format_),
                          file_type(this->file_type_),
                          FieldDataType_Double,
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

int32_t
File::zone_create_fe(const std::string & zone_title,
                     ZoneType zn_type,
                     int64_t n_nodes,
                     int64_t n_cells,
                     const std::vector<ValueLocation> & value_locations)
{
    int32_t zone;
    auto val_locs = convert(value_locations, value_location);
    if (tecZoneCreateFE(this->handle_,
                        zone_title.data(),
                        zone_type(zn_type),
                        n_nodes,
                        n_cells,
                        NULL,
                        NULL,
                        val_locs.data(),
                        NULL,
                        0,
                        0,
                        0,
                        &zone) != 0)
        throw Exception("Failed to write zone");
    return zone;
}

int32_t
File::zone_create_fe_mixed(const std::string & zone_title,
                           int64_t n_nodes,
                           const std::vector<int32_t> & cell_shapes,
                           const std::vector<int32_t> & grid_orders,
                           const std::vector<int32_t> & basis_fns,
                           const std::vector<int64_t> & n_elems,
                           const std::vector<ValueLocation> & value_locations)
{
    int32_t zone;
    int32_t n_sections = cell_shapes.size();
    auto val_locs = convert(value_locations, value_location);
    if (tecZoneCreateFEMixed(this->handle_,
                             zone_title.c_str(),
                             n_nodes,
                             n_sections,
                             cell_shapes.data(),
                             grid_orders.data(),
                             basis_fns.data(),
                             n_elems.data(),
                             NULL,
                             NULL,
                             val_locs.data(),
                             NULL,
                             0,
                             0,
                             0,
                             &zone) != 0)
        throw Exception("Failed to write zone");
    return zone;
}

void
File::zone_var_write(int32_t zone, int32_t var, int32_t partition, const std::vector<double> & vals)
{
    if (tecZoneVarWriteDoubleValues(this->handle_,
                                    zone,
                                    var,
                                    partition,
                                    vals.size(),
                                    vals.data()) != 0)
        throw Exception("Failed to write values fro variable {}", var);
}

void
File::zone_var_write(int32_t zone, int32_t var, int32_t partition, const std::vector<float> & vals)
{
    if (tecZoneVarWriteFloatValues(this->handle_, zone, var, partition, vals.size(), vals.data()) !=
        0)
        throw Exception("Failed to write values fro variable {}", var);
}

void
File::zone_node_map_write(int32_t zone,
                          int32_t partition,
                          const std::vector<int32_t> & connectivity)
{
    int32_t nodes_are_one_based = 1;
    int64_t n = connectivity.size();
    if (tecZoneNodeMapWrite32(this->handle_,
                              zone,
                              partition,
                              nodes_are_one_based,
                              n,
                              connectivity.data()) != 0)
        throw Exception("Failed to write zone node map");
}

void
File::zone_node_map_write(int32_t zone,
                          int32_t partition,
                          const std::vector<int64_t> & connectivity)
{
    int32_t nodes_are_one_based = 1;
    int64_t n = connectivity.size();
    if (tecZoneNodeMapWrite64(this->handle_,
                              zone,
                              partition,
                              nodes_are_one_based,
                              n,
                              connectivity.data()) != 0)
        throw Exception("Failed to write zone node map");
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
