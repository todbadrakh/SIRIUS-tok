/* This file is part of SIRIUS electronic structure library.
 *
 * Copyright (c), ETH Zurich.  All rights reserved.
 *
 * Please, refer to the LICENSE file in the root directory.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __PP_DEBUG_DUMP_HPP__
#define __PP_DEBUG_DUMP_HPP__

#include <cstdlib>
#include <string>
#include <sstream>
#include "core/json.hpp"

namespace sirius {
namespace pp_debug_dump {

inline std::string
dir()
{
    if (auto* v = std::getenv("SIRIUS_DUMP_PP_STEPS_DIR"); v && std::string(v).size()) {
        return std::string(v);
    }
    return "";
}

inline std::string
tag()
{
    if (auto* v = std::getenv("SIRIUS_DUMP_PP_STEPS_TAG"); v && std::string(v).size()) {
        return std::string(v);
    }
    return "";
}

inline std::string
make_filename(std::string const& step__, int rank__, int iter__ = -1)
{
    std::stringstream ss;
    ss << step__;
    auto t = tag();
    if (!t.empty()) {
        ss << "_" << t;
    }
    if (iter__ >= 0) {
        ss << "_iter" << iter__;
    }
    if (rank__ >= 0) {
        ss << "_rank" << rank__;
    }
    ss << ".json";
    return ss.str();
}

inline void
write_json(nlohmann::json const& dict__, std::string const& step__, int rank__, int iter__ = -1)
{
    auto d = dir();
    if (d.empty()) {
        return;
    }
    std::string path = d + "/" + make_filename(step__, rank__, iter__);
    write_json_to_file(dict__, path);
}

} // namespace pp_debug_dump
} // namespace sirius

#endif
