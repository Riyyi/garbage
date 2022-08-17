/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#include "processing-unit.h"

ProcessingUnit::ProcessingUnit(float frequency) : m_frequency(frequency)
{
}

ProcessingUnit::~ProcessingUnit()
{
}

float ProcessingUnit::frequency() {
	return m_frequency;
}