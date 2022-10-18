/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint> // uint32_t
#include <memory>  // std::make_shared

#include "glm/ext/vector_float3.hpp" // glm::vec3
#include "glm/ext/vector_float4.hpp" // glm::vec4
#include "inferno/application.h"
#include "inferno/component/spritecomponent.h"
#include "inferno/component/transformcomponent.h"
#include "inferno/scene/scene.h"
#include "ruc/format/print.h"

#include "emu.h"
#include "ppu.h"
#include "ruc/meta/assert.h"

PPU::PPU(uint32_t frequency)
	: ProcessingUnit(frequency)
{
	m_shared_registers.emplace("LY", &m_lcd_y_coordinate);

	auto& scene = Inferno::Application::the().scene();
	m_entity = scene.findEntity("Screen");
}

PPU::~PPU()
{
}

void PPU::update()
{
	LCDC lcd_control = static_cast<LCDC>(Emu::the().readMemory(0xff40));
	if (!(lcd_control & LCDC::LCDandPPUEnable)) {
		return;
	}

	// print("PPU update\n");

	m_clocks_into_frame++;

	switch (m_state) {
	case State::OAMSearch:
		// OAM logic goes here..

		if (m_clocks_into_frame % 80 == 0) {
			// Reset FIFO
			m_pixel_fifo = {};

			m_state = State::PixelTransfer;
		}
		break;
	case State::PixelTransfer:
		pixelFifo();

		if (m_lcd_x_coordinate == 160) {
			m_lcd_x_coordinate = 0;
			m_state = State::HBlank;
		}
		break;
	case State::HBlank:
		// H-Blank logic goes here..

		if (m_clocks_into_frame % (80 + 172 + 204) == 0) {
			m_lcd_y_coordinate++;
			if (m_lcd_y_coordinate == 144) {
				m_state = State::VBlank;
			}
			else {
				m_state = State::OAMSearch;
			}
		}
		break;
	case State::VBlank:
		// V-Blank logic goes here..

		if (m_clocks_into_frame % (80 + 172 + 204) == 0) {
			m_lcd_y_coordinate++;
			if (m_lcd_y_coordinate == 154) {
				resetFrame();
			}
		}

		break;
	default:
		VERIFY_NOT_REACHED();
	};
}

void PPU::render()
{
	LCDC lcd_control = static_cast<LCDC>(Emu::the().readMemory(0xff40));

	if (!(lcd_control & LCDC::BGandWindowEnable)) {
		// When Bit 0 is cleared, both background and window become blank (white)
		auto pixel = getPixelColor(0, Palette::BGP);
		for (size_t i = 0; i < m_screen.size(); i += 3) {
			m_screen[i + 0] = pixel[0];
			m_screen[i + 1] = pixel[1];
			m_screen[i + 2] = pixel[2];
		}
	}
	else {
		// // Tile map
		// uint32_t tile_map_size = 32 * 32; // 1 KiB
		// uint32_t bg_tile_map_address = (lcd_control & LCDC::BGTileMapArea) ? 0x9c00 : 0x9800;
		// // uint32_t window_tile_map_address = (lcd_control & LCDC::WindowTileMapArea) ? 0x9c00 : 0x9800;

		// // Tile data
		// // uint32_t tile_data_size = 4096; // 4KiB / 16B = 256 tiles
		// bool tile_data_mode = lcd_control & LCDC::BGandWindowTileDataArea;
		// uint32_t tile_data_address = (tile_data_mode) ? 0x8000 : 0x8800;

		// for (uint32_t i = 0; i < tile_map_size; ++i) {
		// 	uint8_t tile_index = Emu::the().readMemory(bg_tile_map_address + i);
		// 	// drawTile((i % 32) * TILE_WIDTH, (i / 32) * TILE_HEIGHT, tile_data_address + (tile_index * TILE_SIZE));
		// }
	}

	auto& scene = Inferno::Application::the().scene();
	auto texture = std::make_shared<Inferno::Texture>(m_screen.data(), SCREEN_WIDTH, SCREEN_HEIGHT, FORMAT_SIZE);
	scene.removeComponent<Inferno::SpriteComponent>(m_entity);
	scene.addComponent<Inferno::SpriteComponent>(m_entity, glm::vec4 { 1.0f }, texture);
}

void PPU::pixelFifo()
{
	LCDC lcd_control = static_cast<LCDC>(Emu::the().readMemory(0xff40));

	// Tile map
	uint32_t bg_tile_map_address = (lcd_control & LCDC::BGTileMapArea) ? 0x9c00 : 0x9800;
	// uint32_t window_tile_map_address = (lcd_control & LCDC::WindowTileMapArea) ? 0x9c00 : 0x9800;

	// Tile data
	uint32_t tile_data_address = (lcd_control & LCDC::BGandWindowTileDataArea) ? 0x8000 : 0x8800;

	// https://gbdev.io/pandocs/Tile_Data.html
	auto getBgTileDataAddress = [&](uint8_t tile_index) -> uint32_t {
		switch (tile_data_address) {
		case 0x8000:
			// 0x8000-0x8fff: index   0 => 255
			return tile_data_address + (tile_index * TILE_SIZE); // Each tile is 16 bytes
		case 0x8800:
			// 0x8800-0x8fff: index 128 => 255 (or -128 => -1)
			// 0x9000-0x97ff: index   0 => 127
			if (tile_index <= 127) {
				return tile_data_address + 0x800 + (tile_index * TILE_SIZE); // Each tile is 16 bytes
			}
			else {
				return tile_data_address + ((tile_index - 128) * TILE_SIZE); // Each tile is 16 bytes
			}
		default:
			VERIFY_NOT_REACHED();
			return 0;
		};
	};

	// -------------------------------------
	// FIFO Pixel Fetcher

	switch (m_pixel_fifo.state) {
	case PixelFifo::State::TileIndex:
		if (!m_pixel_fifo.step) {
			m_pixel_fifo.step = true;
		}
		else {
			m_pixel_fifo.step = false;
			m_pixel_fifo.state = PixelFifo::State::TileDataLow;

			// Viewport
			// https://gbdev.io/pandocs/Scrolling.html#mid-frame-behavior
			m_viewport_x = Emu::the().readMemory(0xff43); // TODO: only read lower 3-bits at beginning of scanline
			m_viewport_y = Emu::the().readMemory(0xff42);

			// Read the tile map index
			uint16_t offset = (((m_viewport_y + m_lcd_y_coordinate) / TILE_HEIGHT) * 32)
			                  + ((m_viewport_x + m_pixel_fifo.x_coordinate) / TILE_WIDTH);
			m_pixel_fifo.x_coordinate += 8;
			m_pixel_fifo.tile_index = Emu::the().readMemory(bg_tile_map_address + offset) & 0xff;

			// Set the tile line we're currently on
			m_pixel_fifo.tile_line = (m_viewport_y + m_lcd_y_coordinate) % TILE_HEIGHT;
		}
		break;
	case PixelFifo::State::TileDataLow:
		if (!m_pixel_fifo.step) {
			m_pixel_fifo.step = true;
		}
		else {
			m_pixel_fifo.step = false;
			m_pixel_fifo.state = PixelFifo::State::TileDataHigh;

			// Read tile data
			m_pixel_fifo.pixels_lsb = Emu::the().readMemory(
				getBgTileDataAddress(m_pixel_fifo.tile_index)
				+ m_pixel_fifo.tile_line * 2); // Each tile line is 2 bytes
		}
		break;
	case PixelFifo::State::TileDataHigh:
		if (!m_pixel_fifo.step) {
			m_pixel_fifo.step = true;
		}
		else {
			m_pixel_fifo.step = false;
			m_pixel_fifo.state = PixelFifo::State::Sleep;

			// Read tile data
			m_pixel_fifo.pixels_msb = Emu::the().readMemory(
				getBgTileDataAddress(m_pixel_fifo.tile_index)
				+ m_pixel_fifo.tile_line * 2 // Each tile line is 2 bytes
				+ 1);
		}
		break;
	case PixelFifo::State::Sleep:
		if (m_pixel_fifo.background.size() <= 9) {
			m_pixel_fifo.state = PixelFifo::State::Push;
		}
		break;
	case PixelFifo::State::Push:
		m_pixel_fifo.state = PixelFifo::State::TileIndex;

		for (uint8_t i = 0; i < 8; ++i) {
			uint8_t color_index = (m_pixel_fifo.pixels_lsb >> (7 - i)
			                       | ((m_pixel_fifo.pixels_msb >> (7 - i)) << 1))
			                      & 0x3;
			m_pixel_fifo.background.push({ color_index, Palette::BGP });
		}
		break;
	default:
		VERIFY_NOT_REACHED();
	};

	// -------------------------------------
	// Mode 3 Operation

	// The pixel FIFO needs to contain more than 8 pixels to shift one out
	if (m_pixel_fifo.background.size() > 8) {
		auto pixel = m_pixel_fifo.background.front();
		m_pixel_fifo.background.pop();

		uint32_t index = (m_lcd_y_coordinate * SCREEN_WIDTH + m_lcd_x_coordinate) * FORMAT_SIZE;
		auto color = getPixelColor(pixel.first, pixel.second);
		m_screen[index + 0] = color[0];
		m_screen[index + 1] = color[1];
		m_screen[index + 2] = color[2];
		m_lcd_x_coordinate++;
	}
}

void PPU::drawTile(uint32_t x, uint32_t y, uint32_t tile_address)
{
	uint32_t viewport_x = Emu::the().readMemory(0xff43);
	uint32_t viewport_y = Emu::the().readMemory(0xff42);

	// Tile is not within viewport
	if ((x < viewport_x || x > viewport_x + SCREEN_WIDTH)
	    || (y < viewport_y || y > viewport_y + SCREEN_HEIGHT)) {
		return;
	}

	size_t screen_index = ((x - viewport_x) * FORMAT_SIZE) + ((y - viewport_y) * SCREEN_WIDTH * FORMAT_SIZE);
	for (uint8_t tile_y = 0; tile_y < TILE_SIZE; tile_y += 2) {
		uint8_t pixels_lsb = Emu::the().readMemory(tile_address + tile_y);
		uint8_t pixels_msb = Emu::the().readMemory(tile_address + tile_y + 1);

		for (uint8_t tile_x = 0; tile_x < 8; ++tile_x) {
			size_t index = screen_index + (tile_x * FORMAT_SIZE);

			// FIXME: Tile is partly out of viewport?
			if (index + 2 >= m_screen.size()) {
				continue;
			}

			uint8_t pixel_index = (pixels_lsb >> (7 - tile_x) | ((pixels_msb >> (7 - tile_x)) << 1)) & 0x3;
			auto pixel_color = getPixelColor(pixel_index, Palette::BGP);
			m_screen[index + 0] = pixel_color[0];
			m_screen[index + 1] = pixel_color[1];
			m_screen[index + 2] = pixel_color[2];
		}

		// Move to next line
		screen_index += SCREEN_WIDTH * FORMAT_SIZE;
	}
}

std::array<uint8_t, 3> PPU::getPixelColor(uint8_t color_index, Palette palette)
{
	VERIFY(color_index < 4, "trying to fetch invalid color index '{}'", color_index);

	switch (Emu::the().mode()) {
	case Emu::Mode::DMG: {
		uint8_t palette_data = Emu::the().readMemory(palette) & 0xff;
		uint8_t palette_value = palette_data >> (color_index * 2) & 0x3;
		switch (palette_value) {
		case 0:
			return { 200, 199, 168 };
		case 1:
			return { 160, 160, 136 };
		case 2:
			return { 104, 104, 80 };
		case 3:
			return { 39, 40, 24 };
		default:
			VERIFY_NOT_REACHED();
		};
	}
	case Emu::Mode::CGB:
		VERIFY_NOT_REACHED();
	default:
		VERIFY_NOT_REACHED();
	}

	return {};
}

void PPU::resetFrame()
{
	m_state = State::OAMSearch;
	m_clocks_into_frame = 0;
	m_lcd_x_coordinate = 0;
	m_lcd_y_coordinate = 0;
}
