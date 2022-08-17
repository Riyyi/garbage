#include "emu.h"

void Emu::update() {
	for (auto unit : m_processing_units) {
		if (m_cycle % int(m_frequency / unit.frequency()) == 0) {
			unit.update();
		}
	}
}

void Emu::addProcessingUnit(ProcessingUnit processing_unit) {
	m_processing_units.push_back(processing_unit);
}

void Emu::addMemorySpace(const char* name, int size) {
	std::vector<uint8_t> memory;
	memory.reserve(size);
	m_memory_spaces.emplace(name, memory);
}