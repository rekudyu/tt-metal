// SPDX-FileCopyrightText: © 2024 Tenstorrent Inc.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "ttnn-pybind/pybind_fwd.hpp"

namespace ttnn::operations::data_movement::detail {
namespace py = pybind11;

void bind_tilize_with_val_padding(py::module& module);

void bind_tilize_with_zero_padding(py::module& module);
}  // namespace ttnn::operations::data_movement::detail
