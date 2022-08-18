/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint> // uint8_t, uint16_t

#include "processing-unit.h"

class CPU final : public ProcessingUnit {
public:
	CPU(unsigned int frequency);
	virtual ~CPU();

	void update() override;

	// 8-bit Arithmetic and Logic Instructions

	void add(uint8_t byte, uint8_t immediate = 0);

	// 16-bit Arithmetic Instructions

	// Bit Operations Instructions

	// Bit Shift Instructions

	// Load Instructions

	// Jumps and Subroutines

	// Stack Operations Instructions

	// Miscellaneous Instructions

private:
	// Registers
	uint8_t m_a { 0 };   // Accumulator
	uint16_t m_bc { 0 }; // BC
	uint16_t m_de { 0 }; // DE
	uint16_t m_hl { 0 }; // HL
	uint16_t m_sp { 0 }; // Stack Pointer
	uint16_t m_pc { 0 }; // Program Counter

	// Flags
	uint8_t m_z { 0 }; // Zero flag
	uint8_t m_n { 0 }; // Subtraction flag (BCD)
	uint8_t m_h { 0 }; // Half Carry flag (BCD)
	uint8_t m_c { 0 }; // Carry flag
};
