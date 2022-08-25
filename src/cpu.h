/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint>    // int8_t, uint8_t, uint32_t
#include <functional> // std::function
#include <unordered_map>

#include "processing-unit.h"
#include "ruc/format/formatter.h"

class CPU final : public ProcessingUnit {
private:
	friend struct CPUTest;

public:
	explicit CPU(uint32_t frequency);
	virtual ~CPU();

	void update() override;

	// -------------------------------------
	// Arithmetic and Logic Instructions

	// 8-bit
	void addi8();
	void dec8();
	void xor8();

	// 16-bit
	void addr16();
	void adds8();
	void dec16();

	// -------------------------------------
	// Bit Operations Instructions

	// -------------------------------------
	// Bit Shift Instructions

	// -------------------------------------
	// Load Instructions

	// 8-bit
	void cp();
	void inc();
	void lda8();
	void ldffi8();
	void ldi8();
	void ldr8();

	// 16-bit
	void ldi16();
	void ldr16();

	// -------------------------------------
	// Jumps and Subroutines

	void call();
	void jp16();
	void jrs8();

	// -------------------------------------
	// Stack Operations Instructions

	// -------------------------------------
	// Miscellaneous Instructions

	void misc();

	// -------------------------------------

	uint32_t af() const { return (m_cf << 4 | m_hf << 5 | m_nf << 6 | m_zf << 7) | m_a << 8; }
	uint32_t bc() const { return m_c | m_b << 8; }
	uint32_t de() const { return m_e | m_d << 8; }
	uint32_t hl() const { return m_l | m_h << 8; }
	uint32_t pc() const { return m_pc; }
	uint32_t sp() const { return m_sp; }

private:
	uint32_t pcRead();
	uint32_t pcRead16() { return pcRead() | (pcRead() << 8); }

	void write(uint32_t address, uint32_t value);
	uint32_t read(uint32_t address);

	void ffWrite(uint32_t address, uint32_t value);
	uint32_t ffRead(uint32_t address);

	bool isCarry(uint32_t lhs, uint32_t rhs, uint32_t limit_bit);

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
};

template<>
struct ruc::format::Formatter<CPU> : Formatter<uint32_t> {
	void parse(Parser& parser);
	void format(Builder& builder, const CPU& value) const;
};
