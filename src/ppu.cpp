/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint> // uint32_t

#include "emu.h"
#include "ppu.h"
#include "ruc/format/print.h"

PPU ::PPU(uint32_t frequency)
	: ProcessingUnit(frequency)
{
}

PPU ::~PPU()
{
}

void PPU::update()
{
	LCDC lcd_control = static_cast<LCDC>(Emu::the().readMemory(0xff40));
	if (!(lcd_control & LCDC::LCDandPPUEnable)) {
		return;
	}

	print("PPU update\n");
}
