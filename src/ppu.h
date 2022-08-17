/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "processing-unit.h"

class PPU final : public ProcessingUnit {
public:
	PPU(unsigned int frequency);
	virtual ~PPU();

	virtual void update() override;
};
