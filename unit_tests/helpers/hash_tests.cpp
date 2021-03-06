/*
 * Copyright (C) 2018 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "runtime/helpers/hash.h"
#include "gtest/gtest.h"

using namespace OCLRT;

TEST(HashTests, givenSamePointersWhenHashIsCalculatedThenSame32BitValuesAreGenerated) {
    uintptr_t ptr1UI = 1;
    uintptr_t ptr2UI = 1;
    void *ptr1 = reinterpret_cast<void *>(ptr1UI);
    void *ptr2 = reinterpret_cast<void *>(ptr2UI);

    uint32_t hash1 = hashPtrToU32(ptr1);
    uint32_t hash2 = hashPtrToU32(ptr2);

    EXPECT_EQ(hash1, hash2);
}

TEST(HashTests, givenDifferentPointersWhenHashIsCalculatedThenUnique32BitValuesAreGenerated) {
    uintptr_t ptr1UI = 1;
    uintptr_t ptr2UI = ptr1UI | (ptr1UI << ((sizeof(uintptr_t) / 2) * 8));
    void *ptr1 = reinterpret_cast<void *>(ptr1UI);
    void *ptr2 = reinterpret_cast<void *>(ptr2UI);

    uint32_t hash1 = hashPtrToU32(ptr1);
    uint32_t hash2 = hashPtrToU32(ptr2);

    EXPECT_NE(hash1, hash2);
}
