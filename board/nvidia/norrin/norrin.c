/*
 * (C) Copyright 2014
 * NVIDIA Corporation <www.nvidia.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <i2c.h>
#include <netdev.h>
#include <power/as3722.h>

#include <asm/arch/gpio.h>
#include <asm/arch/pinmux.h>

#include "pinmux-config-norrin.h"

DECLARE_GLOBAL_DATA_PTR;

/*
 * Routine: pinmux_init
 * Description: Do individual peripheral pinmux configs
 */
void pinmux_init(void)
{
	pinmux_set_tristate_input_clamping();

	gpio_config_table(norrin_gpio_inits,
			  ARRAY_SIZE(norrin_gpio_inits));

	pinmux_config_pingrp_table(norrin_pingrps,
				   ARRAY_SIZE(norrin_pingrps));

	pinmux_config_drvgrp_table(norrin_drvgrps,
				   ARRAY_SIZE(norrin_drvgrps));
}

#ifdef CONFIG_TEGRA_MMC
void board_mmc_power_init(void)
{
	struct udevice *pmic;
	u8 value;
	int err;

	printf("> %s()\n", __func__);

	err = i2c_get_chip_for_busnum(0, 0x40, 1, &pmic);
	if (err < 0) {
		printf("i2c_get_chip_for_busnum() failed: %d\n", err);
		return;
	}

	value = 0x10;

	err = dm_i2c_write(pmic, 0x12, &value, 1);
	if (err < 0) {
		printf("failed to set LDO6 to 3.3V: %d\n", err);
		return;
	}

	err = dm_i2c_read(pmic, 0x4e, &value, 1);
	if (err < 0) {
		printf("failed to read LDO6 control register: %d\n", err);
		return;
	}

	value |= 1 << 2;

	err = dm_i2c_write(pmic, 0x4e, &value, 1);
	if (err < 0) {
		printf("failed to write LDO6 control register: %d\n", err);
		return;
	}

	printf("< %s()\n", __func__);
}
#endif

#ifdef CONFIG_PCI_TEGRA
int tegra_pcie_board_init(void)
{
	struct udevice *pmic;
	int err;

	err = as3722_init(&pmic);
	if (err) {
		error("failed to initialize AS3722 PMIC: %d\n", err);
		return err;
	}

	err = as3722_sd_enable(pmic, 4);
	if (err < 0) {
		error("failed to enable SD4: %d\n", err);
		return err;
	}

	err = as3722_sd_set_voltage(pmic, 4, 0x24);
	if (err < 0) {
		error("failed to set SD4 voltage: %d\n", err);
		return err;
	}

	err = as3722_gpio_configure(pmic, 1, AS3722_GPIO_OUTPUT_VDDH |
					     AS3722_GPIO_INVERT);
	if (err < 0) {
		error("failed to configure GPIO#1 as output: %d\n", err);
		return err;
	}

	err = as3722_gpio_direction_output(pmic, 2, 1);
	if (err < 0) {
		error("failed to set GPIO#2 high: %d\n", err);
		return err;
	}

	return 0;
}

int board_eth_init(bd_t *bis)
{
	return pci_eth_init(bis);
}
#endif /* PCI */
