#ifndef COORD_SYSTEM_LABEL_FORMATTER_HPP
#define COORD_SYSTEM_LABEL_FORMATTER_HPP

#pragma once

#include "CoordSpaceDefs.h"

#include <string>
#include <string_view>
#include <format>

MY_COORD_SPACE_BEGIN

template<typename CU>
struct LabelFormatter
{
    explicit constexpr LabelFormatter(std::string_view labelFormat) noexcept
        : format{ labelFormat }
    {}

    explicit constexpr LabelFormatter(const std::string& labelFormat) noexcept
        : format{ labelFormat }
    {}

    raii_inline constexpr auto operator()(CU value) const noexcept {
        return std::vformat(format, std::make_format_args(value));
    }

    std::string format;
};

MY_COORD_SPACE_END

#endif