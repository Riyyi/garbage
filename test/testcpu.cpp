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

	bool isCarry(uint32_t limit_bit, uint32_t first, uint32_t second, uint32_t third = 0x0)
	{
		return cpu.isCarry(limit_bit, first, second, third);
	}

	bool isCarrySubtraction(uint32_t limit_bit, uint32_t lhs, uint32_t rhs, uint32_t rhs_extra = 0x0)
	{
		return cpu.isCarrySubtraction(limit_bit, lhs, rhs, rhs_extra);
	}
};

// -----------------------------------------

TEST_CASE(CPUIsCarry)
{
	CPUTest test;

	// 8-bit Half-carry false

	EXPECT(test.isCarry(0xf, 56, 17) == false);

	EXPECT(test.isCarrySubtraction(0xf, 136, 1) == false);
	EXPECT(test.isCarrySubtraction(0xf, 219, 20) == false);
	EXPECT(test.isCarrySubtraction(0xf, 250, 20) == false);

	// 8-bit Half-carry true

	EXPECT(test.isCarry(0xf, 46, 34) == true);
	EXPECT(test.isCarry(0xf, 62, 34) == true);

	EXPECT(test.isCarrySubtraction(0xf, 16, 1) == true);
	EXPECT(test.isCarrySubtraction(0xf, 32, 1) == true);
	EXPECT(test.isCarrySubtraction(0xf, 144, 1) == true);
	EXPECT(test.isCarrySubtraction(0xf, 211, 20) == true);

	// 8-bit Full carry false

	EXPECT(test.isCarry(0xff, 254, 1) == false);

	// 8-bit Full carry true

	EXPECT(test.isCarry(0xff, 254, 2) == true);
}
