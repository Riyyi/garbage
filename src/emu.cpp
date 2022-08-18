#include <cstdint> // uint8_t, uint32_t

#include "cpu.h"
#include "emu.h"
#include "ruc/format/print.h"

void Emu::init(uint32_t frequency)
{
	m_frequency = frequency;
}

void Emu::update()
{
	for (auto unit : m_processing_units) {
		if (m_cycle % (m_frequency / unit->frequency()) == 0) {
			unit->update();
		}
	}
	m_cycle++;
}

void Emu::addProcessingUnit(ProcessingUnit* processing_unit)
{
	m_processing_units.push_back(processing_unit);
}

void Emu::addMemorySpace(const char* name, uint32_t size)
{
	std::vector<uint8_t> memory(size);
	m_memory_spaces.emplace(name, memory);
}

void Emu::writeMemory(const char* memory_space, uint32_t location, uint8_t value)
{
	print("{} {} {}\n", memory_space, location, value);
	m_memory_spaces[memory_space][location] = value;
}

uint8_t Emu::readMemory(const char* memory_space, uint32_t location)
{
	return m_memory_spaces[memory_space][location];
}
