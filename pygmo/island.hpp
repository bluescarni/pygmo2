// Copyright 2019 PaGMO development team
//
// This file is part of the pygmo library.
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef PYGMO_ISLAND_HPP
#define PYGMO_ISLAND_HPP

#include <memory>
#include <string>
#include <type_traits>

#include <pybind11/pybind11.h>

#include <pagmo/island.hpp>
#include <pagmo/s11n.hpp>

#include "common_base.hpp"

namespace pagmo
{

namespace detail
{

namespace py = pybind11;

// Disable the static UDI checks for py::object.
template <>
struct disable_udi_checks<py::object> : std::true_type {
};

template <>
struct isl_inner<py::object> final : isl_inner_base, pygmo::common_base {
    // Just need the def ctor, delete everything else.
    isl_inner() = default;
    isl_inner(const isl_inner &) = delete;
    isl_inner(isl_inner &&) = delete;
    isl_inner &operator=(const isl_inner &) = delete;
    isl_inner &operator=(isl_inner &&) = delete;
    explicit isl_inner(const py::object &);
    virtual std::unique_ptr<isl_inner_base> clone() const override final;
    // Mandatory methods.
    virtual void run_evolve(island &) const override final;
    // Optional methods.
    virtual std::string get_name() const override final;
    virtual std::string get_extra_info() const override final;
    template <typename Archive>
    void save(Archive &, unsigned) const;
    template <typename Archive>
    void load(Archive &, unsigned);
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    py::object m_value;
};

} // namespace detail

} // namespace pagmo

// Register the isl_inner specialisation for py::object.
PAGMO_S11N_ISLAND_EXPORT_KEY(pybind11::object)

namespace pygmo
{

namespace py = pybind11;

py::tuple island_pickle_getstate(const pagmo::island &);
pagmo::island island_pickle_setstate(py::tuple);

} // namespace pygmo

#endif
