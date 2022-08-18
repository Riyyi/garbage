/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

class ProcessingUnit {
public:
	ProcessingUnit(unsigned int frequency);
	virtual ~ProcessingUnit();

	virtual void update() = 0;

	unsigned int frequency();

private:
	unsigned int m_frequency { 0 };
};
