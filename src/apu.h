/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "processing-unit.h"

class APU : public ProcessingUnit {
public:
	APU(unsigned int frequency);
	virtual ~APU();
};
