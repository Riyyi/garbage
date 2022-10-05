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

PPU ::PPU(uint32_t frequency)
	: ProcessingUnit(frequency)
{
	auto& scene = Inferno::Application::the().scene();
	m_entity = scene.createEntity("Screen");

	// Flip the screen to accommodate OpenGL's coordinate system
	auto& transform = scene.getComponent<Inferno::TransformComponent>(m_entity);
	transform.scale.y = -1.0f;

	auto texture = std::make_shared<Inferno::Texture>(m_screen.data(), SCREEN_WIDTH, SCREEN_HEIGHT, FORMAT_SIZE);
	scene.addComponent<Inferno::SpriteComponent>(m_entity, glm::vec4 { 1.0f }, texture);
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

void PPU::render()
{
	LCDC lcd_control = static_cast<LCDC>(Emu::the().readMemory(0xff40));

	if (!(lcd_control & LCDC::BGandWindowEnable)) {
		// When Bit 0 is cleared, both background and window become blank (white)
		m_screen.fill(255);
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

		// Pallete
		uint8_t bg_palette = Emu::the().readMemory(0xff47) & 0xff;

		for (uint32_t i = 0; i < tile_map_size; ++i) {
			uint8_t tile_index = Emu::the().readMemory(bg_tile_map_address + i);
			drawTile((i % 32) * TILE_WIDTH, (i / 32) * TILE_HEIGHT, tile_data_address + (tile_index * TILE_SIZE), bg_palette);
		}
	}

	auto& scene = Inferno::Application::the().scene();
	auto texture = std::make_shared<Inferno::Texture>(m_screen.data(), SCREEN_WIDTH, SCREEN_HEIGHT, FORMAT_SIZE);
	scene.removeComponent<Inferno::SpriteComponent>(m_entity);
	scene.addComponent<Inferno::SpriteComponent>(m_entity, glm::vec4 { 1.0f }, texture);
}

void PPU::drawTile(uint32_t x, uint32_t y, uint32_t tile_address, uint8_t bg_palette)
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
			uint8_t pixel_color = (bg_palette >> (pixel_index * 2)) & 0x3;

			if (pixel_color == 0) {
				m_screen[index + 0] = 255;
				m_screen[index + 1] = 255;
				m_screen[index + 2] = 255;
			}
			else if (pixel_color == 1) {
				m_screen[index + 0] = 168;
				m_screen[index + 1] = 168;
				m_screen[index + 2] = 168;
			}
			else if (pixel_color == 2) {
				m_screen[index + 0] = 84;
				m_screen[index + 1] = 84;
				m_screen[index + 2] = 84;
			}
			else if (pixel_color == 3) {
				m_screen[index + 0] = 0;
				m_screen[index + 1] = 0;
				m_screen[index + 2] = 0;
			}
		}

		// Move to next line
		screen_index += SCREEN_WIDTH * FORMAT_SIZE;
	}
}
