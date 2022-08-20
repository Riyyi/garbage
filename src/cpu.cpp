/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint> // int32_t, uint8_t, uint32_t

#include "cpu.h"
#include "emu.h"
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
	// , m_pc(0x100)
	, m_pc(0x0)
	, m_sp(0xfffe)
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

	// Add opcode functions to lookup table
	m_opcode_lookup_table.emplace(0xc6, std::bind(&CPU::add, this));
	m_opcode_lookup_table.emplace(0x01, std::bind(&CPU::ld16, this));
	m_opcode_lookup_table.emplace(0x11, std::bind(&CPU::ld16, this));
	m_opcode_lookup_table.emplace(0x21, std::bind(&CPU::ld16, this));
	m_opcode_lookup_table.emplace(0x31, std::bind(&CPU::ld16, this));
}

CPU::~CPU()
{
}

// -----------------------------------------

void CPU::update()
{
	m_wait_cycles--;
	if (m_wait_cycles <= 0) {
		// Read next opcode
		uint8_t opcode = peekBootrom();
		m_opcode_lookup_table[opcode]();
	}

	// print("This is an update from the CPU\n");
}

void CPU::add()
{
	uint8_t opcode = consumeBootrom();
	uint8_t immediate = consumeBootrom();
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
	}
}

void CPU::ld16()
{
	uint8_t opcode = consumeBootrom();
	switch (opcode) {
	case 0x01: {
		m_wait_cycles += 12;
		setBc(immediate16());
		break;
	}
	case 0x11:
		m_wait_cycles += 12;
		setDe(immediate16());
		break;
	case 0x21:
		m_wait_cycles += 12;
		setHl(immediate16());
		break;
	case 0x31: {
		m_wait_cycles += 12;
		m_sp = immediate16();
		break;
	}
	default:
		VERIFY_NOT_REACHED();
	}
}

// -----------------------------------------

uint8_t CPU::peekBootrom(int32_t offset) const
{
	return Emu::the().bootrom()[m_pc + offset];
}

uint8_t CPU::consumeBootrom()
{
	return Emu::the().bootrom()[m_pc++];
}

void CPU::setBc(uint32_t value)
{
	m_b = value >> 8;
	m_c = value;
}

void CPU::setDe(uint32_t value)
{
	m_d = value >> 8;
	m_e = value;
}

void CPU::setHl(uint32_t value)
{
	m_h = value >> 8;
	m_l = value;
}
