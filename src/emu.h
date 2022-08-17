#pragma once

#include <cstdint>
#include <vector>

#include "processing-unit.h"
#include "ruc/singleton.h"

class Emu : ruc::Singleton<Emu> {
public:
	Emu(s) {}

	void WriteRAM(int location, int length);
	void WriteVRAM(int location, int length);
	void WriteROM(int location, int length);

	void ReadRAM(int location, int length);
	void ReadVRAM(int location, int length);
	void ReadROM(int location, int length);

private:
	float m_frequency;
	int m_cycle = 0;

	uint8_t m_ram[1024];
	uint8_t m_vram[1024];
	uint8_t m_rom[1024];

	std::vector<ProcessingUnit> m_processing_units;
};
