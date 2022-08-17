#include <cstdio>

#include "emu.h"
#include "ruc/timer.h"

int main(int argc, char* argv[])
{
	ruc::Timer t;

	printf("%fms\n", t.elapsedNanoseconds() / 1000000.0);

	Emu::the().ReadRAM(0, 0);

	return 0;
}
