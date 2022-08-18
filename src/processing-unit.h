/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint> // uint32_t

class ProcessingUnit {
public:
	ProcessingUnit(uint32_t frequency);
	virtual ~ProcessingUnit();

	virtual void update() = 0;

	uint32_t frequency();

private:
	uint32_t m_frequency { 0 };
};
