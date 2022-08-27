/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint> // uint32_t

#include "cpu.h"
#include "ruc/format/print.h"
#include "ruc/meta/assert.h"

void CPU::prefix()
{
	// Note: All these opcodes are considered 2 bytes, as the prefix is included

	// Skip 0xcb prefix opcode
	m_pc = (m_pc + 1) & 0xffff;

	// Read next opcode
	uint8_t opcode = read(m_pc);
	print("running opcode: {:#04x} @ ({:#06x})\n", opcode, m_pc);
	if (opcode >= 0x00 && opcode <= 0x07) {
		rlc();
	}
	else if (opcode >= 0x08 && opcode <= 0x0f) {
		rrc();
	}
	else if (opcode >= 0x10 && opcode <= 0x17) {
		rl();
	}
	else if (opcode >= 0x18 && opcode <= 0x1f) {
		rr();
	}
	else if (opcode >= 0x20 && opcode <= 0x27) {
		sla();
	}
	else if (opcode >= 0x28 && opcode <= 0x2f) {
		sra();
	}
	else if (opcode >= 0x30 && opcode <= 0x37) {
		swap();
	}
	else if (opcode >= 0x38 && opcode <= 0x3f) {
		srl();
	}
	else if (opcode >= 0x40 && opcode <= 0x7f) {
		bit();
	}
	else if (opcode >= 0x80 && opcode <= 0xbf) {
		res();
	}
	else if (opcode >= 0xc0 && opcode <= 0xff) {
		set();
	}
	else {
		print("opcode {:#04x} not implemented\n", opcode);
		print("immediate: {:#04x}\n", pcRead());
		VERIFY_NOT_REACHED();
	}
}

void CPU::bit()
{
	auto test_bit = [this](uint32_t bit, uint32_t byte) -> void {
		// BIT b,r8, flags: Z 0 1 -
		m_wait_cycles += 8;

		// Set flags
		// Set zero flag if bit at position 'x' of register r8 is 0
		m_zf = (byte & bit) == 0x0;
		m_nf = 0;
		m_hf = 1;
	};

	auto test_bit_hl = [this](uint32_t bit) -> void {
		// BIT b,(HL), flags: Z 0 1 -
		m_wait_cycles += 12;

		// Set flags
		// Set zero flag if bit at position 'x' of byte pointed by HL is 0
		m_zf = (read(hl()) & bit) == 0x0;
		m_nf = 0;
		m_hf = 1;
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x40: /* BIT 0,B */ test_bit(0x01, m_b); break;
	case 0x41: /* BIT 0,C */ test_bit(0x01, m_c); break;
	case 0x42: /* BIT 0,D */ test_bit(0x01, m_d); break;
	case 0x43: /* BIT 0,E */ test_bit(0x01, m_e); break;
	case 0x44: /* BIT 0,H */ test_bit(0x01, m_h); break;
	case 0x45: /* BIT 0,L */ test_bit(0x01, m_l); break;
	case 0x46: /* BIT 0,(HL) */ test_bit_hl(0x01); break;
	case 0x47: /* BIT 0,A */ test_bit(0x01, m_a); break;

	case 0x48: /* BIT 1,B */ test_bit(0x02, m_b); break;
	case 0x49: /* BIT 1,C */ test_bit(0x02, m_c); break;
	case 0x4a: /* BIT 1,D */ test_bit(0x02, m_d); break;
	case 0x4b: /* BIT 1,E */ test_bit(0x02, m_e); break;
	case 0x4c: /* BIT 1,H */ test_bit(0x02, m_h); break;
	case 0x4d: /* BIT 1,L */ test_bit(0x02, m_l); break;
	case 0x4e: /* BIT 1,(HL) */ test_bit_hl(0x02); break;
	case 0x4f: /* BIT 1,A */ test_bit(0x02, m_a); break;

	case 0x50: /* BIT 2,B */ test_bit(0x04, m_b); break;
	case 0x51: /* BIT 2,C */ test_bit(0x04, m_c); break;
	case 0x52: /* BIT 2,D */ test_bit(0x04, m_d); break;
	case 0x53: /* BIT 2,E */ test_bit(0x04, m_e); break;
	case 0x54: /* BIT 2,H */ test_bit(0x04, m_h); break;
	case 0x55: /* BIT 2,L */ test_bit(0x04, m_l); break;
	case 0x56: /* BIT 2,(HL) */ test_bit_hl(0x04); break;
	case 0x57: /* BIT 2,A */ test_bit(0x04, m_a); break;

	case 0x58: /* BIT 3,B */ test_bit(0x08, m_b); break;
	case 0x59: /* BIT 3,C */ test_bit(0x08, m_c); break;
	case 0x5a: /* BIT 3,D */ test_bit(0x08, m_d); break;
	case 0x5b: /* BIT 3,E */ test_bit(0x08, m_e); break;
	case 0x5c: /* BIT 3,H */ test_bit(0x08, m_h); break;
	case 0x5d: /* BIT 3,L */ test_bit(0x08, m_l); break;
	case 0x5e: /* BIT 3,(HL) */ test_bit_hl(0x08); break;
	case 0x5f: /* BIT 3,A */ test_bit(0x08, m_a); break;

	case 0x60: /* BIT 4,B */ test_bit(0x10, m_b); break;
	case 0x61: /* BIT 4,C */ test_bit(0x10, m_c); break;
	case 0x62: /* BIT 4,D */ test_bit(0x10, m_d); break;
	case 0x63: /* BIT 4,E */ test_bit(0x10, m_e); break;
	case 0x64: /* BIT 4,H */ test_bit(0x10, m_h); break;
	case 0x65: /* BIT 4,L */ test_bit(0x10, m_l); break;
	case 0x66: /* BIT 4,(HL) */ test_bit_hl(0x10); break;
	case 0x67: /* BIT 4,A */ test_bit(0x10, m_a); break;

	case 0x68: /* BIT 5,B */ test_bit(0x20, m_b); break;
	case 0x69: /* BIT 5,C */ test_bit(0x20, m_c); break;
	case 0x6a: /* BIT 5,D */ test_bit(0x20, m_d); break;
	case 0x6b: /* BIT 5,E */ test_bit(0x20, m_e); break;
	case 0x6c: /* BIT 5,H */ test_bit(0x20, m_h); break;
	case 0x6d: /* BIT 5,L */ test_bit(0x20, m_l); break;
	case 0x6e: /* BIT 5,(HL) */ test_bit_hl(0x20); break;
	case 0x6f: /* BIT 5,A */ test_bit(0x20, m_a); break;

	case 0x70: /* BIT 6,B */ test_bit(0x40, m_b); break;
	case 0x71: /* BIT 6,C */ test_bit(0x40, m_c); break;
	case 0x72: /* BIT 6,D */ test_bit(0x40, m_d); break;
	case 0x73: /* BIT 6,E */ test_bit(0x40, m_e); break;
	case 0x74: /* BIT 6,H */ test_bit(0x40, m_h); break;
	case 0x75: /* BIT 6,L */ test_bit(0x40, m_l); break;
	case 0x76: /* BIT 6,(HL) */ test_bit_hl(0x40); break;
	case 0x77: /* BIT 6,A */ test_bit(0x40, m_a); break;

	case 0x78: /* BIT 7,B */ test_bit(0x80, m_b); break;
	case 0x79: /* BIT 7,C */ test_bit(0x80, m_c); break;
	case 0x7a: /* BIT 7,D */ test_bit(0x80, m_d); break;
	case 0x7b: /* BIT 7,E */ test_bit(0x80, m_e); break;
	case 0x7c: /* BIT 7,H */ test_bit(0x80, m_h); break;
	case 0x7d: /* BIT 7,L */ test_bit(0x80, m_l); break;
	case 0x7e: /* BIT 7,(HL) */ test_bit_hl(0x80); break;
	case 0x7f: /* BIT 7,A */ test_bit(0x80, m_a); break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::res()
{
	auto reset_bit = [this](uint32_t bit, uint32_t& register_) -> void {
		// RES b,r8
		m_wait_cycles += 8;

		// Reset bit at position 'x' in register r8
		register_ = register_ & (~bit);
	};

	auto reset_bit_hl = [this](uint32_t bit) -> void {
		// RES b,(HL)
		m_wait_cycles += 16;

		// Set bit at postition 'x' in the byte pointed by HL to 0
		uint32_t data = read(hl());
		data = data & (~bit);
		write(hl(), data);
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x40: /* RES 0,B */ reset_bit(0x01, m_b); break;
	case 0x41: /* RES 0,C */ reset_bit(0x01, m_c); break;
	case 0x42: /* RES 0,D */ reset_bit(0x01, m_d); break;
	case 0x43: /* RES 0,E */ reset_bit(0x01, m_e); break;
	case 0x44: /* RES 0,H */ reset_bit(0x01, m_h); break;
	case 0x45: /* RES 0,L */ reset_bit(0x01, m_l); break;
	case 0x46: /* RES 0,(HL) */ reset_bit_hl(0x01); break;
	case 0x47: /* RES 0,A */ reset_bit(0x01, m_a); break;

	case 0x48: /* RES 1,B */ reset_bit(0x02, m_b); break;
	case 0x49: /* RES 1,C */ reset_bit(0x02, m_c); break;
	case 0x4a: /* RES 1,D */ reset_bit(0x02, m_d); break;
	case 0x4b: /* RES 1,E */ reset_bit(0x02, m_e); break;
	case 0x4c: /* RES 1,H */ reset_bit(0x02, m_h); break;
	case 0x4d: /* RES 1,L */ reset_bit(0x02, m_l); break;
	case 0x4e: /* RES 1,(HL) */ reset_bit_hl(0x02); break;
	case 0x4f: /* RES 1,A */ reset_bit(0x02, m_a); break;

	case 0x50: /* RES 2,B */ reset_bit(0x04, m_b); break;
	case 0x51: /* RES 2,C */ reset_bit(0x04, m_c); break;
	case 0x52: /* RES 2,D */ reset_bit(0x04, m_d); break;
	case 0x53: /* RES 2,E */ reset_bit(0x04, m_e); break;
	case 0x54: /* RES 2,H */ reset_bit(0x04, m_h); break;
	case 0x55: /* RES 2,L */ reset_bit(0x04, m_l); break;
	case 0x56: /* RES 2,(HL) */ reset_bit_hl(0x04); break;
	case 0x57: /* RES 2,A */ reset_bit(0x04, m_a); break;

	case 0x58: /* RES 3,B */ reset_bit(0x08, m_b); break;
	case 0x59: /* RES 3,C */ reset_bit(0x08, m_c); break;
	case 0x5a: /* RES 3,D */ reset_bit(0x08, m_d); break;
	case 0x5b: /* RES 3,E */ reset_bit(0x08, m_e); break;
	case 0x5c: /* RES 3,H */ reset_bit(0x08, m_h); break;
	case 0x5d: /* RES 3,L */ reset_bit(0x08, m_l); break;
	case 0x5e: /* RES 3,(HL) */ reset_bit_hl(0x08); break;
	case 0x5f: /* RES 3,A */ reset_bit(0x08, m_a); break;

	case 0x60: /* RES 4,B */ reset_bit(0x10, m_b); break;
	case 0x61: /* RES 4,C */ reset_bit(0x10, m_c); break;
	case 0x62: /* RES 4,D */ reset_bit(0x10, m_d); break;
	case 0x63: /* RES 4,E */ reset_bit(0x10, m_e); break;
	case 0x64: /* RES 4,H */ reset_bit(0x10, m_h); break;
	case 0x65: /* RES 4,L */ reset_bit(0x10, m_l); break;
	case 0x66: /* RES 4,(HL) */ reset_bit_hl(0x10); break;
	case 0x67: /* RES 4,A */ reset_bit(0x10, m_a); break;

	case 0x68: /* RES 5,B */ reset_bit(0x20, m_b); break;
	case 0x69: /* RES 5,C */ reset_bit(0x20, m_c); break;
	case 0x6a: /* RES 5,D */ reset_bit(0x20, m_d); break;
	case 0x6b: /* RES 5,E */ reset_bit(0x20, m_e); break;
	case 0x6c: /* RES 5,H */ reset_bit(0x20, m_h); break;
	case 0x6d: /* RES 5,L */ reset_bit(0x20, m_l); break;
	case 0x6e: /* RES 5,(HL) */ reset_bit_hl(0x20); break;
	case 0x6f: /* RES 5,A */ reset_bit(0x20, m_a); break;

	case 0x70: /* RES 6,B */ reset_bit(0x40, m_b); break;
	case 0x71: /* RES 6,C */ reset_bit(0x40, m_c); break;
	case 0x72: /* RES 6,D */ reset_bit(0x40, m_d); break;
	case 0x73: /* RES 6,E */ reset_bit(0x40, m_e); break;
	case 0x74: /* RES 6,H */ reset_bit(0x40, m_h); break;
	case 0x75: /* RES 6,L */ reset_bit(0x40, m_l); break;
	case 0x76: /* RES 6,(HL) */ reset_bit_hl(0x40); break;
	case 0x77: /* RES 6,A */ reset_bit(0x40, m_a); break;

	case 0x78: /* RES 7,B */ reset_bit(0x80, m_b); break;
	case 0x79: /* RES 7,C */ reset_bit(0x80, m_c); break;
	case 0x7a: /* RES 7,D */ reset_bit(0x80, m_d); break;
	case 0x7b: /* RES 7,E */ reset_bit(0x80, m_e); break;
	case 0x7c: /* RES 7,H */ reset_bit(0x80, m_h); break;
	case 0x7d: /* RES 7,L */ reset_bit(0x80, m_l); break;
	case 0x7e: /* RES 7,(HL) */ reset_bit_hl(0x80); break;
	case 0x7f: /* RES 7,A */ reset_bit(0x80, m_a); break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::set()
{
	auto set_bit = [this](uint32_t bit, uint32_t& register_) -> void {
		// RES b,r8
		m_wait_cycles += 8;

		// Set bit at position 'x' in register r8
		register_ = register_ | bit;
	};

	auto set_bit_hl = [this](uint32_t bit) -> void {
		// RES b,(HL)
		m_wait_cycles += 16;

		// Set bit at postition 'x' in the byte pointed by HL to 0
		uint32_t data = read(hl());
		data = data | bit;
		write(hl(), data);
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x40: /* SET 0,B */ set_bit(0x01, m_b); break;
	case 0x41: /* SET 0,C */ set_bit(0x01, m_c); break;
	case 0x42: /* SET 0,D */ set_bit(0x01, m_d); break;
	case 0x43: /* SET 0,E */ set_bit(0x01, m_e); break;
	case 0x44: /* SET 0,H */ set_bit(0x01, m_h); break;
	case 0x45: /* SET 0,L */ set_bit(0x01, m_l); break;
	case 0x46: /* SET 0,(HL) */ set_bit_hl(0x01); break;
	case 0x47: /* SET 0,A */ set_bit(0x01, m_a); break;

	case 0x48: /* SET 1,B */ set_bit(0x02, m_b); break;
	case 0x49: /* SET 1,C */ set_bit(0x02, m_c); break;
	case 0x4a: /* SET 1,D */ set_bit(0x02, m_d); break;
	case 0x4b: /* SET 1,E */ set_bit(0x02, m_e); break;
	case 0x4c: /* SET 1,H */ set_bit(0x02, m_h); break;
	case 0x4d: /* SET 1,L */ set_bit(0x02, m_l); break;
	case 0x4e: /* SET 1,(HL) */ set_bit_hl(0x02); break;
	case 0x4f: /* SET 1,A */ set_bit(0x02, m_a); break;

	case 0x50: /* SET 2,B */ set_bit(0x04, m_b); break;
	case 0x51: /* SET 2,C */ set_bit(0x04, m_c); break;
	case 0x52: /* SET 2,D */ set_bit(0x04, m_d); break;
	case 0x53: /* SET 2,E */ set_bit(0x04, m_e); break;
	case 0x54: /* SET 2,H */ set_bit(0x04, m_h); break;
	case 0x55: /* SET 2,L */ set_bit(0x04, m_l); break;
	case 0x56: /* SET 2,(HL) */ set_bit_hl(0x04); break;
	case 0x57: /* SET 2,A */ set_bit(0x04, m_a); break;

	case 0x58: /* SET 3,B */ set_bit(0x08, m_b); break;
	case 0x59: /* SET 3,C */ set_bit(0x08, m_c); break;
	case 0x5a: /* SET 3,D */ set_bit(0x08, m_d); break;
	case 0x5b: /* SET 3,E */ set_bit(0x08, m_e); break;
	case 0x5c: /* SET 3,H */ set_bit(0x08, m_h); break;
	case 0x5d: /* SET 3,L */ set_bit(0x08, m_l); break;
	case 0x5e: /* SET 3,(HL) */ set_bit_hl(0x08); break;
	case 0x5f: /* SET 3,A */ set_bit(0x08, m_a); break;

	case 0x60: /* SET 4,B */ set_bit(0x10, m_b); break;
	case 0x61: /* SET 4,C */ set_bit(0x10, m_c); break;
	case 0x62: /* SET 4,D */ set_bit(0x10, m_d); break;
	case 0x63: /* SET 4,E */ set_bit(0x10, m_e); break;
	case 0x64: /* SET 4,H */ set_bit(0x10, m_h); break;
	case 0x65: /* SET 4,L */ set_bit(0x10, m_l); break;
	case 0x66: /* SET 4,(HL) */ set_bit_hl(0x10); break;
	case 0x67: /* SET 4,A */ set_bit(0x10, m_a); break;

	case 0x68: /* SET 5,B */ set_bit(0x20, m_b); break;
	case 0x69: /* SET 5,C */ set_bit(0x20, m_c); break;
	case 0x6a: /* SET 5,D */ set_bit(0x20, m_d); break;
	case 0x6b: /* SET 5,E */ set_bit(0x20, m_e); break;
	case 0x6c: /* SET 5,H */ set_bit(0x20, m_h); break;
	case 0x6d: /* SET 5,L */ set_bit(0x20, m_l); break;
	case 0x6e: /* SET 5,(HL) */ set_bit_hl(0x20); break;
	case 0x6f: /* SET 5,A */ set_bit(0x20, m_a); break;

	case 0x70: /* SET 6,B */ set_bit(0x40, m_b); break;
	case 0x71: /* SET 6,C */ set_bit(0x40, m_c); break;
	case 0x72: /* SET 6,D */ set_bit(0x40, m_d); break;
	case 0x73: /* SET 6,E */ set_bit(0x40, m_e); break;
	case 0x74: /* SET 6,H */ set_bit(0x40, m_h); break;
	case 0x75: /* SET 6,L */ set_bit(0x40, m_l); break;
	case 0x76: /* SET 6,(HL) */ set_bit_hl(0x40); break;
	case 0x77: /* SET 6,A */ set_bit(0x40, m_a); break;

	case 0x78: /* SET 7,B */ set_bit(0x80, m_b); break;
	case 0x79: /* SET 7,C */ set_bit(0x80, m_c); break;
	case 0x7a: /* SET 7,D */ set_bit(0x80, m_d); break;
	case 0x7b: /* SET 7,E */ set_bit(0x80, m_e); break;
	case 0x7c: /* SET 7,H */ set_bit(0x80, m_h); break;
	case 0x7d: /* SET 7,L */ set_bit(0x80, m_l); break;
	case 0x7e: /* SET 7,(HL) */ set_bit_hl(0x80); break;
	case 0x7f: /* SET 7,A */ set_bit(0x80, m_a); break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::swap()
{
	auto swap_bits = [this](uint32_t& register_) -> void {
		// SWAP r8, flags: Z 0 0 0
		m_wait_cycles += 8;

		// Swap upper 4 bits in register r8 with lower 4 bits
		register_ = register_ & 0xff;
		register_ = (register_ >> 4) | (register_ << 4);

		// Set flags
		m_zf = register_ == 0;
		m_nf = 0;
		m_hf = 0;
		m_cf = 0;
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x78: /* SWAP B */ swap_bits(m_b); break;
	case 0x79: /* SWAP C */ swap_bits(m_c); break;
	case 0x7a: /* SWAP D */ swap_bits(m_d); break;
	case 0x7b: /* SWAP E */ swap_bits(m_e); break;
	case 0x7c: /* SWAP H */ swap_bits(m_h); break;
	case 0x7d: /* SWAP L */ swap_bits(m_l); break;
	case 0x7e: /* SWAP (HL) */ {
		m_wait_cycles += 8; // + 8 = 16 total

		// Swap upper 4 bits in the byte pointed by HL with lower 4 bits
		uint32_t data = read(hl());
		swap_bits(data);
		write(hl(), data);
		break;
	}
	case 0x7f: /* SWAP A */ swap_bits(m_a); break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::rl()
{
	auto rotate_left_carry = [this](uint32_t& register_) -> void {
		// RL r8, flags: Z 0 0 C
		m_wait_cycles += 8;

		// Copy bit 7 into carry flag
		uint32_t old_carry = m_cf != 0;
		m_cf = (register_ & 0x80) == 0x80;

		// Rotate register r8 left through carry
		register_ = (old_carry | (register_ << 1)) & 0xff;

		// Set other flags
		m_zf = register_ == 0;
		m_nf = 0;
		m_hf = 0;
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x10: /* RL B */ rotate_left_carry(m_b); break;
	case 0x11: /* RL C */ rotate_left_carry(m_c); break;
	case 0x12: /* RL D */ rotate_left_carry(m_d); break;
	case 0x13: /* RL E */ rotate_left_carry(m_e); break;
	case 0x14: /* RL H */ rotate_left_carry(m_h); break;
	case 0x15: /* RL L */ rotate_left_carry(m_l); break;
	case 0x16: /* RL (HL) */ {
		m_wait_cycles += 8; // + 8 = 16 total

		uint32_t data = read(hl());
		rotate_left_carry(data);
		write(hl(), data);
		break;
	}
	case 0x17: /* RL A */ rotate_left_carry(m_a); break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::rlc()
{
	auto rotate_left = [this](uint32_t& register_) -> void {
		// RLC r8, flags: Z 0 0 C
		m_wait_cycles += 8;

		// Copy bit 7 into carry flag
		m_cf = (register_ & 0x80) == 0x80;

		// Rotate register r8 left
		register_ = ((register_ >> 7) | (register_ << 1)) & 0xff;

		// Set other flags
		m_zf = register_ == 0;
		m_nf = 0;
		m_hf = 0;
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x00: /* RLC B */ rotate_left(m_b); break;
	case 0x01: /* RLC C */ rotate_left(m_c); break;
	case 0x02: /* RLC D */ rotate_left(m_d); break;
	case 0x03: /* RLC E */ rotate_left(m_e); break;
	case 0x04: /* RLC H */ rotate_left(m_h); break;
	case 0x05: /* RLC L */ rotate_left(m_l); break;
	case 0x06: /* RLC (HL) */ {
		m_wait_cycles += 8; // + 8 = 16 total

		// Rotate the byte pointed to by HL left
		uint32_t data = read(hl());
		rotate_left(data);
		write(hl(), data);
		break;
	}
	case 0x07: /* RLC A */ rotate_left(m_a); break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::rr()
{
	auto rotate_right_carry = [this](uint32_t& register_) -> void {
		// RR r8, flags: Z 0 0 C
		m_wait_cycles += 8;

		// Copy bit 0 into carry flag
		uint32_t old_carry = m_cf != 0;
		m_cf = (register_ & 0x01) == 0x01;

		// Rotate register r8 right through carry
		register_ = ((register_ >> 1) | (old_carry << 7)) & 0xff;

		// Set other flags
		m_zf = register_ == 0;
		m_nf = 0;
		m_hf = 0;
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x18: /* RR B */ rotate_right_carry(m_b); break;
	case 0x19: /* RR C */ rotate_right_carry(m_c); break;
	case 0x1a: /* RR D */ rotate_right_carry(m_d); break;
	case 0x1b: /* RR E */ rotate_right_carry(m_e); break;
	case 0x1c: /* RR H */ rotate_right_carry(m_h); break;
	case 0x1d: /* RR L */ rotate_right_carry(m_l); break;
	case 0x1e: /* RR (HL) */ {
		m_wait_cycles += 8; // + 8 = 16 total

		uint32_t data = read(hl());
		rotate_right_carry(data);
		write(hl(), data);
		break;
	}
	case 0x1f: /* RR A */ rotate_right_carry(m_a); break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::rrc()
{
	auto rotate_right = [this](uint32_t& register_) -> void {
		// RRC r8, flags: Z 0 0 C
		m_wait_cycles += 8;

		// Copy bit 0 into carry flag
		m_cf = (register_ & 0x1) == 0x1;

		// Rotate register r8 right
		register_ = ((register_ >> 1) | (register_ << 7)) & 0xff;

		// Set other flags
		m_zf = register_ == 0;
		m_nf = 0;
		m_hf = 0;
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x08: /* RRC B */ rotate_right(m_b); break;
	case 0x09: /* RRC C */ rotate_right(m_c); break;
	case 0x0a: /* RRC D */ rotate_right(m_d); break;
	case 0x0b: /* RRC E */ rotate_right(m_e); break;
	case 0x0c: /* RRC H */ rotate_right(m_h); break;
	case 0x0d: /* RRC L */ rotate_right(m_l); break;
	case 0x0e: /* RRC (HL) */ {
		m_wait_cycles += 8; // + 8 = 16 total

		// Rotate the byte pointed to by HL right
		uint32_t data = read(hl());
		rotate_right(data);
		write(hl(), data);
		break;
	}
	case 0x0f: /* RRC A */ rotate_right(m_a); break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::sla()
{
	auto shift_left_arithmatically = [this](uint32_t& register_) -> void {
		// SLA r8, flags: Z 0 0 C
		m_wait_cycles += 8;

		// Shift Left Arithmetically register r8
		//     ┌─────────┐
		// C <─│7  <──  0│<─ 0
		//     └─────────┘
		//         r8

		// Copy bit 7 into carry flag
		m_cf = (m_a & 0x80) == 0x80;

		// Shift Left Arithmetically register r8
		register_ = (register_ << 1) & 0xff;

		// Set other flags
		m_zf = register_ == 0;
		m_nf = 0;
		m_hf = 0;
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x20: /* SLA B */ shift_left_arithmatically(m_b); break;
	case 0x21: /* SLA C */ shift_left_arithmatically(m_c); break;
	case 0x22: /* SLA D */ shift_left_arithmatically(m_d); break;
	case 0x23: /* SLA E */ shift_left_arithmatically(m_e); break;
	case 0x24: /* SLA H */ shift_left_arithmatically(m_h); break;
	case 0x25: /* SLA L */ shift_left_arithmatically(m_l); break;
	case 0x26: /* SLA (HL) */ {
		m_wait_cycles += 8; // + 8 = 16 total

		// Rotate the byte pointed to by HL right
		uint32_t data = read(hl());
		shift_left_arithmatically(data);
		write(hl(), data);
		break;
	}
	case 0x27: /* SLA A */ shift_left_arithmatically(m_a); break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::sra()
{
	auto shift_right_arithmatically = [this](uint32_t& register_) -> void {
		// SRL r8, flags: Z 0 0 C
		m_wait_cycles += 8;

		// Shift Right Arithmatically register r8
		//    ┌─────────┐
		//  ┌─│7  ──>  0│─> C
		//  │ └─────────┘
		//  │  ^  r8
		//  └──┘

		// Copy bit 0 into carry flag
		m_cf = (m_a & 0x01) == 0x01;

		// Shift Right Arithmatically register r8
		register_ = (register_ >> 1) | (register_ & 0x80); // Note: bit 7 remains

		// Set other flags
		m_zf = register_ == 0;
		m_nf = 0;
		m_hf = 0;
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x28: /* SRA B */ shift_right_arithmatically(m_b); break;
	case 0x29: /* SRA C */ shift_right_arithmatically(m_c); break;
	case 0x2a: /* SRA D */ shift_right_arithmatically(m_d); break;
	case 0x2b: /* SRA E */ shift_right_arithmatically(m_e); break;
	case 0x2c: /* SRA H */ shift_right_arithmatically(m_h); break;
	case 0x2d: /* SRA L */ shift_right_arithmatically(m_l); break;
	case 0x2e: /* SRA (HL) */ {
		m_wait_cycles += 8; // + 8 = 16 total

		// Rotate the byte pointed to by HL right
		uint32_t data = read(hl());
		shift_right_arithmatically(data);
		write(hl(), data);
		break;
	}
	case 0x2f: /* SRA A */ shift_right_arithmatically(m_a); break;
	default:
		VERIFY_NOT_REACHED();
	}
}

void CPU::srl()
{
	auto shift_right_logically = [this](uint32_t& register_) -> void {
		// SRL r8, flags: Z 0 0 C
		m_wait_cycles += 8;

		// Shift Right Locically register r8
		//     ┌─────────┐
		// 0 ─>│7  ──>  0│─> C
		//     └─────────┘
		//         r8

		// Copy bit 0 into carry flag
		m_cf = (m_a & 0x01) == 0x01;

		// Shift Right Locically register r8
		register_ = (register_ >> 1) & 0x7f; // Note: bit 7 is set to 0

		// Set other flags
		m_zf = register_ == 0;
		m_nf = 0;
		m_hf = 0;
	};

	uint8_t opcode = pcRead();
	switch (opcode) {
	case 0x38: /* SRL B */ shift_right_logically(m_b); break;
	case 0x39: /* SRL C */ shift_right_logically(m_c); break;
	case 0x3a: /* SRL D */ shift_right_logically(m_d); break;
	case 0x3b: /* SRL E */ shift_right_logically(m_e); break;
	case 0x3c: /* SRL H */ shift_right_logically(m_h); break;
	case 0x3d: /* SRL L */ shift_right_logically(m_l); break;
	case 0x3e: /* SRL (HL) */ {
		m_wait_cycles += 8; // + 8 = 16 total

		// Rotate the byte pointed to by HL right
		uint32_t data = read(hl());
		shift_right_logically(data);
		write(hl(), data);
		break;
	}
	case 0x3f: /* SRL A */ shift_right_logically(m_a); break;
	default:
		VERIFY_NOT_REACHED();
	}
}
