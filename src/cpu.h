/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint> // int8_t, uint8_t, uint32_t

#include "processing-unit.h"

class CPU final : public ProcessingUnit {
public:
	explicit CPU(uint32_t frequency);
	virtual ~CPU();

	void update() override;

	// 8-bit Arithmetic and Logic Instructions

	void add(uint8_t opcode, uint8_t immediate = 0);

	// 16-bit Arithmetic Instructions

	// Bit Operations Instructions

	// Bit Shift Instructions

	// Load Instructions

	// Jumps and Subroutines

	// Stack Operations Instructions

	// Miscellaneous Instructions

private:
	// Registers
	uint32_t m_a { 0 };  // Accumulator
	uint32_t m_bc { 0 }; // BC
	uint32_t m_de { 0 }; // DE
	uint32_t m_hl { 0 }; // HL
	uint32_t m_sp { 0 }; // Stack Pointer
	uint32_t m_pc { 0 }; // Program Counter

	// Flags
	uint32_t m_z { 0 }; // Zero flag
	uint32_t m_n { 0 }; // Subtraction flag (BCD)
	uint32_t m_h { 0 }; // Half Carry flag (BCD)
	uint32_t m_c { 0 }; // Carry flag

	int8_t m_wait_cycles { 0 };
};
