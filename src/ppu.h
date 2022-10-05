/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <array>
#include <cstdint> // uint8_t, uint32_t

#include "ruc/meta/core.h"

#include "processing-unit.h"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144
#define FORMAT_SIZE 3
#define TILE_WIDTH 8
#define TILE_HEIGHT 8
#define TILE_SIZE 16

enum LCDC : uint8_t {
	None = 0,
	BGandWindowEnable = BIT(0),
	OBJEnable = BIT(1),
	OBJSize = BIT(2),                 // 0 = 8x8, 1 = 8x16
	BGTileMapArea = BIT(3),           // 0 = 0x9800-9bff, 1 = 0x9c00-9fff
	BGandWindowTileDataArea = BIT(4), // 0 = 0x8800-97ff, 1 = 0x8000-8fff
	WindowEnable = BIT(5),            //
	WindowTileMapArea = BIT(6),       // 0 = 0x9800-9bff, 1 = 0x9c00-9fff
	LCDandPPUEnable = BIT(7),
};

class PPU final : public ProcessingUnit {
public:
	PPU(uint32_t frequency);
	~PPU();

	void update() override;
	void render();

	void drawTile(uint32_t screen_x, uint32_t screen_y, uint32_t tile_address, uint8_t bg_palette);

private:
	uint32_t m_entity;
	std::array<uint8_t, SCREEN_WIDTH * FORMAT_SIZE * SCREEN_HEIGHT> m_screen;
};
