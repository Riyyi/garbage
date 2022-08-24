/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "ruc/singleton.h"
#include <string_view>

class Loader final : public ruc::Singleton<Loader> {
public:
	Loader(s) {}

	void init();
	void destroy();

	void setRomPath(std::string_view rom_path) { m_rom_path = rom_path; }

private:
	std::string_view m_rom_path;
};
