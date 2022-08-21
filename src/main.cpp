/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#include "cpu.h"
#include "emu.h"
#include "ppu.h"
#include "ruc/format/print.h"
#include "ruc/timer.h"

int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;

	ruc::Timer t;

	print("{}ms\n", t.elapsedNanoseconds() / 1000000.0);

	Emu::the().init(8000000, "gbc_bios.bin");

	CPU cpu(8000000);
	PPU ppu(4000000);

	Emu::the().addProcessingUnit("cpu", &cpu);
	Emu::the().addProcessingUnit("ppu", &ppu);

	// https://gbdev.io/pandocs/Memory_Map.html
	// https://gbdev.io/pandocs/Power_Up_Sequence.html
	Emu::the().addMemorySpace("BOOTROM1", 0x0000, 0x0100);   // 256B
	Emu::the().addMemorySpace("BOOTROM2", 0x0200, 0x08ff);   // 1792B
	Emu::the().addMemorySpace("VRAM", 0x8000, 0x9fff, 2);    // 8KiB * 2 banks
	Emu::the().addMemorySpace("CARDRAM", 0xa000, 0xbfff);    // 8KiB
	Emu::the().addMemorySpace("WRAM1", 0xc000, 0xcfff, 1);   // 4 KiB, Work RAM
	Emu::the().addMemorySpace("WRAM2", 0xd000, 0xdfff, 7);   // 4 KiB * 7 banks, Work RAM
	Emu::the().addMemorySpace("ECHORAM", 0xe000, 0xfdff, 1); // 7679B, Mirror of 0xc000~0xddff
	Emu::the().addMemorySpace("OAM", 0xfe00, 0xfe9f, 1);     // 4KiB, Object Attribute Memory (VRAM Sprite Attribute Table)
	Emu::the().addMemorySpace("HRAM", 0xff80, 0xfffe, 1);    // 126B, High RAM (CPU cache)

	// Get shared register
	print("{}\n", *Emu::the().processingUnit("cpu")->sharedRegister("a"));
	print("{}\n", *Emu::the().processingUnit("cpu")->sharedRegister("b"));

	while (true) {
		Emu::the().update();
	}

	return 0;
}
