/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint> // uint8_t, uint32_t

#include "cpu.h"
#include "ruc/format/print.h"
#include "ruc/meta/assert.h"

CPU::CPU(uint32_t frequency)
	: ProcessingUnit(frequency)
{
	m_shared_registers.emplace("a", &m_a);
	m_shared_registers.emplace("bc", &m_bc);
	m_shared_registers.emplace("de", &m_de);
	m_shared_registers.emplace("hl", &m_hl);
	m_shared_registers.emplace("sp", &m_sp);
	m_shared_registers.emplace("pc", &m_pc);
	m_shared_registers.emplace("z", &m_z);
	m_shared_registers.emplace("n", &m_n);
	m_shared_registers.emplace("h", &m_h);
	m_shared_registers.emplace("c", &m_c);
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

	m_a = 6;
	m_bc = 732;

	// print("This is an update from the CPU\n");
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

		// Drop overflown bits, the 'A' register is 8-bit
		m_a &= 0xff;
		break;
	default:
		VERIFY_NOT_REACHED();
		break;
	}
}
