/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint> // uint32_t
#include <string_view>

#include "cpu.h"
#include "emu.h"
#include "ruc/file.h"
#include "ruc/format/print.h"

void Emu::init(uint32_t frequency, std::string_view bootrom)
{
	m_frequency = frequency;
	m_timestep = 1.0 / m_frequency * 1000000;
	m_bootrom = ruc::File(bootrom.data()).data();
}

void Emu::update()
{
	double time = m_timer.elapsedNanoseconds() / 1000.0;
	m_cycle_time += (time - m_previous_time);
	m_previous_time = time;
	if (m_cycle_time > m_timestep) {
		for (auto unit : m_processing_units) {
			if (m_cycle % (m_frequency / unit.second->frequency()) == 0) {
				unit.second->update();
			}
		}
		m_cycle_time -= m_timestep;
		m_cycle++;
	}
}

void Emu::addProcessingUnit(std::string_view name, ProcessingUnit* processing_unit)
{
	m_processing_units.emplace(name, processing_unit);
}

void Emu::addMemorySpace(std::string_view name, uint32_t start_address, uint32_t end_adress, uint32_t amount_of_banks)
{
	uint32_t bank_length = 1 + end_adress - start_address;
	MemorySpace memory_space {
		.memory = BankedMemory(amount_of_banks, std::vector<uint32_t>(bank_length)),
		.active_bank = 0,
		.start_address = start_address,
		.end_address = end_adress,
	};

	m_memory_spaces.emplace(name, memory_space);
}

void Emu::writeMemory(uint32_t address, uint32_t value)
{
	for (auto& memory_space : m_memory_spaces) {
		auto memory = memory_space.second;
		if (address >= memory.start_address && address <= memory.end_address) {
			memory.memory[memory.active_bank][address] = value;
		}
	}
}

uint32_t Emu::readMemory(uint32_t address)
{
	for (auto& memory_space : m_memory_spaces) {
		auto memory = memory_space.second;
		if (address >= memory.start_address && address <= memory.end_address) {
			return memory.memory[memory.active_bank][address];
			break;
		}
	}

	return 0;
}
