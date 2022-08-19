#pragma once

#include <cstdint> // uint32_t
#include <unordered_map>
#include <vector>

#include "processing-unit.h"
#include "ruc/singleton.h"
#include "ruc/timer.h"

class Emu final : public ruc::Singleton<Emu> {
public:
	Emu(s) {}

	void init(uint32_t frequency);

	void update();

	void addProcessingUnit(const char* name, ProcessingUnit* processing_unit);
	void addMemorySpace(const char* name, uint32_t size);

	void writeMemory(const char* memory_space, uint32_t location, uint32_t value);
	uint32_t readMemory(const char* memory_space, uint32_t location);

	// -------------------------------------

	ProcessingUnit* processingUnit(const char* name) const { return m_processing_units.at(name); }

private:
	uint32_t m_frequency { 0 };
	double m_timestep { 0 };
	uint32_t m_cycle { 0 };
	double m_cycle_time { 0 };
	double m_previous_time { 0 };

	ruc::Timer m_timer;

	std::unordered_map<const char*, ProcessingUnit*> m_processing_units;
	std::unordered_map<const char*, std::vector<uint32_t>> m_memory_spaces;
};
