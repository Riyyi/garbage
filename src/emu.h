#pragma once

#include <cstdint> // uint8_t, uint32_t
#include <unordered_map>
#include <vector>

#include "processing-unit.h"
#include "ruc/singleton.h"

class Emu final : public ruc::Singleton<Emu> {
public:
	Emu(s) {}

	void init(uint32_t frequency);

	void update();

	void addProcessingUnit(ProcessingUnit* processing_unit);
	void addMemorySpace(const char* name, uint32_t size);

	void writeMemory(const char* memory_space, uint32_t location, uint8_t value);

	uint8_t readMemory(const char* memory_space, uint32_t location);

private:
	uint32_t m_frequency;
	uint32_t m_cycle = 0;

	std::vector<ProcessingUnit*> m_processing_units;
	std::unordered_map<const char*, std::vector<uint8_t>> m_memory_spaces;
};
