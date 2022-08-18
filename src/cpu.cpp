/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#include "cpu.h"
#include "emu.h"
#include <iostream>

CPU::CPU(unsigned int frequency)
	: ProcessingUnit(frequency)
{
}

CPU::~CPU()
{
}

void CPU::update()
{
	printf("This is an update from the CPU\n");
	Emu::the().writeMemory("RAM", 123, 42);
	printf("fff");
}

void CPU::add(uint8_t byte, uint8_t immediate)
{
	switch (byte) {
	case 0xc6: // ADD A,d8
		// program_counter += 2;
		// clock += 8;

		// Flags: Z0HC
		m_z = m_a + immediate == 0;
		m_n = 0;
		m_h = m_a + immediate > 16;
		m_c = m_a + immediate > 255;

		// A = A + r
		m_a += immediate;
		break;
	default:
		break;
	}
}
