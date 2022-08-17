#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>

#include "processing-unit.h"
#include "ruc/singleton.h"

class Emu final : public ruc::Singleton<Emu> {
public:
	Emu(s) {}

	void init(unsigned int frequency);

	void update();

	void addProcessingUnit(ProcessingUnit* processing_unit);
	void addMemorySpace(const char* name, int size);

	void writeRAM(const char* memory_space, int location);
	void writeVRAM(const char* memory_space, int location);
	void writeROM(const char* memory_space, int location);

	uint8_t readRAM(const char* memory_space, int location);
	uint8_t readVRAM(const char* memory_space, int location);
	uint8_t readROM(const char* memory_space, int location);

private:
	unsigned int m_frequency;
	unsigned int m_cycle = 0;

	std::vector<ProcessingUnit*> m_processing_units;
	std::unordered_map<const char*, std::vector<uint8_t>> m_memory_spaces;
};
