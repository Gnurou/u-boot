/*
 * Copyright (c) 2013,2015 NVIDIA CORPORATION. All rights reserved.
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

/* Tegra210 high-level function multiplexing */

#include <common.h>
#include <asm/arch/clock.h>
#include <asm/arch/funcmux.h>
#include <asm/arch/pinmux.h>

int funcmux_select(enum periph_id id, int config)
{
	int bad_config = config != FUNCMUX_DEFAULT;

	switch (id) {
	case PERIPH_ID_UART4:
		switch (config) {
		case FUNCMUX_UART4_GPIO: /* TXD,RXD,CTS,RTS */
			pinmux_set_func(PMUX_PINGRP_UART4_TX_PI4, PMUX_FUNC_UARTD);
			pinmux_set_func(PMUX_PINGRP_UART4_RX_PI5, PMUX_FUNC_UARTD);
			pinmux_set_func(PMUX_PINGRP_UART4_RTS_PI6, PMUX_FUNC_UARTD);
			pinmux_set_func(PMUX_PINGRP_UART4_CTS_PI7, PMUX_FUNC_UARTD);

			pinmux_set_io(PMUX_PINGRP_UART4_TX_PI4, PMUX_PIN_OUTPUT);
			pinmux_set_io(PMUX_PINGRP_UART4_RX_PI5, PMUX_PIN_INPUT);
			pinmux_set_io(PMUX_PINGRP_UART4_RTS_PI6, PMUX_PIN_OUTPUT);
			pinmux_set_io(PMUX_PINGRP_UART4_CTS_PI7, PMUX_PIN_INPUT);

			pinmux_tristate_disable(PMUX_PINGRP_UART4_TX_PI4);
			pinmux_tristate_disable(PMUX_PINGRP_UART4_RX_PI5);
			pinmux_tristate_disable(PMUX_PINGRP_UART4_RTS_PI6);
			pinmux_tristate_disable(PMUX_PINGRP_UART4_CTS_PI7);
			break;
		}
		break;

	case PERIPH_ID_UART1:
		switch (config) {
		case FUNCMUX_UART1_KBC:
			pinmux_set_func(PMUX_PINGRP_UART1_TX_PU0,
					PMUX_FUNC_UARTA);
			pinmux_set_func(PMUX_PINGRP_UART1_RX_PU1,
					PMUX_FUNC_UARTA);

			pinmux_set_io(PMUX_PINGRP_UART1_TX_PU0, PMUX_PIN_OUTPUT);
			pinmux_set_io(PMUX_PINGRP_UART1_RX_PU1, PMUX_PIN_INPUT);

			pinmux_tristate_disable(PMUX_PINGRP_UART1_TX_PU0);
			pinmux_tristate_disable(PMUX_PINGRP_UART1_RX_PU1);
			break;
		}
		break;

	/* Add other periph IDs here as needed */

	default:
		debug("%s: invalid periph_id %d", __func__, id);
		return -1;
	}

	if (bad_config) {
		debug("%s: invalid config %d for periph_id %d", __func__,
		      config, id);
		return -1;
	}
	return 0;
}
