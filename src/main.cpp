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
	Emu::the().addMemorySpace("CARDROM", (1 + 0x3fff - 0x0000));                          // 16KiB
	Emu::the().addMemorySpace("VRAM", (1 + 0x9fff - 0x8000) * 2);                         // 8KiB * 2 banks
	Emu::the().addMemorySpace("CARDRAM", (1 + 0xbfff - 0xa000));                          // 8KiB
	Emu::the().addMemorySpace("WRAM", (1 + 0xcfff - 0xc000) + (1 + 0xdfff - 0xd000) * 7); // 4 KiB + 4 KiB * 7 banks, Work RAM
	Emu::the().addMemorySpace("ECHORAM", (1 + 0xfdff - 0xe000));                          // 7679B, Mirror of 0xc000~0xddff
	Emu::the().addMemorySpace("HRAM", (1 + 0xfffe - 0xff80));                             // 126B, High RAM (CPU cache)

	// Get shared register
	print("{}\n", *Emu::the().processingUnit("cpu")->sharedRegister("a"));
	print("{}\n", *Emu::the().processingUnit("cpu")->sharedRegister("b"));

	while (true) {
		Emu::the().update();
	}

	return 0;
}
