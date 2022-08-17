/*
 * Copyright (C) 2022 Riyyi
 *
 * SPDX-License-Identifier: MIT
 */

#include "macro.h"
#include "testcase.h"
#include "testsuite.h"

// -----------------------------------------

TEST_CASE(Thing)
{
	int a = 5;
	EXPECT_EQ(a, 5);
}
