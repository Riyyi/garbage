/*
 * Copyright (C) 2022 Riyyi
 * Copyright (C) 2022 Th3FrankXD
 *
 * SPDX-License-Identifier: MIT
 */

#include "cpu.h"
#include "emu.h"
#include <iostream>

CPU::CPU(unsigned int frequency)
	: ProcessingUnit(frequency)
{
}

CPU::~CPU()
{
}

void CPU::update()
{
	printf("This is an update from the CPU\n");
	Emu::the().writeMemory("RAM", 123, 42);
	printf("fff");
}
