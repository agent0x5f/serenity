/*
 * Copyright (c) 2022, Leon Albrecht <leon2002.la@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Concepts.h>
#include <AK/Types.h>

#include <AK/Format.h>

namespace AK {

template<Integral I>
constexpr I pow(I base, I exponent)
{
    // https://en.wikipedia.org/wiki/Exponentiation_by_squaring
    if (exponent < 0)
        return 0;
    if (exponent == 0)
        return 1;

    I res = 1;
    while (exponent > 0) {
        if (exponent & 1)
            res *= base;
        base *= base;
        exponent /= 2u;
    }
    return res;
}

}
