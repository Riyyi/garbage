/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <string_view>

#include "ruc/singleton.h"

class Loader final : public ruc::Singleton<Loader> {
public:
	Loader(s) {}

	void loadRom(std::string_view rom_path);

private:
	void init();
	void update();
	void destroy();

	void loadCartridgeHeader();

	std::string m_rom_data;
};
