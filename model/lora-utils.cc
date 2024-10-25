/*
 * Copyright (c) 2017 University of Padova
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Romagnolo Stefano <romagnolostefano93@gmail.com>
 */

#include "lora-utils.h"

#include <cmath>

namespace ns3
{
namespace lorawan
{

double
DbToRatio(double dB)
{
    double ratio = std::pow(10.0, dB / 10.0);
    return ratio;
}

double
DbmToW(double dBm)
{
    double mW = std::pow(10.0, dBm / 10.0);
    return mW / 1000.0;
}

double
WToDbm(double w)
{
    return 10.0 * std::log10(w * 1000.0);
}

double
RatioToDb(double ratio)
{
    return 10.0 * std::log10(ratio);
}

} // namespace lorawan
} // namespace ns3