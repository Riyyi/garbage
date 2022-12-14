/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint> // uint32_t
#include <string_view>
#include <unordered_map>

class ProcessingUnit {
public:
	ProcessingUnit(uint32_t frequency);
	virtual ~ProcessingUnit();

	virtual void update() = 0;

	// -------------------------------------

	uint32_t frequency() const { return m_frequency; };
	uint32_t* sharedRegister(std::string_view shared_register) const { return m_shared_registers.at(shared_register); }

protected:
	uint32_t m_frequency { 0 };
	std::unordered_map<std::string_view, uint32_t*> m_shared_registers;
};
