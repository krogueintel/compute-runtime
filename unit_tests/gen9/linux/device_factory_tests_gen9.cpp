/*
 * Copyright (C) 2017-2018 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "unit_tests/os_interface/linux/device_factory_tests.h"

typedef DeviceFactoryLinuxTest DeviceFactoryLinuxTestSkl;

GEN9TEST_F(DeviceFactoryLinuxTestSkl, queryWhitelistedPreemptionRegister) {
    pDrm->StoredPreemptionSupport = 1;
    HardwareInfo *hwInfo = nullptr;
    size_t numDevices = 0;

    bool success = DeviceFactory::getDevices(&hwInfo, numDevices, executionEnvironment);
    EXPECT_TRUE(success);
#if defined(I915_PARAM_HAS_PREEMPTION)
    EXPECT_TRUE(hwInfo->capabilityTable.whitelistedRegisters.csChicken1_0x2580);
#else
    EXPECT_FALSE(hwInfo->capabilityTable.whitelistedRegisters.csChicken1_0x2580);
#endif

    DeviceFactory::releaseDevices();
}

GEN9TEST_F(DeviceFactoryLinuxTestSkl, queryNotWhitelistedPreemptionRegister) {
    pDrm->StoredPreemptionSupport = 0;
    HardwareInfo *hwInfo = nullptr;
    size_t numDevices = 0;

    bool success = DeviceFactory::getDevices(&hwInfo, numDevices, executionEnvironment);
    EXPECT_TRUE(success);
    EXPECT_FALSE(hwInfo->capabilityTable.whitelistedRegisters.csChicken1_0x2580);

    DeviceFactory::releaseDevices();
}
