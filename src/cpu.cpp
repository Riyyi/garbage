/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint> // uint8_t, uint32_t

#include "cpu.h"
#include "ruc/format/print.h"

CPU::CPU(uint32_t frequency)
	: ProcessingUnit(frequency)
{
}

CPU::~CPU()
{
}

void CPU::update()
{
	m_wait_cycles--;
	if (m_wait_cycles <= 0) {
		// Read next opcode
	}

	print("This is an update from the CPU\n");
}

void CPU::add(uint8_t opcode, uint8_t immediate)
{
	switch (opcode) {
	case 0xc6: // ADD A,d8
		// Program counter +2
		m_pc += 2;

		// Clock cycles +8
		m_wait_cycles += 8;

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
