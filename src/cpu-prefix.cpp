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
	// Skip 0xcb opcode
	m_pc = (m_pc + 1) & 0xffff;

	// Read next opcode
	uint8_t opcode = read(m_pc);
	print("running opcode: {:#04x} @ ({:#06x})\n", opcode, m_pc);
	if (opcode >= 0x40 && opcode <= 0x7f) {
		bit();
	}
	else {
		print("opcode {:#04x} not implemented\n", opcode);
		print("immediate: {:#04x}\n", m_pc, pcRead());
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
