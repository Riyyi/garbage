#include "emu.h"
#include "cpu.h"
#include <iostream>

void Emu::init(unsigned int frequency) {
	m_frequency = frequency;
}

void Emu::update() {
	for (auto unit : m_processing_units) {
		if (m_cycle % (m_frequency / unit->frequency()) == 0) {
			unit->update();
		}
	}
	m_cycle++;
}

void Emu::addProcessingUnit(ProcessingUnit* processing_unit) {
	m_processing_units.push_back(processing_unit);
}

void Emu::addMemorySpace(const char* name, int size) {
	std::vector<uint8_t> memory;
	memory.reserve(size);
	m_memory_spaces.emplace(name, memory);
}