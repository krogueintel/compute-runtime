/*
 * Copyright (C) 2017-2018 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "runtime/context/context.h"
#include "unit_tests/fixtures/device_host_queue_fixture.h"

using namespace OCLRT;
namespace DeviceHostQueue {
typedef ::testing::Types<CommandQueue, DeviceQueue> QueueTypes;

template <typename T>
class clRetainReleaseCommandQueueTests : public DeviceHostQueueFixture<T> {};

TYPED_TEST_CASE(clRetainReleaseCommandQueueTests, QueueTypes);

TYPED_TEST(clRetainReleaseCommandQueueTests, retain_release) {
    using BaseType = typename TypeParam::BaseType;

    auto queue = this->createClQueue();
    ASSERT_EQ(CL_SUCCESS, this->retVal);
    auto qObject = castToObject<TypeParam>(static_cast<BaseType *>(queue));
    ASSERT_NE(qObject, nullptr);

    cl_uint refCount;
    this->retVal = clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT,
                                         sizeof(cl_uint), &refCount, NULL);
    EXPECT_EQ(CL_SUCCESS, this->retVal);
    EXPECT_EQ(1u, refCount);

    this->retVal = clRetainCommandQueue(queue);
    EXPECT_EQ(CL_SUCCESS, this->retVal);

    this->retVal = clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT,
                                         sizeof(cl_uint), &refCount, NULL);
    EXPECT_EQ(CL_SUCCESS, this->retVal);
    EXPECT_EQ(2u, refCount);

    this->retVal = clReleaseCommandQueue(queue);
    EXPECT_EQ(CL_SUCCESS, this->retVal);

    this->retVal = clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT,
                                         sizeof(cl_uint), &refCount, NULL);
    EXPECT_EQ(CL_SUCCESS, this->retVal);
    EXPECT_EQ(1u, refCount);

    this->retVal = clReleaseCommandQueue(queue);
    EXPECT_EQ(CL_SUCCESS, this->retVal);
}
} // namespace DeviceHostQueue
