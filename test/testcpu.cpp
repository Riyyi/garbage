/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <vector>

#include "cpu.h"
#include "emu.h"
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

CPU runCPUTest(std::vector<uint8_t> test)
{
	CPU cpu(4000000);

	Emu::the().destroy();
	Emu::the().init(8000000);
	Emu::the().addProcessingUnit("cpu", &cpu);
	Emu::the().addMemorySpace("FULL", 0x0000, 0xffff);

	// Load the test
	for (size_t i = 0; i < test.size(); ++i) {
		Emu::the().writeMemory(i, test[i]);
	}

	// Run the test
	while (cpu.pc() < test.size()) {
		Emu::the().update();
	}

	return cpu;
}

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

TEST_CASE(CPUAddPlusCarry)
{
	CPU cpu(0);

	// ADC A,E

	std::vector<uint8_t> adc_r8 = {
		// clang-format off
		0x3e, 0xe1,       // LD A,i8
		0x1e, 0x0f,       // LD E,i8
		0x37,             // SCF
		0x8b,             // ADC A,E
		// clang-format on
	};
	cpu = runCPUTest(adc_r8);
	EXPECT_EQ(cpu.a(), 0xf1);
	EXPECT_EQ(cpu.zf(), 0x0);
	EXPECT_EQ(cpu.nf(), 0x0);
	EXPECT_EQ(cpu.hf(), 0x1);
	EXPECT_EQ(cpu.cf(), 0x0);

	// ADC A,i8

	std::vector<uint8_t> adc_i8 = {
		// clang-format off
		0x3e, 0xe1,       // LD A,i8
		0x37,             // SCF
		0xce, 0x3b,       // ADC A,i8
		// clang-format on
	};
	cpu = runCPUTest(adc_i8);
	EXPECT_EQ(cpu.a(), 0x1d);
	EXPECT_EQ(cpu.zf(), 0x0);
	EXPECT_EQ(cpu.nf(), 0x0);
	EXPECT_EQ(cpu.hf(), 0x0);
	EXPECT_EQ(cpu.cf(), 0x1);

	// ADC A,(HL)

	std::vector<uint8_t> adc_hl = {
		// clang-format off
		0x3e, 0xe1,       // LD A,i8
		0x36, 0x1e,       // LD (HL),i8
		0x37,             // SCF
		0x8e,             // ADC A,(HL)
		// clang-format on
	};
	cpu = runCPUTest(adc_hl);
	EXPECT_EQ(cpu.a(), 0x0);
	EXPECT_EQ(cpu.zf(), 0x1);
	EXPECT_EQ(cpu.nf(), 0x0);
	EXPECT_EQ(cpu.hf(), 0x1);
	EXPECT_EQ(cpu.cf(), 0x1);
}

TEST_CASE(CPUSetStackPointer)
{
	std::vector<uint8_t> test = { 0x31, 0xfe, 0xff }; // LD SP,i16
	CPU cpu = runCPUTest(test);
	EXPECT_EQ(cpu.sp(), 0xfffe);
}

TEST_CASE(CPUPushToStack)
{
	// PUSH BC

	std::vector<uint8_t> push_bc = {
		// clang-format off
		0x31, 0xfe, 0xff, // LD SP,i16
		0x01, 0xfc, 0xff, // LD BC,i16
		0xc5,             // PUSH BC
		// clang-format on
	};
	CPU cpu = runCPUTest(push_bc);
	EXPECT_EQ(cpu.bc(), 0xfffc);
	EXPECT_EQ(cpu.sp(), 0xfffc);
	EXPECT_EQ(Emu::the().readMemory(0xfffd), 0xff);
	EXPECT_EQ(Emu::the().readMemory(0xfffc), 0xfc);
}

TEST_CASE(CPUPopFromStack)
{
	// POP BC

	std::vector<uint8_t> pop_bc = {
		// clang-format off
		0x31, 0xfe, 0xff, // LD SP,i16
		0x11, 0x5f, 0x3c, // LD DE,i16
		0xd5,             // PUSH DE
		0xc1,             // POP BC
		// clang-format on
	};
	CPU cpu = runCPUTest(pop_bc);
	EXPECT_EQ(cpu.bc(), 0x3c5f);
	EXPECT_EQ(cpu.sp(), 0xfffe);
	EXPECT_EQ(Emu::the().readMemory(0xfffd), 0x3c);
	EXPECT_EQ(Emu::the().readMemory(0xfffc), 0x5f);
}
