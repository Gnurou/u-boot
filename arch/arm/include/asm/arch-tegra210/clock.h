/*
 * Copyright (c) 2010-2013,2015 NVIDIA CORPORATION. All rights reserved.
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

/* Tegra210 clock control definitions */

#ifndef _TEGRA210_CLOCK_H_
#define _TEGRA210_CLOCK_H_

#include <asm/arch-tegra/clock.h>

/* CLK_RST_CONTROLLER_OSC_CTRL_0 */
#define OSC_FREQ_SHIFT          28
#define OSC_FREQ_MASK           (0xF << OSC_FREQ_SHIFT)

int tegra_plle_enable(void);

#endif /* _TEGRA210_CLOCK_H_ */
