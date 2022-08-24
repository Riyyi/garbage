/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#include "loader.h"
#include "ruc/argparser.h"
#include "ruc/format/print.h"
#include "ruc/timer.h"
#include <string_view>

int main(int argc, const char* argv[])
{
	std::string_view rom_path;

	ruc::ArgParser argParser;
	argParser.addOption(rom_path, 'r', "rom", nullptr, nullptr, "", ruc::ArgParser::Required::Yes);
	argParser.parse(argc, argv);

	Loader::the().loadRom(rom_path);

	return 0;
}
