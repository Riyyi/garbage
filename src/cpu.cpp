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
	// https://gbdev.io/pandocs/Power_Up_Sequence.html#cpu-registers
    // CGB registers
	, m_a(0x11)
	, m_b(0x0)
	, m_c(0x0)
	, m_d(0xff)
	, m_e(0x56)
	, m_h(0x0)
	, m_l(0x0d)
	, m_pc(0x100)
	, m_sp(0xffe)
	// Flags
	, m_zf(0x1)
	, m_nf(0x0)
	, m_hf(0x0)
	, m_cf(0x0)
{
	m_shared_registers.emplace("a", &m_a);
	m_shared_registers.emplace("b", &m_b);
	m_shared_registers.emplace("c", &m_c);
	m_shared_registers.emplace("d", &m_d);
	m_shared_registers.emplace("e", &m_e);
	m_shared_registers.emplace("h", &m_h);
	m_shared_registers.emplace("l", &m_l);
	m_shared_registers.emplace("sp", &m_sp);
	m_shared_registers.emplace("pc", &m_pc);
	m_shared_registers.emplace("zf", &m_zf);
	m_shared_registers.emplace("nf", &m_nf);
	m_shared_registers.emplace("hf", &m_hf);
	m_shared_registers.emplace("cf", &m_cf);
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
		m_zf = m_a + immediate == 0;
		m_nf = 0;
		m_hf = m_a + immediate > 16;
		m_cf = m_a + immediate > 255;

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
