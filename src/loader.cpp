/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstddef> // size_t
#include <cstdint> // uint32_t
#include <memory>  // std::make_shared

#include "cpu.h"
#include "emu.h"
#include "loader.h"
#include "ppu.h"
#include "ruc/file.h"
#include "ruc/format/print.h"

void Loader::loadRom(std::string_view rom_path)
{
	if (!rom_path.empty()) {
		m_rom_data = ruc::File(rom_path).data();
	}

	init();
}

void Loader::disableBootrom()
{
	Emu::the().removeMemorySpace("BOOTROM1");
	Emu::the().removeMemorySpace("CARTHEADER");
	Emu::the().removeMemorySpace("BOOTROM2");

	// Load cartridge bank 0
	Emu::the().addMemorySpace("CARTROM1", 0x0000, 0x3fff); // 16KiB
	for (size_t i = 0x0000; i <= 0x3fff; ++i) {
		Emu::the().writeMemory(i, m_rom_data[i]);
	}
}

// -----------------------------------------

void Loader::init()
{
	destroy();

	Emu::the().init(8000000);

	auto cpu = std::make_shared<CPU>(8000000);
	auto ppu = std::make_shared<PPU>(4000000);

	Emu::the().addProcessingUnit("CPU", cpu);
	Emu::the().addProcessingUnit("PPU", ppu);

	// https://gbdev.io/pandocs/Memory_Map.html
	// https://gbdev.io/pandocs/Power_Up_Sequence.html
	Emu::the().addMemorySpace("BOOTROM1", 0x0000, 0x00ff); // 256B
	loadCartridgeHeader();
	Emu::the().addMemorySpace("BOOTROM2", 0x0200, 0x08ff); // 1792B
	loadCartridgeBanks();
	Emu::the().addMemorySpace("VRAM", 0x8000, 0x9fff, 2);    // 8KiB * 2 banks
	Emu::the().addMemorySpace("CARTRAM", 0xa000, 0xbfff, 1); // 8KiB * ? banks, if any
	Emu::the().addMemorySpace("WRAM1", 0xc000, 0xcfff);      // 4 KiB, Work RAM
	Emu::the().addMemorySpace("WRAM2", 0xd000, 0xdfff, 7);   // 4 KiB * 7 banks, Work RAM
	Emu::the().addMemorySpace("ECHORAM", 0xe000, 0xfdff);    // 7680B, Mirror of 0xc000~0xddff
	Emu::the().addMemorySpace("OAM", 0xfe00, 0xfe9f);        // 160B, Object Attribute Memory (VRAM Sprite Attribute Table)
	Emu::the().addMemorySpace("Not Usable", 0xfea0, 0xfeff); // 96B, Nintendo probibits this area
	Emu::the().addMemorySpace("IO", 0xff00, 0xff7f);         // 128B, I/O Registers
	Emu::the().addMemorySpace("HRAM", 0xff80, 0xfffe);       // 127B, High RAM (CPU cache)
	Emu::the().addMemorySpace("IE", 0xffff, 0xffff);         // 1B, Interrupt Enable register

	// Load bootrom
	auto bootrom = ruc::File(m_bootrom_path).data();
	for (size_t i = 0; i < bootrom.length(); ++i) {
		// Skip cartridge header memory range
		if (i >= 0x0100 && i <= 0x01ff) {
			continue;
		}

		Emu::the().writeMemory(i, bootrom[i]);
	}
}

void Loader::destroy()
{
	Emu::destroy();
}

void Loader::loadCartridgeHeader()
{
	if (m_rom_data.empty()) {
		return;
	}

	Emu::the().addMemorySpace("CARTHEADER", 0x100, 0x14f); // 80B

	for (size_t i = 0x0100; i <= 0x014f; ++i) {
		Emu::the().writeMemory(i, m_rom_data[i]);
	}
}

void Loader::loadCartridgeBanks()
{
	if (m_rom_data.empty()) {
		return;
	}

	uint32_t rom_size = 32 * 1024 * (1 << m_rom_data[0x0148]);
	uint32_t rom_banks = rom_size / (16 * 1024) - 1;
	Emu::the().addMemorySpace("CARTROM2", 0x4000, 0x7fff, rom_banks); // 16KiB * banks

	// Load cartridge bank 1~NN
	auto rom_memory_spaces = Emu::the().memorySpace("CARTROM2");
	for (size_t i = 0; i < rom_banks; ++i) {
		for (size_t i = 0x4000; i <= 0x7fff; ++i) {
			Emu::the().writeMemory(i, m_rom_data[i]);
		}
		rom_memory_spaces.active_bank += 1;
	}
	rom_memory_spaces.active_bank = 0;
}
