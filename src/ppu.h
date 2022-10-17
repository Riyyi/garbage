/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <array>
#include <cstdint> // uint8_t, uint16_t, uint32_t
#include <queue>
#include <utility> // std::pair

#include "ruc/meta/core.h"

#include "processing-unit.h"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144
#define FORMAT_SIZE 3
#define TILE_WIDTH 8
#define TILE_HEIGHT 8
#define TILE_SIZE 16

class PPU final : public ProcessingUnit {
public:
	PPU(uint32_t frequency);
	~PPU();

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

	enum State : uint8_t {
		HBlank = 0,
		VBlank = 1,
		OAMSearch = 2,
		PixelTransfer = 3,
	};

	enum Palette : uint16_t {
		BGP = 0xff47,  // BG and Window palette
		OBP0 = 0xff48, // OBJ palette 0
		OBP1 = 0xff49, // OBJ palette 1
	};

	struct PixelFifo {
		enum State : uint8_t {
			TileIndex,
			TileDataLow,
			TileDataHigh,
			Sleep,
			Push,
		};

		State state = State::TileIndex;
		bool step = false;
		uint8_t x_coordinate = 0;
		uint8_t tile_index = 0;
		uint8_t tile_line = 0;
		uint8_t pixels_lsb = 0;
		uint8_t pixels_msb = 0;

		using Fifo = std::queue<std::pair<uint8_t, Palette>>; // colorID, source

		Fifo background;
		Fifo oam;
	};

	void update() override;
	void render();

	void pixelFifo();
	void drawTile(uint32_t screen_x, uint32_t screen_y, uint32_t tile_address);
	std::array<uint8_t, 3> getPixelColor(uint8_t color_index, Palette palette);

	void resetFrame();

private:
	State m_state { State::OAMSearch };
	uint32_t m_clocks_into_frame { 0 };
	uint32_t m_lcd_x_coordinate { 0 };
	uint32_t m_lcd_y_coordinate { 0 }; // Note: includes V-Blank

	uint8_t m_viewport_x { 0 };
	uint8_t m_viewport_y { 0 };
	PixelFifo m_pixel_fifo;

	uint32_t m_entity;
	std::array<uint8_t, SCREEN_WIDTH * SCREEN_HEIGHT * FORMAT_SIZE> m_screen;
};
