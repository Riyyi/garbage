/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#include <string_view>

#include "inferno.h"
#include "inferno/entrypoint.h"
#include "ppu.h"
#include "ruc/argparser.h"
#include "ruc/format/print.h"
#include "ruc/timer.h"

#include "emu.h"
#include "loader.h"

class GarbAGE final : public Inferno::Application {
public:
	GarbAGE(int argc, char* argv[])
		: Application()
	{
		std::string_view bootrom_path = "gbc_bios.bin";
		std::string_view rom_path;

		ruc::ArgParser argParser;
		argParser.addOption(bootrom_path, 'b', "bootrom", nullptr, nullptr, "", ruc::ArgParser::Required::Yes);
		argParser.addOption(rom_path, 'r', "rom", nullptr, nullptr, "", ruc::ArgParser::Required::Yes);
		argParser.parse(argc, argv);

		Loader::the().setBootromPath(bootrom_path);
		Loader::the().loadRom(rom_path);
	}

	~GarbAGE()
	{
	}

	void update() override
	{
		// 154 scanlines * 456 cycles = 70224 cycles per frame
		for (int i = 0; i < (144 * 456 + 10 * 456); ++i) {
			Emu::the().update();
		}
	}

	void render() override
	{
		auto* ppu = static_cast<PPU*>(Emu::the().processingUnit("PPU").get());
		ppu->render();
	}
};

Inferno::Application* Inferno::createApplication(int argc, char* argv[])
{
	GarbAGE* gameboy = new GarbAGE(argc, argv);
	return gameboy;
}
