/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

class ProcessingUnit {
public:
	ProcessingUnit(float frequency);
	virtual ~ProcessingUnit();

	virtual void update();

	float frequency();

private:
float m_frequency;
};
