/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#include "processing-unit.h"
#include <iostream>

ProcessingUnit::ProcessingUnit(unsigned int frequency) : m_frequency(frequency)
{
}

ProcessingUnit::~ProcessingUnit()
{
}

unsigned int ProcessingUnit::frequency() {
	return m_frequency;
}