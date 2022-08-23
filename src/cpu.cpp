/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint> // int32_t, uint8_t, uint32_t

#include "cpu.h"
#include "emu.h"
#include "ruc/format/color.h"
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

		case 0x20: jr8(); break;
		case 0x01: ld16(); break;
		case 0x02: ld8(); break;
		case 0x06: ld8(); break;
		case 0x08: ld16(); break;
		case 0x0a: ld8(); break;
		case 0x0d: dec8(); break;
		case 0x0e: ld8(); break;
		case 0x11: ld16(); break;
		case 0x12: ld8(); break;
		case 0x16: ld8(); break;
		case 0x1a: ld8(); break;
		case 0x1e: ld8(); break;
		case 0x21: ld16(); break;
		case 0x22: ld8(); break;
		case 0x26: ld8(); break;
		case 0x2a: ld8(); break;
		case 0x2e: ld8(); break;
		case 0x2f: misc(); break;
		case 0x31: ld16(); break;
		case 0x32: ld8(); break;
		case 0x36: ld8(); break;
		case 0x3a: ld8(); break;
		case 0x3e: ld8(); break;
		case 0xa8: xor8(); break;
		case 0xaf: xor8(); break;
		case 0xc3: jp16(); break;
		case 0xc6: add(); break;
		case 0xcd: call(); break;
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
		// ADD A,d8, flags: Z 0 H C
		m_wait_cycles += 8;

		// Set flags
		m_zf = m_a + immediate == 0;
		m_nf = 0;
		m_hf = m_a + immediate > 16;
		m_cf = m_a + immediate > 255;

		// A = A + r
		m_a = (m_a + immediate) & 0x00ff;
		break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::dec8()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x0d: {
		// DEC C, flags: Z 1 H -
		m_wait_cycles += 4;

		m_c = (m_c - 1) & 0x00ff;

		// Set flags
		// TODO
		// m_zf = ?
		m_nf = 1;
		// m_hf = ?
		break;
	}
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::xor8()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0xa8:
		// XOR B, flags: Z 0 0 0
		m_nf = m_hf = m_cf = 0;
		m_a ^= m_b;
		m_zf = m_a == 0;
		break;
	case 0xaf:
		// XOR A, flags: 1 0 0 0
		// A^A will always be 0
		m_a = m_nf = m_hf = m_cf = 0;
		m_zf = 1;
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
	case 0x06:
		// LD B,n
		m_wait_cycles += 8;
		m_b = pcRead();
		break;
	case 0x0a:
		// LD A,(BC)
		m_wait_cycles += 8;
		m_a = read(bc());
		break;
	case 0x0e:
		// LD C,n
		m_wait_cycles += 8;
		m_c = pcRead();
		break;
	case 0x12:
		// LD (DE),A
		m_wait_cycles += 8;
		write(de(), m_a);
		break;
	case 0x16:
		// LD D,n
		m_wait_cycles += 8;
		m_d = pcRead();
		break;
	case 0x1a:
		// LD A,(DE)
		m_wait_cycles += 8;
		m_a = read(de());
		break;
	case 0x1e:
		// LD E,n
		m_wait_cycles += 8;
		m_e = pcRead();
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
	case 0x26:
		// LD H,n
		m_wait_cycles += 8;
		m_h = pcRead();
		break;
	case 0x2a: {
		// LD A,(HL+) == LD A,(HLI) == LDI A,(HL)
		m_wait_cycles += 8;

		// Put value at address in HL into A
		uint32_t address = hl();
		m_a = read(address);

		// Increment HL
		address = (address + 1) & 0xffff;
		m_l = address & 0x00ff;
		m_h = address >> 8;
		break;
	}
	case 0x2e:
		// LD L,n
		m_wait_cycles += 8;
		m_l = pcRead();
		break;
	case 0x32: {
		// LD (HL-),A == LD (HLD),A == LDD (HL),A
		m_wait_cycles += 8;

		// Put A into memory address in HL
		uint32_t address = hl();
		write(address, m_a);

		// Decrement HL
		address = (address - 1) & 0xffff;
		m_l = address & 0x00ff;
		m_h = address >> 8;
		break;
	}
	case 0x36:
		// LD (HL),n
		m_wait_cycles += 12;
		write(hl(), pcRead());
		break;
	case 0x3a: {
		// LD A,(HL-) == LD A,(HLD) == LDD A,(HL)
		m_wait_cycles += 8;

		// Put value at address in HL into A
		uint32_t address = hl();
		m_a = read(address);

		// Decrement HL
		address = (address + 1) & 0xffff;
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
		// LD (nn),SP
		m_wait_cycles += 20;

		// Put value of SP into address given by next 2 bytes in memory
		// TODO
		break;
	}
	case 0x11:
		// LD DE,nn
		m_wait_cycles += 12;
		write(de(), pcRead16());
		break;
	case 0x21:
		// LD HL,nn
		m_wait_cycles += 12;
		write(hl(), pcRead16());
		break;
	case 0x31: {
		// LD SP,nn
		m_wait_cycles += 12;
		m_sp = pcRead16();
		break;
	}
	case 0xf8: {
		// LD HL,SP + e8 == LDHL SP,e8
		m_wait_cycles += 12;

		// Put SP + next (signed) byte in memory into HL
		// TODO

		// Unsets ZF and NF, may enable HF and CF
		// TODO flags
		break;
	}
	case 0xf9: {
		// LD SP,HL
		m_wait_cycles += 8;
		m_sp = hl();
		break;
	}
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::call()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0xcd: {
		// CALL nn
		m_wait_cycles += 24;

		uint32_t data = pcRead16();

		// Push address of next 2 bytes in memory onto stack
		m_sp = (m_sp - 1) & 0xffff;
		write(m_sp, data >> 8);
		m_sp = (m_sp - 1) & 0xffff;
		write(m_sp, data & 0x00ff);

		// Jump to this address
		m_pc = data;
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
		// JP nn
		m_wait_cycles += 16;
		m_pc = pcRead16();
		break;
	default:
		VERIFY_NOT_REACHED();
	}
}

// Jump relative
void CPU::jr8()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x20: {
		// JR NZ,e8
		m_wait_cycles += 8;

		if (!m_zf) {
			m_wait_cycles += 4;
			// TODO
		}
		break;
	}
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::misc()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x2f:
		// CPL, flags: - 1 1 -
		m_wait_cycles += 4;

		// Complement register A (flip all bits)
		m_a ^= 0x00ff; // equivalent to: m_a = ~m_a & 0x00ff

		// Set flags
		m_nf = m_hf = 1;
		break;
	default:
		VERIFY_NOT_REACHED();
	}
}

// -----------------------------------------

uint32_t CPU::pcRead()
{
	uint32_t data = Emu::the().readMemory(m_pc) & 0x00ff;
	m_pc = (m_pc + 1) & 0xffff;
	return data;
}

void CPU::write(uint32_t address, uint32_t value)
{
	Emu::the().writeMemory(address, value & 0x00ff);
}

uint32_t CPU::read(uint32_t address)
{
	return Emu::the().readMemory(address) & 0x00ff;
}

void CPU::ffWrite(uint32_t address, uint32_t value)
{
	Emu::the().writeMemory(address | (0xff << 8), value & 0x00ff);
}

uint32_t CPU::ffRead(uint32_t address)
{
	return Emu::the().readMemory(address | (0xff << 8)) & 0x00ff;
}

// -----------------------------------------

void Formatter<CPU>::parse(Parser& parser)
{
	parser.parseSpecifier(specifier, Parser::ParameterType::UserDefined);
}

void Formatter<CPU>::format(Builder& builder, const CPU& value) const
{
	if (!specifier.alternativeForm) {
		builder.putString(
			::format("| AF {:#06x} | BC {:#06x} | DE {:#06x} | HL {:#06x} | PC {:#06x} | SP {:#06x} |",
		             value.af(), value.bc(), value.de(), value.hl(), value.pc(), value.sp()));
		return;
	}

	Formatter<uint32_t> formatter { .specifier = specifier };
	builder.putString("AF: ");
	formatter.format(builder, value.af());
	builder.putString("\nBC: ");
	formatter.format(builder, value.bc());
	builder.putString("\nDE: ");
	formatter.format(builder, value.de());
	builder.putString("\nHL: ");
	formatter.format(builder, value.hl());
	builder.putString("\nPC: ");
	formatter.format(builder, value.pc());
	builder.putString("\nSP: ");
	formatter.format(builder, value.sp());
}
