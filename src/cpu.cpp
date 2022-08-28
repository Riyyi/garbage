/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint> // uint8_t, uint32_t

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
	// FIXME: Figure out if other ProcessingUnits require access to these registers,
	//        delete this functionality if they dont
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

		// print(ruc::format::Emphasis::Underline | ruc::format::Emphasis::Bold | fg(ruc::format::TerminalColor::Blue), "{:#06x}\n", *this);

		// Read next opcode
		uint8_t opcode = read(m_pc);
		print("running opcode: {:#04x} @ ({:#06x})\n", opcode, m_pc);
		switch (opcode) {

		case 0x00: nop(); break;
		case 0x01: ldi16(); break;
		case 0x02: ldr8(); break;
		case 0x04: inc8(); break;
		case 0x05: dec8(); break;
		case 0x06: ldi8(); break;
		case 0x07: ra(); break;
		case 0x08: ldr16(); break;
		case 0x09: addr16(); break;
		case 0x0a: ldr8(); break;
		case 0x0b: dec16(); break;
		case 0x0c: inc8(); break;
		case 0x0d: dec8(); break;
		case 0x0e: ldi8(); break;
		case 0x0f: ra(); break;
		case 0x11: ldi16(); break;
		case 0x12: ldr8(); break;
		case 0x14: inc8(); break;
		case 0x15: dec8(); break;
		case 0x16: ldi8(); break;
		case 0x17: ra(); break;
		case 0x18: jrs8(); break;
		case 0x19: addr16(); break;
		case 0x1a: ldr8(); break;
		case 0x1b: dec16(); break;
		case 0x1c: inc8(); break;
		case 0x1d: dec8(); break;
		case 0x1e: ldi8(); break;
		case 0x1f: ra(); break;
		case 0x20: jrs8(); break;
		case 0x21: ldi16(); break;
		case 0x22: ldr8(); break;
		case 0x24: inc8(); break;
		case 0x25: dec8(); break;
		case 0x26: ldi8(); break;
		case 0x28: jrs8(); break;
		case 0x29: addr16(); break;
		case 0x2a: lda8(); break;
		case 0x2b: dec16(); break;
		case 0x2c: inc8(); break;
		case 0x2d: dec8(); break;
		case 0x2e: ldi8(); break;
		case 0x2f: misc(); break;
		case 0x30: jrs8(); break;
		case 0x31: ldi16(); break;
		case 0x32: ldr8(); break;
		case 0x34: inc8(); break;
		case 0x35: dec8(); break;
		case 0x36: ldi8(); break;
		case 0x38: jrs8(); break;
		case 0x39: addr16(); break;
		case 0x3a: ldr8(); break;
		case 0x3b: dec16(); break;
		case 0x3c: inc8(); break;
		case 0x3d: dec8(); break;
		case 0x3e: ldi8(); break;
		case 0x40: ldr8(); break;
		case 0x41: ldr8(); break;
		case 0x42: ldr8(); break;
		case 0x43: ldr8(); break;
		case 0x44: ldr8(); break;
		case 0x45: ldr8(); break;
		case 0x47: ldr8(); break;
		case 0x48: ldr8(); break;
		case 0x49: ldr8(); break;
		case 0x4a: ldr8(); break;
		case 0x4b: ldr8(); break;
		case 0x4c: ldr8(); break;
		case 0x4d: ldr8(); break;
		case 0x4f: ldr8(); break;
		case 0x50: ldr8(); break;
		case 0x51: ldr8(); break;
		case 0x52: ldr8(); break;
		case 0x53: ldr8(); break;
		case 0x54: ldr8(); break;
		case 0x55: ldr8(); break;
		case 0x57: ldr8(); break;
		case 0x58: ldr8(); break;
		case 0x59: ldr8(); break;
		case 0x5a: ldr8(); break;
		case 0x5b: ldr8(); break;
		case 0x5c: ldr8(); break;
		case 0x5d: ldr8(); break;
		case 0x5f: ldr8(); break;
		case 0x60: ldr8(); break;
		case 0x61: ldr8(); break;
		case 0x62: ldr8(); break;
		case 0x63: ldr8(); break;
		case 0x64: ldr8(); break;
		case 0x65: ldr8(); break;
		case 0x67: ldr8(); break;
		case 0x68: ldr8(); break;
		case 0x69: ldr8(); break;
		case 0x6a: ldr8(); break;
		case 0x6b: ldr8(); break;
		case 0x6c: ldr8(); break;
		case 0x6d: ldr8(); break;
		case 0x6f: ldr8(); break;
		case 0x70: ldr8(); break;
		case 0x71: ldr8(); break;
		case 0x72: ldr8(); break;
		case 0x73: ldr8(); break;
		case 0x74: ldr8(); break;
		case 0x75: ldr8(); break;
		case 0x77: ldr8(); break;
		case 0x78: ldr8(); break;
		case 0x79: ldr8(); break;
		case 0x7a: ldr8(); break;
		case 0x7b: ldr8(); break;
		case 0x7c: ldr8(); break;
		case 0x7d: ldr8(); break;
		case 0x7e: ldr8(); break;
		case 0x7f: ldr8(); break;
		case 0xa0: and8(); break;
		case 0xa1: and8(); break;
		case 0xa2: and8(); break;
		case 0xa3: and8(); break;
		case 0xa4: and8(); break;
		case 0xa5: and8(); break;
		case 0xa6: and8(); break;
		case 0xa7: and8(); break;
		case 0xa8: xor8(); break;
		case 0xaf: xor8(); break;
		case 0xb8: cp(); break;
		case 0xb9: cp(); break;
		case 0xba: cp(); break;
		case 0xbb: cp(); break;
		case 0xbc: cp(); break;
		case 0xbd: cp(); break;
		case 0xbe: cp(); break;
		case 0xbf: cp(); break;
		case 0xc1: pop(); break;
		case 0xc3: jp16(); break;
		case 0xc4: call(); break;
		case 0xc5: push(); break;
		case 0xc6: addi8(); break;
		case 0xc7: rst(); break;
		case 0xcb: prefix(); break;
		case 0xcc: call(); break;
		case 0xcd: call(); break;
		case 0xcf: rst(); break;
		case 0xd1: pop(); break;
		case 0xd4: call(); break;
		case 0xd5: push(); break;
		case 0xd7: rst(); break;
		case 0xdc: call(); break;
		case 0xdf: rst(); break;
		case 0xe0: ldffi8(); break;
		case 0xe1: pop(); break;
		case 0xe2: ldr8(); break;
		case 0xe5: push(); break;
		case 0xe6: and8(); break;
		case 0xe7: rst(); break;
		case 0xe8: adds8(); break;
		case 0xea: ldr8(); break;
		case 0xef: rst(); break;
		case 0xf0: ldffi8(); break;
		case 0xf1: pop(); break;
		case 0xf2: lda8(); break;
		case 0xf5: push(); break;
		case 0xf7: rst(); break;
		case 0xf8: ldr16(); break;
		case 0xf9: ldr16(); break;
		case 0xfa: lda8(); break;
		case 0xfe: cp(); break;
		case 0xff: rst(); break;

		case 0xd3:
		case 0xdb:
		case 0xdd:
		case 0xe3:
		case 0xe4:
		case 0xeb:
		case 0xec:
		case 0xed:
		case 0xf4:
		case 0xfc:
		case 0xfd:
			print("illegal opcode\n");
			VERIFY_NOT_REACHED();
		default:
			print("opcode {:#04x} not implemented\n", opcode);
			print("immediate: {:#04x}\n", pcRead());
			VERIFY_NOT_REACHED();
		}
	}
}

// -------------------------------------

void CPU::addi8()
{
	uint8_t opcode = pcRead();
	uint8_t immediate = pcRead();
	switch (opcode) {
	case 0xc6: // ADD A,i8, flags: Z 0 H C
		m_wait_cycles += 8;

		// Set flags
		m_nf = 0;
		m_hf = isCarry(m_a, immediate, 0x10);
		m_cf = isCarry(m_a, immediate, 0x100);

		// A = A + r
		m_a = (m_a + immediate) & 0xff;

		// Zero flag
		m_zf = m_a == 0;
		break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::and8()
{
	auto bitwise_and = [this](uint32_t byte) {
		// AND r8, flags: Z 0 1 0
		m_wait_cycles += 4;

		// Set flags
		m_nf = 0;
		m_hf = 1;
		m_cf = 0;

		// Bitwise AND between the value in r8 and A
		m_a = m_a & byte;

		// Zero flag
		m_zf = m_a == 0;
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0xa0: /* AND B */ bitwise_and(m_b); break;
	case 0xa1: /* AND C */ bitwise_and(m_c); break;
	case 0xa2: /* AND D */ bitwise_and(m_d); break;
	case 0xa3: /* AND E */ bitwise_and(m_e); break;
	case 0xa4: /* AND H */ bitwise_and(m_h); break;
	case 0xa5: /* AND L */ bitwise_and(m_l); break;
	case 0xa6: /* AND (HL) */ {
		m_wait_cycles += 4; // + 4 = 8 total

		// Bitwise AND between the byte pointed to by HL and A
		bitwise_and(read(hl()));
		break;
	}
	case 0xa7: /* AND A */ bitwise_and(m_a); break;
	case 0xe6: /* AND i8 */ {
		m_wait_cycles += 4; // + 4 = 8 total

		// Bitwise AND between the value in i8 and A
		bitwise_and(pcRead());
		break;
	}
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::dec8()
{
	auto decrement = [this](uint32_t& register_) -> void {
		// DEC r8, flags: Z 1 H -
		m_wait_cycles += 4;

		// Set flags
		m_nf = 1;
		m_hf = isCarry(register_, -1, 0x10);

		// Decrement value in register r8 by 1
		register_ = (register_ - 1) & 0xff;

		// Zero flag
		m_zf = register_ == 0;
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x05: /* DEC B */ decrement(m_b); break;
	case 0x0d: /* DEC C */ decrement(m_c); break;
	case 0x15: /* DEC D */ decrement(m_d); break;
	case 0x1d: /* DEC E */ decrement(m_e); break;
	case 0x25: /* DEC H */ decrement(m_h); break;
	case 0x2d: /* DEC L */ decrement(m_l); break;
	case 0x35: /* DEC (HL) */ {
		m_wait_cycles += 8; // + 4 = 12 total

		// Decrement the byte pointed to by HL by 1
		uint32_t data = read(hl());
		decrement(data);
		write(hl(), data);
		break;
	}
	case 0x3d: /* DEC A */ decrement(m_a); break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::inc8()
{
	auto increment = [this](uint32_t& register_) -> void {
		// INC r8, flags: Z 0 H -
		m_wait_cycles += 4;

		// Set flags
		m_nf = 0;
		m_hf = isCarry(register_, 1, 0x10);

		// Increment value in register r8 by 1
		register_ = (register_ + 1) & 0xff;

		// Zero flag
		m_zf = register_ == 0;
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x04: /* INC B */ increment(m_b); break;
	case 0x0c: /* INC C */ increment(m_c); break;
	case 0x14: /* INC D */ increment(m_d); break;
	case 0x1c: /* INC E */ increment(m_e); break;
	case 0x24: /* INC H */ increment(m_h); break;
	case 0x2c: /* INC L */ increment(m_l); break;
	case 0x34: /* INC (HL) */ {
		m_wait_cycles += 8; // + 4 = 12 total

		// Increment the byte pointed to by HL by 1
		uint32_t data = read(hl());
		increment(data);
		write(hl(), data);
		break;
	}
	case 0x3c: /* INC A */ increment(m_a); break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::xor8()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0xa8: // XOR B, flags: Z 0 0 0
		m_nf = m_hf = m_cf = 0;
		m_a ^= m_b;
		m_zf = m_a == 0;
		break;
	case 0xaf: // XOR A, flags: 1 0 0 0
		// A ^ A will always be 0
		m_a = m_nf = m_hf = m_cf = 0;
		m_zf = 1;
		break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::lda8()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x2a: { // LD A,(HL+) == LD A,(HLI) == LDI A,(HL)
		m_wait_cycles += 8;

		// Put value at address in HL into A
		uint32_t address = hl();
		m_a = read(address);

		// Increment HL
		address = (address + 1) & 0xffff;
		m_l = address & 0xff;
		m_h = address >> 8;
		break;
	}
	case 0x3a: { // LD A,(HL-) == LD A,(HLD) == LDD A,(HL)
		m_wait_cycles += 8;

		// Put value at address in HL into A
		uint32_t address = hl();
		m_a = read(address);

		// Decrement HL
		address = (address + 1) & 0xffff;
		m_l = address & 0xff;
		m_h = address >> 8;
		break;
	}
	case 0xf2: // LD A,(C)
		m_wait_cycles += 8;
		m_a = read(m_c);
		break;
	case 0xfa: // LD A,a16
		m_wait_cycles += 16;
		m_a = pcRead16();
		break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::addr16()
{
	auto add = [this](uint32_t register_) -> void {
		// ADD HL,r16, flags: - 0 H C
		m_wait_cycles += 8;

		// Set flags
		m_nf = 0;
		m_hf = isCarry(hl(), register_, 0x1000);
		m_cf = isCarry(hl(), register_, 0x10000);

		// Add the value in r16 to HL
		uint32_t data = (hl() + register_) & 0xffff;
		m_l = data & 0xff;
		m_h = data >> 8;
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x09: /* ADD HL,BC */ add(bc()); break;
	case 0x19: /* ADD HL,DE */ add(de()); break;
	case 0x29: /* ADD HL,HL */ add(hl()); break;
	case 0x39: /* ADD HL,SP */ add(sp()); break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::adds8()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0xe8: { // ADD SP,s8, flags: 0 0 H C
		m_wait_cycles += 16;

		uint32_t signed_data = (pcRead() ^ 0x80) - 0x80;

		// Set flags
		m_zf = 0;
		m_nf = 0;
		m_hf = isCarry(m_sp, signed_data, 0x10);
		m_cf = isCarry(m_sp, signed_data, 0x100);

		// Add the signed value s8 to SP
		m_sp = m_sp + signed_data;
		break;
	}
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::dec16()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x0b: { // DEC BC
		uint32_t data = (bc() - 1) & 0xffff;
		m_c = data & 0xff;
		m_b = data >> 8;
		break;
	}
	case 0x1b: { // DEC DE
		uint32_t data = (de() - 1) & 0xffff;
		m_e = data & 0xff;
		m_d = data >> 8;
		break;
	}
	case 0x2b: { // DEC HL
		uint32_t data = (hl() - 1) & 0xffff;
		m_l = data & 0xff;
		m_h = data >> 8;
		break;
	}
	case 0x3b: /* DEC SP */ m_sp = (m_sp - 1) & 0xffff; break;
	default:
		VERIFY_NOT_REACHED();
	}

	m_wait_cycles += 8;
}

// -------------------------------------

void CPU::ldi8()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x06: /* LD B,i8 */ m_b = pcRead(); break;
	case 0x0e: /* LD C,i8 */ m_c = pcRead(); break;
	case 0x16: /* LD D,i8 */ m_d = pcRead(); break;
	case 0x1e: /* LD E,i8 */ m_e = pcRead(); break;
	case 0x26: /* LD H,i8 */ m_h = pcRead(); break;
	case 0x2e: /* LD L,i8 */ m_l = pcRead(); break;
	case 0x36: // LD (HL),i8
		m_wait_cycles += 4;
		write(hl(), pcRead());
		break;
	case 0x3e: /* LD A,i8 */ m_a = pcRead(); break;
	default:
		VERIFY_NOT_REACHED();
	}

	m_wait_cycles += 8;
}

void CPU::ldr8()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x02: // LD (BC),A
		m_wait_cycles += 4;
		write(bc(), m_a);
		break;
	case 0x0a: // LD A,(BC)
		m_wait_cycles += 4;
		m_a = read(bc());
		break;
	case 0x12: // LD (DE),A
		m_wait_cycles += 4;
		write(de(), m_a);
		break;
	case 0x1a: // LD A,(DE)
		m_wait_cycles += 4;
		m_a = read(de());
		break;
	case 0x22: { // LD (HL+),A == LD (HLI),A == LDI (HL),A
		m_wait_cycles += 4;

		// Put A into memory address in HL
		uint32_t address = hl();
		write(address, m_a);

		// Increment HL
		address = (address + 1) & 0xffff;
		m_l = address & 0xff;
		m_h = address >> 8;
		break;
	}
	case 0x32: { // LD (HL-),A == LD (HLD),A == LDD (HL),A
		m_wait_cycles += 4;

		// Put A into memory address in HL
		uint32_t address = hl();
		write(address, m_a);

		// Decrement HL
		address = (address - 1) & 0xffff;
		m_l = address & 0xff;
		m_h = address >> 8;
		break;
	}
	case 0x40: /* LD B,B    m_b = m_b; */ break;
	case 0x41: /* LD B,C */ m_b = m_c; break;
	case 0x42: /* LD B,D */ m_b = m_d; break;
	case 0x43: /* LD B,E */ m_b = m_e; break;
	case 0x44: /* LD B,H */ m_b = m_h; break;
	case 0x45: /* LD B,L */ m_b = m_l; break;
	case 0x47: /* LD B,A */ m_b = m_a; break;
	case 0x48: /* LD C,B */ m_c = m_b; break;
	case 0x49: /* LD C,C    m_c = m_c; */ break;
	case 0x4a: /* LD C,D */ m_c = m_d; break;
	case 0x4b: /* LD C,E */ m_c = m_e; break;
	case 0x4c: /* LD C,H */ m_c = m_h; break;
	case 0x4d: /* LD C,L */ m_c = m_l; break;
	case 0x4f: /* LD C,A */ m_c = m_a; break;
	case 0x50: /* LD D,B */ m_d = m_b; break;
	case 0x51: /* LD D,C */ m_d = m_c; break;
	case 0x52: /* LD D,D    m_d = m_d; */ break;
	case 0x53: /* LD D,E */ m_d = m_e; break;
	case 0x54: /* LD D,H */ m_d = m_h; break;
	case 0x55: /* LD D,L */ m_d = m_l; break;
	case 0x57: /* LD D,A */ m_d = m_a; break;
	case 0x58: /* LD E,B */ m_e = m_b; break;
	case 0x59: /* LD E,C */ m_e = m_c; break;
	case 0x5a: /* LD E,D */ m_e = m_d; break;
	case 0x5b: /* LD E,E    m_e = m_e; */ break;
	case 0x5c: /* LD E,H */ m_e = m_h; break;
	case 0x5d: /* LD E,L */ m_e = m_l; break;
	case 0x5f: /* LD E,A */ m_e = m_a; break;
	case 0x60: /* LD H,B */ m_h = m_b; break;
	case 0x61: /* LD H,C */ m_h = m_c; break;
	case 0x62: /* LD H,D */ m_h = m_d; break;
	case 0x63: /* LD H,E */ m_h = m_e; break;
	case 0x64: /* LD H,H    m_h = m_h; */ break;
	case 0x65: /* LD H,L */ m_h = m_l; break;
	case 0x67: /* LD H,A */ m_h = m_a; break;
	case 0x68: /* LD L,B */ m_l = m_b; break;
	case 0x69: /* LD L,C */ m_l = m_c; break;
	case 0x6a: /* LD L,D */ m_l = m_d; break;
	case 0x6b: /* LD L,E */ m_l = m_e; break;
	case 0x6c: /* LD L,H */ m_l = m_h; break;
	case 0x6d: /* LD L,L    m_l = m_l; */ break;
	case 0x6f: /* LD L,A */ m_l = m_a; break;
	case 0x70: // LD (HL),B
		m_wait_cycles += 4;
		write(hl(), m_b);
		break;
	case 0x71: // LD (HL),C
		m_wait_cycles += 4;
		write(hl(), m_c);
		break;
	case 0x72: // LD (HL),D
		m_wait_cycles += 4;
		write(hl(), m_d);
		break;
	case 0x73: // LD (HL),E
		m_wait_cycles += 4;
		write(hl(), m_e);
		break;
	case 0x74: // LD (HL),H
		m_wait_cycles += 4;
		write(hl(), m_h);
		break;
	case 0x75: // LD (HL),L
		m_wait_cycles += 4;
		write(hl(), m_l);
		break;
	case 0x77: // LD (HL),A
		m_wait_cycles += 4;
		write(hl(), m_a);
		break;
	case 0x78: /* LD A,B */ m_a = m_b; break;
	case 0x79: /* LD A,B */ m_a = m_c; break;
	case 0x7a: /* LD A,D */ m_a = m_d; break;
	case 0x7b: /* LD A,E */ m_a = m_e; break;
	case 0x7c: /* LD A,H */ m_a = m_h; break;
	case 0x7d: /* LD A,L */ m_a = m_l; break;
	case 0x7e: // LD A,(HL)
		m_wait_cycles += 4;
		m_a = read(hl());
		break;
	case 0x7f: /* LD A,A    m_a = m_a; */ break;
	case 0xe2: // LD (C),A
		m_wait_cycles += 4;
		write(m_c, m_a);
		break;
	case 0xea: // LD a16,A
		m_wait_cycles += 16;
		write(pcRead16(), m_a);
		break;
	default:
		VERIFY_NOT_REACHED();
	}

	m_wait_cycles += 4;
}

// Rotate accumulator
void CPU::ra()
{
	// Make sure we only look at the bottom 8 bits
	m_a = m_a & 0xff;

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x07: // RLCA

		// Rotates A to the left
		//     ┌──────────────┐
		//     │ ┌─────────┐  │
		// C <─┴─│7  <──  0│<─┘
		//       └─────────┘
		//            A

		// Copy bit 7 into carry flag
		m_cf = (m_a & 0x80) == 0x80;

		// Rotate register A left
		m_a = (m_a >> 7) | (m_a << 1);
		break;
	case 0x0f: // RRCA

		// Rotates A to the right
		// ┌──────────────┐
		// │  ┌─────────┐ │
		// └─>│7  ──>  0│─┴─> C
		//    └─────────┘
		//         A

		// Copy bit 0 into carry flag
		m_cf = (m_a & 0x1) == 0x1;

		// Rotate register A right
		m_a = (m_a >> 1) | (m_a << 7);
		break;
	case 0x17: { // RLA

		// Rotate register A left through carry
		// ┌────────────────────┐
		// │       ┌─────────┐  │
		// └─ C <──│7  <──  0│<─┘
		//         └─────────┘
		//              A

		// Copy bit 7 into carry flag
		uint32_t old_carry = m_cf != 0;
		m_cf = (m_a & 0x80) == 0x80;

		// Rotate register A left through carry
		m_a = old_carry | (m_a << 1);
		break;
	}
	case 0x1f: { // RRA

		// Rotate register A right through carry
		// ┌────────────────────┐
		// │  ┌─────────┐       │
		// └─>│7  ──>  0│──> C ─┘
		//    └─────────┘
		//         A

		// Copy bit 0 into carry flag
		uint32_t old_carry = m_cf != 0;
		m_cf = (m_a & 0x1) == 0x1;

		// Rotate register A right through carry
		m_a = (m_a >> 1) | (old_carry << 7);
		break;
	}
	default:
		VERIFY_NOT_REACHED();
	}

	// RLCA/RRCA/RLA/RRA, Flags: 0 0 0 C
	m_wait_cycles += 4;

	// Set flags
	m_zf = 0;
	m_nf = 0;
	m_hf = 0;
}

void CPU::cp()
{
	uint8_t opcode = pcRead();
	uint8_t value = 0;
	switch (opcode) {
	case 0xb8: /* CP B */ value = m_b; break;
	case 0xb9: /* CP C */ value = m_c; break;
	case 0xba: /* CP D */ value = m_d; break;
	case 0xbb: /* CP E */ value = m_e; break;
	case 0xbc: /* CP H */ value = m_h; break;
	case 0xbd: /* CP L */ value = m_l; break;
	case 0xbe: // CP,(HL)
		m_wait_cycles += 4;
		value = read(hl());
		break;
	case 0xbf: // CP A, flags: 1 1 0 0
		m_wait_cycles += 4;
		// A - A
		m_zf = m_nf = 1;
		m_hf = m_cf = 0;
		return; // Early return as we already set the flags
	case 0xfe:  // CP i8
		m_wait_cycles += 4;
		value = pcRead();
		break;
	default:
		VERIFY_NOT_REACHED();
	}

	// CP r8, flags: Z 1 H C
	m_wait_cycles += 4;

	// Set flags
	m_zf = ((m_a - value) & 0xff) == 0;
	m_nf = 1;
	m_hf = isCarry(m_a, value, 0x10);
	m_cf = isCarry(m_a, value, 0x100);
}

void CPU::ldffi8()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0xe0: // LD ($ff00 + i8),A == LDH (io8),A
		m_wait_cycles += 12;

		// Put value in A into address (0xff00 + next byte in memory)
		ffWrite(pcRead(), m_a);
		break;
	case 0xf0: // LD A,($ff00 + i8) == LDH A,(io8)
		m_wait_cycles += 12;

		// Put value at address (0xff00 + next byte in memory) into A
		m_a = ffRead(pcRead());
		break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::ldi16()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x01: /* LD BC,i16 */
		m_c = pcRead();
		m_b = pcRead();
		break;
	case 0x11: /* LD DE,i16 */
		m_e = pcRead();
		m_d = pcRead();
		break;
	case 0x21: /* LD HL,i16 */
		m_l = pcRead();
		m_h = pcRead();
		break;
	case 0x31: /* LD SP,i16 */
		m_sp = pcRead16();
		break;
	default:
		VERIFY_NOT_REACHED();
	}

	m_wait_cycles += 12;
}

void CPU::ldr16()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x08: { // LD a16,SP
		m_wait_cycles += 20;

		// Put value of SP into address given by next 2 bytes in memory
		write(pcRead16(), m_sp);
		break;
	}
	case 0xf8: { // LD HL,SP + s8 == LDHL SP,s8, flags: 0 0 H C
		m_wait_cycles += 12;

		// Put SP + next (signed) byte in memory into HL
		uint32_t signed_data = (pcRead() ^ 0x80) - 0x80;
		uint32_t sum = m_sp + signed_data;
		m_h = sum >> 8;
		m_l = sum & 0xff;

		// Set flags
		m_zf = 0;
		m_nf = 0;
		m_hf = isCarry(m_sp, signed_data, 0x10);
		m_cf = isCarry(m_sp, signed_data, 0x100);
		break;
	}
	case 0xf9: { // LD SP,HL
		m_wait_cycles += 8;
		m_sp = hl();
		break;
	}
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::pop()
{
	auto pop_stack = [this](uint32_t& register_high, uint32_t& register_low) -> void {
		// POP r16
		m_wait_cycles += 12;

		// Pop register r16 from stack
		register_low = read(m_sp);
		m_sp = (m_sp + 1) & 0xffff;
		register_high = read(m_sp);
		m_sp = (m_sp + 1) & 0xffff;
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0xc1: /* POP BC */ pop_stack(m_b, m_c); break;
	case 0xd1: /* POP DE */ pop_stack(m_d, m_e); break;
	case 0xe1: /* POP HL */ pop_stack(m_h, m_l); break;
	case 0xf1: /* POP AF, flags: Z N H C */ {
		uint32_t data;
		pop_stack(m_a, data);

		// Set flags
		m_zf = (data & 0x80) == 0x80; // bit 7 of the popped low byte
		m_nf = (data & 0x40) == 0x40; // bit 6 ,,
		m_hf = (data & 0x20) == 0x20; // bit 5 ,,
		m_cf = (data & 0x10) == 0x10; // bit 4 ,,
		break;
	}
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::push()
{
	auto push_into_stack = [this](uint32_t register_) -> void {
		// PUSH r16
		m_wait_cycles += 16;

		// Push register r16 into stack
		m_sp = (m_sp - 1) & 0xffff;
		write(m_sp, register_ >> 8);
		m_sp = (m_sp - 1) & 0xffff;
		write(m_sp, register_ & 0xff);
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0xc5: /* PUSH BC */ push_into_stack(bc()); break;
	case 0xd5: /* PUSH DE */ push_into_stack(de()); break;
	case 0xe5: /* PUSH HL */ push_into_stack(hl()); break;
	case 0xf5: /* PUSH AF */ push_into_stack(af()); break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::call()
{
	auto function_call = [this](bool should_call) -> void {
		// CALL cc,i16

		// Note: the operand is read even when the condition is false
		uint32_t data = pcRead16();

		if (!should_call) {
			m_wait_cycles += 12;
		}
		m_wait_cycles += 24;

		// Push address of next 2 bytes in memory onto stack
		m_sp = (m_sp - 1) & 0xffff;
		write(m_sp, data >> 8);
		m_sp = (m_sp - 1) & 0xffff;
		write(m_sp, data & 0xff);

		// Jump to this address
		m_pc = data;
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0xc4: /* CALL NZ,i16 */ function_call(!m_nf); break;
	case 0xcc: /* CALL Z,i16 */ function_call(m_nf); break;
	case 0xcd: /* CALL i16 */ function_call(true); break;
	case 0xd4: /* CALL NC,i16 */ function_call(!m_cf); break;
	case 0xdc: /* CALL C,i16 */ function_call(m_cf); break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::jp16()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0xc3: // JP a16
		m_wait_cycles += 16;
		m_pc = pcRead16();
		break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::jrs8()
{
	auto jump_relative = [this](bool should_jump) -> void {
		// JR cc,s8

		// Note: the operand is read even when the condition is false
		uint32_t signed_data = (pcRead() ^ 0x80) - 0x80;

		if (!should_jump) {
			m_wait_cycles += 8;
			return;
		}
		m_wait_cycles += 12;

		// Relative jump by adding s8 to the address of the instruction following the JR
		m_pc = m_pc + signed_data;
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x18: /* JR s8 */ jump_relative(true); break;
	case 0x20: /* JR NZ,s8 */ jump_relative(!m_zf); break;
	case 0x28: /* JR Z,s8 */ jump_relative(m_zf); break;
	case 0x30: /* JR NC,s8 */ jump_relative(!m_cf); break;
	case 0x38: /* JR C,s8 */ jump_relative(m_cf); break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::rst()
{
	auto function_call = [this](uint32_t fixed_address) -> void {
		// RST vec
		m_wait_cycles += 16;

		// Push present address onto stack
		m_sp = (m_sp - 1) & 0xffff;
		write(m_sp, m_pc >> 8);
		m_sp = (m_sp - 1) & 0xffff;
		write(m_sp, m_pc & 0xff);

		// Jump to this address
		m_pc = fixed_address;
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0xc7: /* RST 0x00 */ function_call(0x00); break;
	case 0xcf: /* RST 0x08 */ function_call(0x08); break;
	case 0xd7: /* RST 0x10 */ function_call(0x10); break;
	case 0xdf: /* RST 0x18 */ function_call(0x18); break;
	case 0xe7: /* RST 0x20 */ function_call(0x20); break;
	case 0xef: /* RST 0x28 */ function_call(0x28); break;
	case 0xf7: /* RST 0x30 */ function_call(0x30); break;
	case 0xff: /* RST 0x38 */ function_call(0x38); break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::misc()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x2f: // CPL, flags: - 1 1 -
		m_wait_cycles += 4;

		// Complement register A (flip all bits)
		m_a ^= 0xff; // equivalent to: m_a = ~m_a & 0xff

		// Set flags
		m_nf = m_hf = 1;
		break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::nop()
{
	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x0: /* NOP */ m_wait_cycles += 4; break;
	default:
		VERIFY_NOT_REACHED();
	}
}

// -----------------------------------------

uint32_t CPU::pcRead()
{
	uint32_t data = Emu::the().readMemory(m_pc) & 0xff;
	m_pc = (m_pc + 1) & 0xffff;
	return data;
}

void CPU::write(uint32_t address, uint32_t value)
{
	Emu::the().writeMemory(address, value & 0xff);
}

uint32_t CPU::read(uint32_t address)
{
	return Emu::the().readMemory(address) & 0xff;
}

void CPU::ffWrite(uint32_t address, uint32_t value)
{
	Emu::the().writeMemory(address | (0xff << 8), value & 0xff);
}

uint32_t CPU::ffRead(uint32_t address)
{
	return Emu::the().readMemory(address | (0xff << 8)) & 0xff;
}

bool CPU::isCarry(uint32_t lhs, uint32_t rhs, uint32_t limit_bit)
{
	// limit_bit values:
	//  8-bit half-carry = 0x10    or 16
	//  8-bit carry      = 0x100   or 256
	// 16-bit half-carry = 0x1000  or 4096
	// 16-bit carry      = 0x10000 or 65536

	// Example for 8-bit half-carry:
	// 0b00111110 62      | 0b00111000 56
	// 0b00100010 34 +    | 0b00010001 17 +
	// ---------------    | ---------------
	// 0b01100000 96      | 0b01001001 73
	//                    |
	// 0b00111110 62      | 0b00111000 56
	// 0b00100010 34 ^    | 0b00010001 17 ^
	// ---------------    | ---------------
	// 0b00011100         | 0b00101001
	// 0b01100000 96 ^    | 0b01001001 73 ^
	// ---------------    | ---------------
	// 0b01111100         | 0b01100000
	// 0b00010000 16 &    | 0b00010000 16 &
	// ---------------    | ---------------
	// 0b00010000 = true! | 0b00000000 = false!

	return (lhs ^ rhs ^ (lhs + rhs)) & limit_bit;
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
