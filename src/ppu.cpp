/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint> // uint8_t, uint16_t, uint32_t
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
		updatePixelFifo();

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

	auto& scene = Inferno::Application::the().scene();
	auto texture = std::make_shared<Inferno::Texture>(m_screen.data(), SCREEN_WIDTH, SCREEN_HEIGHT, FORMAT_SIZE);
	scene.removeComponent<Inferno::SpriteComponent>(m_entity);
	scene.addComponent<Inferno::SpriteComponent>(m_entity, glm::vec4 { 1.0f }, texture);
}

void PPU::resetFrame()
{
	m_state = State::OAMSearch;
	m_clocks_into_frame = 0;
	m_lcd_x_coordinate = 0;
	m_lcd_y_coordinate = 0;
}

// -----------------------------------------

uint32_t PPU::getBgTileDataAddress(uint8_t tile_index)
{
	// https://gbdev.io/pandocs/Tile_Data.html
	switch (m_pixel_fifo.tile_data_address) {
	case 0x8000:
		// 0x8000-0x8fff: index   0 => 255
		return m_pixel_fifo.tile_data_address + (tile_index * TILE_SIZE); // Each tile is 16 bytes
	case 0x8800:
		// 0x8800-0x8fff: index 128 => 255 (or -128 => -1)
		// 0x9000-0x97ff: index   0 => 127
		if (tile_index <= 127) {
			return m_pixel_fifo.tile_data_address + 0x800 + (tile_index * TILE_SIZE); // Each tile is 16 bytes
		}
		else {
			return m_pixel_fifo.tile_data_address + ((tile_index - 128) * TILE_SIZE); // Each tile is 16 bytes
		}
	default:
		VERIFY_NOT_REACHED();
		return 0;
	};
};

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

void PPU::updatePixelFifo()
{
	switch (m_pixel_fifo.state) {
	case PixelFifo::State::TileIndex:
		tileIndex();
		break;
	case PixelFifo::State::TileDataLow:
		tileDataLow();
		break;
	case PixelFifo::State::TileDataHigh:
		tileDataHigh();
		break;
	case PixelFifo::State::Sleep:
		sleep();
		break;
	case PixelFifo::State::Push:
		pushFifo();
		break;
	default:
		VERIFY_NOT_REACHED();
	};

	pushPixel();
}

void PPU::tileIndex()
{
	if (!m_pixel_fifo.step) {
		m_pixel_fifo.step = true;
	}
	else {
		m_pixel_fifo.step = false;
		m_pixel_fifo.state = PixelFifo::State::TileDataLow;

		LCDC lcd_control = static_cast<LCDC>(Emu::the().readMemory(0xff40));

		// Tile map
		uint32_t bg_tile_map_address = (lcd_control & LCDC::BGTileMapArea) ? 0x9c00 : 0x9800;
		// uint32_t window_tile_map_address = (lcd_control & LCDC::WindowTileMapArea) ? 0x9c00 : 0x9800;

		// Tile data
		m_pixel_fifo.tile_data_address = (lcd_control & LCDC::BGandWindowTileDataArea) ? 0x8000 : 0x8800;

		// Viewport
		// https://gbdev.io/pandocs/Scrolling.html#mid-frame-behavior
		m_pixel_fifo.viewport_x = Emu::the().readMemory(0xff43); // TODO: only read lower 3-bits at beginning of scanline
		m_pixel_fifo.viewport_y = Emu::the().readMemory(0xff42);

		// Read the tile map index
		uint16_t offset = (((m_pixel_fifo.viewport_y + m_lcd_y_coordinate) / TILE_HEIGHT) * 32)
		                  + ((m_pixel_fifo.viewport_x + m_pixel_fifo.x_coordinate) / TILE_WIDTH);
		m_pixel_fifo.x_coordinate += 8;
		m_pixel_fifo.tile_index = Emu::the().readMemory(bg_tile_map_address + offset);

		// Set the tile line we're currently on
		m_pixel_fifo.tile_line = (m_pixel_fifo.viewport_y + m_lcd_y_coordinate) % TILE_HEIGHT;
	}
}

void PPU::tileDataLow()
{
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
}

void PPU::tileDataHigh()
{
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
}

void PPU::sleep()
{
	if (m_pixel_fifo.background.size() <= TILE_WIDTH + 1) {
		m_pixel_fifo.state = PixelFifo::State::Push;
	}
}

void PPU::pushFifo()
{
	m_pixel_fifo.state = PixelFifo::State::TileIndex;

	for (uint8_t i = 0; i < TILE_WIDTH; ++i) {
		uint8_t color_index = (m_pixel_fifo.pixels_lsb >> (7 - i)
		                       | ((m_pixel_fifo.pixels_msb >> (7 - i)) << 1))
		                      & 0x3;
		m_pixel_fifo.background.push({ color_index, Palette::BGP });
	}
}

void PPU::pushPixel()
{
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
