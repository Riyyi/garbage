/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint> // uint8_t, uint32_t

#include "processing-unit.h"
#include "ruc/meta/core.h"

enum LCDC : uint8_t {
	None = 0,
	BGandWindowEnable = BIT(0),
	OBJEnable = BIT(1),
	OBJSize = BIT(2),                 // 0 = 8x8, 1 = 8x16
	BGTileMapArea = BIT(3),           // 0 = 9800-9bff, 1 = 9c00-9fff
	BGandWindowTileDataArea = BIT(4), // 0 = 8800-97ff, 1 = 8000-8fff
	WindowEnable = BIT(5),            //
	WindowTileMapArea = BIT(6),       // 0 = 9800-9bff, 1 = 9c00-9fff
	LCDandPPUEnable = BIT(7),
};

class PPU final : public ProcessingUnit {
public:
	PPU(uint32_t frequency);
	virtual ~PPU();

	virtual void update() override;
};
