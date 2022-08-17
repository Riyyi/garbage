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
	std::vector<uint8_t> memory(size);
	m_memory_spaces.emplace(name, memory);
}

void Emu::writeMemory(const char* memory_space, unsigned int location, uint8_t value) {
	// printf("%s %d %d", memory_space, location, value);
	// m_memory_spaces[memory_space][location] = value;
}

uint8_t Emu::readMemory(const char* memory_space, unsigned int location){
	// return m_memory_spaces[memory_space][location];
}