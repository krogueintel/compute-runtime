/*
 * Copyright (C) 2018 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "runtime/os_interface/windows/os_interface.h"
#include "unit_tests/os_interface/windows/hw_info_config_win_tests.h"

using namespace OCLRT;
using namespace std;

using HwInfoConfigTestWindowsBxt = HwInfoConfigTestWindows;

BXTTEST_F(HwInfoConfigTestWindowsBxt, whenCallAdjustPlatformThenDoNothing) {
    EXPECT_EQ(IGFX_BROXTON, productFamily);
    auto hwInfoConfig = HwInfoConfig::get(productFamily);
    hwInfoConfig->adjustPlatformForProductFamily(&testHwInfo);

    int ret = memcmp(outHwInfo.pPlatform, testHwInfo.pPlatform, sizeof(PLATFORM));
    EXPECT_EQ(0, ret);
}
