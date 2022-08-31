/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint> // uint8_t, uint32_t

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
	// print("running opcode: {:#04x} @ ({:#06x})\n", opcode, m_pc);
	if (opcode <= 0x07) {
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
	else if (opcode >= 0xc0) {
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
	case 0x80: /* RES 0,B */ reset_bit(0x01, m_b); break;
	case 0x81: /* RES 0,C */ reset_bit(0x01, m_c); break;
	case 0x82: /* RES 0,D */ reset_bit(0x01, m_d); break;
	case 0x83: /* RES 0,E */ reset_bit(0x01, m_e); break;
	case 0x84: /* RES 0,H */ reset_bit(0x01, m_h); break;
	case 0x85: /* RES 0,L */ reset_bit(0x01, m_l); break;
	case 0x86: /* RES 0,(HL) */ reset_bit_hl(0x01); break;
	case 0x87: /* RES 0,A */ reset_bit(0x01, m_a); break;

	case 0x88: /* RES 1,B */ reset_bit(0x02, m_b); break;
	case 0x89: /* RES 1,C */ reset_bit(0x02, m_c); break;
	case 0x8a: /* RES 1,D */ reset_bit(0x02, m_d); break;
	case 0x8b: /* RES 1,E */ reset_bit(0x02, m_e); break;
	case 0x8c: /* RES 1,H */ reset_bit(0x02, m_h); break;
	case 0x8d: /* RES 1,L */ reset_bit(0x02, m_l); break;
	case 0x8e: /* RES 1,(HL) */ reset_bit_hl(0x02); break;
	case 0x8f: /* RES 1,A */ reset_bit(0x02, m_a); break;

	case 0x90: /* RES 2,B */ reset_bit(0x04, m_b); break;
	case 0x91: /* RES 2,C */ reset_bit(0x04, m_c); break;
	case 0x92: /* RES 2,D */ reset_bit(0x04, m_d); break;
	case 0x93: /* RES 2,E */ reset_bit(0x04, m_e); break;
	case 0x94: /* RES 2,H */ reset_bit(0x04, m_h); break;
	case 0x95: /* RES 2,L */ reset_bit(0x04, m_l); break;
	case 0x96: /* RES 2,(HL) */ reset_bit_hl(0x04); break;
	case 0x97: /* RES 2,A */ reset_bit(0x04, m_a); break;

	case 0x98: /* RES 3,B */ reset_bit(0x08, m_b); break;
	case 0x99: /* RES 3,C */ reset_bit(0x08, m_c); break;
	case 0x9a: /* RES 3,D */ reset_bit(0x08, m_d); break;
	case 0x9b: /* RES 3,E */ reset_bit(0x08, m_e); break;
	case 0x9c: /* RES 3,H */ reset_bit(0x08, m_h); break;
	case 0x9d: /* RES 3,L */ reset_bit(0x08, m_l); break;
	case 0x9e: /* RES 3,(HL) */ reset_bit_hl(0x08); break;
	case 0x9f: /* RES 3,A */ reset_bit(0x08, m_a); break;

	case 0xa0: /* RES 4,B */ reset_bit(0x10, m_b); break;
	case 0xa1: /* RES 4,C */ reset_bit(0x10, m_c); break;
	case 0xa2: /* RES 4,D */ reset_bit(0x10, m_d); break;
	case 0xa3: /* RES 4,E */ reset_bit(0x10, m_e); break;
	case 0xa4: /* RES 4,H */ reset_bit(0x10, m_h); break;
	case 0xa5: /* RES 4,L */ reset_bit(0x10, m_l); break;
	case 0xa6: /* RES 4,(HL) */ reset_bit_hl(0x10); break;
	case 0xa7: /* RES 4,A */ reset_bit(0x10, m_a); break;

	case 0xa8: /* RES 5,B */ reset_bit(0x20, m_b); break;
	case 0xa9: /* RES 5,C */ reset_bit(0x20, m_c); break;
	case 0xaa: /* RES 5,D */ reset_bit(0x20, m_d); break;
	case 0xab: /* RES 5,E */ reset_bit(0x20, m_e); break;
	case 0xac: /* RES 5,H */ reset_bit(0x20, m_h); break;
	case 0xad: /* RES 5,L */ reset_bit(0x20, m_l); break;
	case 0xae: /* RES 5,(HL) */ reset_bit_hl(0x20); break;
	case 0xaf: /* RES 5,A */ reset_bit(0x20, m_a); break;

	case 0xb0: /* RES 6,B */ reset_bit(0x40, m_b); break;
	case 0xb1: /* RES 6,C */ reset_bit(0x40, m_c); break;
	case 0xb2: /* RES 6,D */ reset_bit(0x40, m_d); break;
	case 0xb3: /* RES 6,E */ reset_bit(0x40, m_e); break;
	case 0xb4: /* RES 6,H */ reset_bit(0x40, m_h); break;
	case 0xb5: /* RES 6,L */ reset_bit(0x40, m_l); break;
	case 0xb6: /* RES 6,(HL) */ reset_bit_hl(0x40); break;
	case 0xb7: /* RES 6,A */ reset_bit(0x40, m_a); break;

	case 0xb8: /* RES 7,B */ reset_bit(0x80, m_b); break;
	case 0xb9: /* RES 7,C */ reset_bit(0x80, m_c); break;
	case 0xba: /* RES 7,D */ reset_bit(0x80, m_d); break;
	case 0xbb: /* RES 7,E */ reset_bit(0x80, m_e); break;
	case 0xbc: /* RES 7,H */ reset_bit(0x80, m_h); break;
	case 0xbd: /* RES 7,L */ reset_bit(0x80, m_l); break;
	case 0xbe: /* RES 7,(HL) */ reset_bit_hl(0x80); break;
	case 0xbf: /* RES 7,A */ reset_bit(0x80, m_a); break;
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
	case 0xc0: /* SET 0,B */ set_bit(0x01, m_b); break;
	case 0xc1: /* SET 0,C */ set_bit(0x01, m_c); break;
	case 0xc2: /* SET 0,D */ set_bit(0x01, m_d); break;
	case 0xc3: /* SET 0,E */ set_bit(0x01, m_e); break;
	case 0xc4: /* SET 0,H */ set_bit(0x01, m_h); break;
	case 0xc5: /* SET 0,L */ set_bit(0x01, m_l); break;
	case 0xc6: /* SET 0,(HL) */ set_bit_hl(0x01); break;
	case 0xc7: /* SET 0,A */ set_bit(0x01, m_a); break;

	case 0xc8: /* SET 1,B */ set_bit(0x02, m_b); break;
	case 0xc9: /* SET 1,C */ set_bit(0x02, m_c); break;
	case 0xca: /* SET 1,D */ set_bit(0x02, m_d); break;
	case 0xcb: /* SET 1,E */ set_bit(0x02, m_e); break;
	case 0xcc: /* SET 1,H */ set_bit(0x02, m_h); break;
	case 0xcd: /* SET 1,L */ set_bit(0x02, m_l); break;
	case 0xce: /* SET 1,(HL) */ set_bit_hl(0x02); break;
	case 0xcf: /* SET 1,A */ set_bit(0x02, m_a); break;

	case 0xd0: /* SET 2,B */ set_bit(0x04, m_b); break;
	case 0xd1: /* SET 2,C */ set_bit(0x04, m_c); break;
	case 0xd2: /* SET 2,D */ set_bit(0x04, m_d); break;
	case 0xd3: /* SET 2,E */ set_bit(0x04, m_e); break;
	case 0xd4: /* SET 2,H */ set_bit(0x04, m_h); break;
	case 0xd5: /* SET 2,L */ set_bit(0x04, m_l); break;
	case 0xd6: /* SET 2,(HL) */ set_bit_hl(0x04); break;
	case 0xd7: /* SET 2,A */ set_bit(0x04, m_a); break;

	case 0xd8: /* SET 3,B */ set_bit(0x08, m_b); break;
	case 0xd9: /* SET 3,C */ set_bit(0x08, m_c); break;
	case 0xda: /* SET 3,D */ set_bit(0x08, m_d); break;
	case 0xdb: /* SET 3,E */ set_bit(0x08, m_e); break;
	case 0xdc: /* SET 3,H */ set_bit(0x08, m_h); break;
	case 0xdd: /* SET 3,L */ set_bit(0x08, m_l); break;
	case 0xde: /* SET 3,(HL) */ set_bit_hl(0x08); break;
	case 0xdf: /* SET 3,A */ set_bit(0x08, m_a); break;

	case 0xe0: /* SET 4,B */ set_bit(0x10, m_b); break;
	case 0xe1: /* SET 4,C */ set_bit(0x10, m_c); break;
	case 0xe2: /* SET 4,D */ set_bit(0x10, m_d); break;
	case 0xe3: /* SET 4,E */ set_bit(0x10, m_e); break;
	case 0xe4: /* SET 4,H */ set_bit(0x10, m_h); break;
	case 0xe5: /* SET 4,L */ set_bit(0x10, m_l); break;
	case 0xe6: /* SET 4,(HL) */ set_bit_hl(0x10); break;
	case 0xe7: /* SET 4,A */ set_bit(0x10, m_a); break;

	case 0xe8: /* SET 5,B */ set_bit(0x20, m_b); break;
	case 0xe9: /* SET 5,C */ set_bit(0x20, m_c); break;
	case 0xea: /* SET 5,D */ set_bit(0x20, m_d); break;
	case 0xeb: /* SET 5,E */ set_bit(0x20, m_e); break;
	case 0xec: /* SET 5,H */ set_bit(0x20, m_h); break;
	case 0xed: /* SET 5,L */ set_bit(0x20, m_l); break;
	case 0xee: /* SET 5,(HL) */ set_bit_hl(0x20); break;
	case 0xef: /* SET 5,A */ set_bit(0x20, m_a); break;

	case 0xf0: /* SET 6,B */ set_bit(0x40, m_b); break;
	case 0xf1: /* SET 6,C */ set_bit(0x40, m_c); break;
	case 0xf2: /* SET 6,D */ set_bit(0x40, m_d); break;
	case 0xf3: /* SET 6,E */ set_bit(0x40, m_e); break;
	case 0xf4: /* SET 6,H */ set_bit(0x40, m_h); break;
	case 0xf5: /* SET 6,L */ set_bit(0x40, m_l); break;
	case 0xf6: /* SET 6,(HL) */ set_bit_hl(0x40); break;
	case 0xf7: /* SET 6,A */ set_bit(0x40, m_a); break;

	case 0xf8: /* SET 7,B */ set_bit(0x80, m_b); break;
	case 0xf9: /* SET 7,C */ set_bit(0x80, m_c); break;
	case 0xfa: /* SET 7,D */ set_bit(0x80, m_d); break;
	case 0xfb: /* SET 7,E */ set_bit(0x80, m_e); break;
	case 0xfc: /* SET 7,H */ set_bit(0x80, m_h); break;
	case 0xfd: /* SET 7,L */ set_bit(0x80, m_l); break;
	case 0xfe: /* SET 7,(HL) */ set_bit_hl(0x80); break;
	case 0xff: /* SET 7,A */ set_bit(0x80, m_a); break;
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
	case 0x30: /* SWAP B */ swap_bits(m_b); break;
	case 0x31: /* SWAP C */ swap_bits(m_c); break;
	case 0x32: /* SWAP D */ swap_bits(m_d); break;
	case 0x33: /* SWAP E */ swap_bits(m_e); break;
	case 0x34: /* SWAP H */ swap_bits(m_h); break;
	case 0x35: /* SWAP L */ swap_bits(m_l); break;
	case 0x36: /* SWAP (HL) */ {
		m_wait_cycles += 8; // + 8 = 16 total

		// Swap upper 4 bits in the byte pointed by HL with lower 4 bits
		uint32_t data = read(hl());
		swap_bits(data);
		write(hl(), data);
		break;
	}
	case 0x37: /* SWAP A */ swap_bits(m_a); break;
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
