/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "cpu.h"
#include "macro.h"
#include "testcase.h"
#include "testsuite.h"

struct CPUTest {
	CPU cpu { 0 };

	bool isCarry(uint32_t lhs, uint32_t rhs, uint32_t limit_bit) { return cpu.isCarry(lhs, rhs, limit_bit); }
};

// -----------------------------------------

TEST_CASE(CPUIsCarry)
{
	CPUTest test;

	// 8-bit Half-carry false

	EXPECT_EQ(test.isCarry(56, 17, 0x10), false);

	EXPECT_EQ(test.isCarry(16, -1, 0x10), false);
	EXPECT_EQ(test.isCarry(32, -1, 0x10), false);
	EXPECT_EQ(test.isCarry(144, -1, 0x10), false);
	EXPECT_EQ(test.isCarry(211, -20, 0x10), false);

	// 8-bit Half-carry true

	EXPECT_EQ(test.isCarry(46, 34, 0x10), true);
	EXPECT_EQ(test.isCarry(62, 34, 0x10), true);

	EXPECT_EQ(test.isCarry(136, -1, 0x10), true);
	EXPECT_EQ(test.isCarry(219, -20, 0x10), true);
	EXPECT_EQ(test.isCarry(250, -20, 0x10), true);

	// 8-bit Full carry false

	EXPECT_EQ(test.isCarry(254, 1, 0x100), false);

	// 8-bit Full carry true

	EXPECT_EQ(test.isCarry(254, 2, 0x100), true);
}
