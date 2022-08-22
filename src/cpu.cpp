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
		uint8_t opcode = read(m_pc);
		print("running opcode: {:#x}\n", opcode);
		switch (opcode) {

		case 0x01: ld16(); break;
		case 0x02: ld8(); break;
		case 0x08: ld16(); break;
		case 0x11: ld16(); break;
		case 0x12: ld8(); break;
		case 0x21: ld16(); break;
		case 0x22: ld8(); break;
		case 0x31: ld16(); break;
		case 0x32: ld8(); break;
		case 0x3e: ld8(); break;
		case 0xc3: jp16(); break;
		case 0xc6: add(); break;
		case 0xe0: ldh8(); break;
		case 0xf0: ldh8(); break;
		case 0xf8: ld16(); break;
		case 0xf9: ld16(); break;

		default:
			print("opcode {:#x} not implemented\n", opcode);
			VERIFY_NOT_REACHED();
		}
	}
}

void CPU::add()
{
	uint8_t opcode = pcRead();
	uint8_t immediate = pcRead();
	switch (opcode) {
	case 0xc6:
		// ADD A,d8
		m_wait_cycles += 8;

		// Flags: Z0HC
		m_zf = m_a + immediate == 0;
		m_nf = 0;
		m_hf = m_a + immediate > 16;
		m_cf = m_a + immediate > 255;

		// A = A + r
		m_a += immediate;

		// Drop overflown bits, the 'A' register is 8-bit
		m_a &= 0x00ff;
		break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::ld8()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x02:
		// LD (BC),A
		m_wait_cycles += 8;
		write(bc(), m_a);
		break;
	case 0x12:
		// LD (DE),A
		m_wait_cycles += 8;
		write(de(), m_a);
		break;
	case 0x22: {
		// LD (HL+),A == LD (HLI),A == LDI (HL),A
		m_wait_cycles += 8;

		// Put A into memory address in HL
		uint32_t address = hl();
		write(address, m_a);

		// Increment HL
		address = (address + 1) & 0xffff;
		m_l = address & 0x00ff;
		m_h = address >> 8;
		break;
	}
	case 0x32: {
		// LD (HL-),A == LD (HLD),A == LDD (HL),A
		m_wait_cycles += 8;

		// Put A into memory address in hl
		uint32_t address = hl();
		write(address, m_a);

		// Decrement HL
		address = (address - 1) & 0xffff;
		m_l = address & 0x00ff;
		m_h = address >> 8;
		break;
	}
	case 0x3e:
		// LD A,n
		m_wait_cycles += 8;
		m_a = pcRead();
		break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::ldh8()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0xe0:
		// LD ($ff00 + n),A == LDH (n),A
		m_wait_cycles += 12;

		// Put value in A into address (0xff00 + next byte in memory)
		ffWrite(pcRead(), m_a);
		break;
	case 0xf0:
		// LD A,($ff00 + n) == LDH A,(n)
		m_wait_cycles += 12;

		// Put value at address (0xff00 + next byte in memory) into A
		m_a = ffRead(pcRead());
		break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::ld16()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x01: {
		m_wait_cycles += 12;
		write(bc(), pcRead16());
		break;
	}
	case 0x08: {
		m_wait_cycles += 20;
		// TODO
		break;
	}
	case 0x11:
		m_wait_cycles += 12;
		write(de(), pcRead16());
		break;
	case 0x21:
		m_wait_cycles += 12;
		write(hl(), pcRead16());
		break;
	case 0x31: {
		m_wait_cycles += 12;
		m_sp = pcRead16();
		break;
	}
	case 0xf8: {
		m_wait_cycles += 12;
		// TODO
		// TODO flags
		break;
	}
	case 0xf9: {
		m_wait_cycles += 8;
		m_sp = hl();
		break;
	}
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::jp16()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0xc3:
		m_wait_cycles += 16;
		m_pc = pcRead16();
		break;
	default:
		VERIFY_NOT_REACHED();
	}
}

// -----------------------------------------

uint32_t CPU::pcRead()
{
	uint32_t data = Emu::the().readMemory(m_pc);
	m_pc = (m_pc + 1) & 0xffff;
	return data;
}

void CPU::write(uint32_t address, uint32_t value)
{
	Emu::the().writeMemory(address, value);
}

uint32_t CPU::read(uint32_t address)
{
	return Emu::the().readMemory(address);
}

void CPU::ffWrite(uint32_t address, uint32_t value)
{
	Emu::the().writeMemory(address | (0xff << 8), value);
}

uint32_t CPU::ffRead(uint32_t address)
{
	return Emu::the().readMemory(address | (0xff << 8));
}
