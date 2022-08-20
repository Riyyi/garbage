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

class Emu final : public ruc::Singleton<Emu> {
public:
	Emu(s) {}

	void init(uint32_t frequency, std::string_view bootrom);

	void update();

	void addProcessingUnit(std::string_view name, ProcessingUnit* processing_unit);
	void addMemorySpace(std::string_view name, uint32_t size);

	void writeMemory(std::string_view memory_space, uint32_t location, uint32_t value);
	uint32_t readMemory(std::string_view memory_space, uint32_t location);

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
	std::unordered_map<std::string_view, std::vector<uint32_t>> m_memory_spaces;

	std::string m_bootrom;
};
