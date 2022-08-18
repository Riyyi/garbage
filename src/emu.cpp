#include <cstdint> // uint8_t, uint32_t

#include "cpu.h"
#include "emu.h"
#include "ruc/format/print.h"

void Emu::init(uint32_t frequency)
{
	m_frequency = frequency;
	m_timestep = 1.0 / m_frequency * 1000000;
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

void Emu::addProcessingUnit(const char* name, ProcessingUnit* processing_unit)
{
	m_processing_units.emplace(name, processing_unit);
}

void Emu::addMemorySpace(const char* name, uint32_t size)
{
	std::vector<uint32_t> memory(size);
	m_memory_spaces.emplace(name, memory);
}

void Emu::writeMemory(const char* memory_space, uint32_t location, uint32_t value)
{
	m_memory_spaces[memory_space][location] = value;
}

uint8_t Emu::readMemory(const char* memory_space, uint32_t location)
{
	return m_memory_spaces[memory_space][location];
}
