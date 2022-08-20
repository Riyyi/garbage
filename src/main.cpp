#include <cstdio>

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

	Emu::the().addMemorySpace("RAM", 1024);
	Emu::the().addMemorySpace("VRAM", 1024);
	Emu::the().addMemorySpace("ROM", 1024);
	Emu::the().addMemorySpace("CARDROM", 1024);

	// Get shared register
	print("{}\n", *Emu::the().processingUnit("cpu")->sharedRegister("a"));
	print("{}\n", *Emu::the().processingUnit("cpu")->sharedRegister("b"));

	while (true) {
		Emu::the().update();
	}

	return 0;
}
