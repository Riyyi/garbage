/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#include "ppu.h"
#include "ruc/format/print.h"
#include <iostream>

PPU ::PPU(unsigned int frequency)
	: ProcessingUnit(frequency)
{
}

PPU ::~PPU()
{
}

void PPU::update()
{
	// print("ppu update\n");
}
