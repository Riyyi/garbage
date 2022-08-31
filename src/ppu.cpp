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

	// print("PPU update\n");

	// Increment LY (LCD Y Coordinate)
	uint32_t ly_register = Emu::the().readMemory(0xff44) + 1;
	if (ly_register >= 154) {
		ly_register = 0;
	}
	Emu::the().writeMemory(0xff44, ly_register);
}
