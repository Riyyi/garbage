#include <cstdio>

#include "cpu.h"
#include "emu.h"
#include "ppu.h"
#include "ruc/timer.h"

int main(int argc, char* argv[])
{
	ruc::Timer t;

	printf("%fms\n", t.elapsedNanoseconds() / 1000000.0);

	Emu::the().init(4000000);

	CPU cpu(1000000);
	PPU ppu(2000000);

	Emu::the().addProcessingUnit(&cpu);
	Emu::the().addProcessingUnit(&ppu);

	Emu::the().addMemorySpace("RAM", 1024);

	for (int i = 0; i < 1000; i++) {
		Emu::the().update();
	}

	return 0;
}
