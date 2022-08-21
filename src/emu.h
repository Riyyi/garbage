/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint> // uint32_t
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "processing-unit.h"
#include "ruc/singleton.h"
#include "ruc/timer.h"

using BankedMemory = std::vector<std::vector<uint32_t>>;

struct MemorySpace {
	BankedMemory memory;
	uint32_t active_bank { 0 };
	uint32_t start_address { 0 };
	uint32_t end_address { 0 };
};

class Emu final : public ruc::Singleton<Emu> {
public:
	Emu(s) {}

	void init(uint32_t frequency, std::string_view bootrom);

	void update();

	void addProcessingUnit(std::string_view name, ProcessingUnit* processing_unit);
	void addMemorySpace(std::string_view name, uint32_t start_address, uint32_t end_address, uint32_t amount_of_banks = 1);

	void writeMemory(uint32_t address, uint32_t value);
	uint32_t readMemory(uint32_t address) const;

	// -------------------------------------

	ProcessingUnit* processingUnit(std::string_view name) const { return m_processing_units.at(name); }

	std::string_view bootrom() const { return m_bootrom; }

private:
	uint32_t m_frequency { 0 };
	double m_timestep { 0 };
	uint32_t m_cycle { 0 };
	double m_cycle_time { 0 };
	double m_previous_time { 0 };

	ruc::Timer m_timer;

	std::unordered_map<std::string_view, ProcessingUnit*> m_processing_units;
	std::unordered_map<std::string_view, MemorySpace> m_memory_spaces;

	std::string m_bootrom;
};
