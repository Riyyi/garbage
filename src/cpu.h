/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint>    // int8_t, int32_t, uint8_t, uint32_t
#include <functional> // std::function
#include <unordered_map>

#include "processing-unit.h"

class CPU final : public ProcessingUnit {
public:
	explicit CPU(uint32_t frequency);
	virtual ~CPU();

	void update() override;

	// -------------------------------------
	// Arithmetic and Logic Instructions

	// 8-bit
	void add();

	// 16-bit

	// -------------------------------------
	// Bit Operations Instructions

	// -------------------------------------
	// Bit Shift Instructions

	// -------------------------------------
	// Load Instructions

	// 8-bit
	void ld8();
	void ldh8();

	// 16-bit
	void ld16();

	// -------------------------------------
	// Jumps and Subroutines

	void jp16();

	// -------------------------------------
	// Stack Operations Instructions

	// -------------------------------------
	// Miscellaneous Instructions

private:
	uint8_t peekMemory(int32_t offset = 0) const;
	uint8_t consumeMemory();

	uint32_t immediate8() { return consumeMemory(); }
	uint32_t immediate16() { return consumeMemory() | (consumeMemory() << 8); }

	void writeFf(uint32_t value);
	uint32_t readFf();

	void setBc(uint32_t value);
	void setDe(uint32_t value);
	void setHl(uint32_t value);

	uint32_t bc() const { return m_b << 8 | m_c; }
	uint32_t de() const { return m_b << 8 | m_c; }
	uint32_t hl() const { return m_b << 8 | m_c; }

	// Registers
	uint32_t m_a { 0 };  // Accumulator
	uint32_t m_b { 0 };  // B
	uint32_t m_c { 0 };  // C
	uint32_t m_d { 0 };  // D
	uint32_t m_e { 0 };  // E
	uint32_t m_h { 0 };  // H
	uint32_t m_l { 0 };  // L
	uint32_t m_pc { 0 }; // Program Counter
	uint32_t m_sp { 0 }; // Stack Pointer

	// Flags
	uint32_t m_zf { 0 }; // Zero flag
	uint32_t m_nf { 0 }; // Subtraction flag (BCD)
	uint32_t m_hf { 0 }; // Half Carry flag (BCD)
	uint32_t m_cf { 0 }; // Carry flag

	int8_t m_wait_cycles { 0 };

	std::unordered_map<uint32_t, std::function<void()>> m_opcode_lookup_table;
};
