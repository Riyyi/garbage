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

		if (m_clocks_into_frame % 20 == 0) {
			m_state = State::PixelTransfer;
		}
		break;
	case State::PixelTransfer:
		// PixelTransfer logic goes here..
		m_lcd_x_coordinate++;

		if (m_lcd_x_coordinate == 160) {
			m_lcd_x_coordinate = 0;
			m_state = State::HBlank;
		}
		break;
	case State::HBlank:
		// H-Blank logic goes here..

		if (m_clocks_into_frame % (20 + 43 + 51) == 0) {
			if (m_lcd_y_coordinate == 144) {
				m_state = State::VBlank;
			}
			else {
				m_state = State::OAMSearch;
			}
			m_lcd_y_coordinate++;
		}
		break;
	case State::VBlank:
		// V-Blank logic goes here..

		if (m_clocks_into_frame % (20 + 43 + 51) == 0) {
			if (m_lcd_y_coordinate == 154) {
				m_lcd_y_coordinate = 0;
				m_clocks_into_frame = 0;
				m_state = State::OAMSearch;
			}
			m_lcd_y_coordinate++;
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
		// Tile map
		uint32_t tile_map_size = 32 * 32; // 1 KiB
		uint32_t bg_tile_map_address = (lcd_control & LCDC::BGTileMapArea) ? 0x9c00 : 0x9800;
		// uint32_t window_tile_map_address = (lcd_control & LCDC::WindowTileMapArea) ? 0x9c00 : 0x9800;

		// Tile data
		// uint32_t tile_data_size = 4096; // 4KiB / 16B = 256 tiles
		bool tile_data_mode = lcd_control & LCDC::BGandWindowTileDataArea;
		uint32_t tile_data_address = (tile_data_mode) ? 0x8000 : 0x8800;

		for (uint32_t i = 0; i < tile_map_size; ++i) {
			uint8_t tile_index = Emu::the().readMemory(bg_tile_map_address + i);
			drawTile((i % 32) * TILE_WIDTH, (i / 32) * TILE_HEIGHT, tile_data_address + (tile_index * TILE_SIZE));
		}
	}

	auto& scene = Inferno::Application::the().scene();
	auto texture = std::make_shared<Inferno::Texture>(m_screen.data(), SCREEN_WIDTH, SCREEN_HEIGHT, FORMAT_SIZE);
	scene.removeComponent<Inferno::SpriteComponent>(m_entity);
	scene.addComponent<Inferno::SpriteComponent>(m_entity, glm::vec4 { 1.0f }, texture);
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
