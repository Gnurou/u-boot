/*
 * Copyright (c) 2015 NVIDIA CORPORATION. All rights reserved.
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <i2c.h>
#include <netdev.h>

#include <asm/arch/gpio.h>
#include <asm/arch/pinmux.h>
#include <asm/gpio.h>

#include "pinmux-config-p2571-e01.h"

/*
 * Routine: pinmux_init
 * Description: Do individual peripheral pinmux configs
 */
void pinmux_init(void)
{
	pinmux_clear_tristate_input_clamping();

	gpio_config_table(p2571_e01_gpio_inits,
			  ARRAY_SIZE(p2571_e01_gpio_inits));

	pinmux_config_pingrp_table(p2571_e01_pingrps,
				   ARRAY_SIZE(p2571_e01_pingrps));

	pinmux_config_drvgrp_table(p2571_e01_drvgrps,
				   ARRAY_SIZE(p2571_e01_drvgrps));
}

/*
 * Routine: start_cpu_fan
 * Description: Enable/start PWM CPU fan on P2530
 */
void start_cpu_fan(void)
{
	/* GPIO_PE4 is PS_VDD_FAN_ENABLE */
	gpio_request(GPIO_PE4, "FAN_VDD");
	gpio_direction_output(GPIO_PE4, 1);
}

#ifdef CONFIG_TEGRA_MMC
void board_mmc_power_init(void)
{
	struct udevice *pmic;
	u8 value;
	int err;

	err = i2c_get_chip_for_busnum(0, 0x3c, 1, &pmic);
	if (err < 0) {
		printf("  i2c_get_chip_for_busnum() failed: %d\n", err);
		return;
	}

	/* SD3 -> 1.8V */
	err = dm_i2c_read(pmic, 0x52, &value, 1);
	if (err < 0) {
		printf("failed to read FPS mode for SD3: %d\n", err);
		return;
	}

	value |= 0xc0;

	err = dm_i2c_write(pmic, 0x52, &value, 1);
	if (err < 0) {
		printf("failed to set FPS mode for SD3: %d\n", err);
		return;
	}

	value = 0x70;
	err = dm_i2c_write(pmic, 0x20, &value, 1);
	if (err < 0) {
		printf("failed to configure SD3: %d\n", err);
		return;
	}

	value = 0x60;
	err = dm_i2c_write(pmic, 0x19, &value, 1);
	if (err < 0) {
		printf("failed to set SD3 to 1.8V: %d\n", err);
		return;
	}

	/* LDO2 -> 3.3V */
	value = 0xf2;
	err = dm_i2c_write(pmic, 0x27, &value, 1);
	if (err < 0) {
		printf("failed to set LDO2 to 3.3V: %d\n", err);
		return;
	}
}
#endif

#ifdef CONFIG_PCI_TEGRA
int tegra_pcie_board_init(void)
{
	struct udevice *pmic;
	u8 value;
	int err;

	err = i2c_get_chip_for_busnum(0, 0x3c, 1, &pmic);
	if (err < 0) {
		printf("  i2c_get_chip_for_busnum() failed: %d\n", err);
		return err;
	}

	/* LDO1 -> 1.05V */
	err = dm_i2c_read(pmic, 0x47, &value, 1);
	if (err < 0)
		return err;

	value |= 0xc0;

	err = dm_i2c_write(pmic, 0x47, &value, 1);
	if (err < 0)
		return err;

	value = 0xca;
	err = dm_i2c_write(pmic, 0x25, &value, 1);
	if (err < 0) {
		printf("failed to set LDO1 to 1.05V: %d\n", err);
		return err;
	}

	/* LDO7 -> 1.05V */
	err = dm_i2c_read(pmic, 0x4d, &value, 1);
	if (err < 0)
		return err;

	value |= 0xc0;

	err = dm_i2c_write(pmic, 0x4d, &value, 1);
	if (err < 0)
		return err;

	value = 0xc5;
	err = dm_i2c_write(pmic, 0x31, &value, 1);
	if (err < 0) {
		printf("failed to set LDO7 to 1.05V: %d\n", err);
		return err;
	}

	/* SD3 -> 1.8V */
	err = dm_i2c_read(pmic, 0x52, &value, 1);
	if (err < 0)
		return err;

	value |= 0xc0;

	err = dm_i2c_write(pmic, 0x52, &value, 1);
	if (err < 0)
		return err;

	value = 0x70;
	err = dm_i2c_write(pmic, 0x20, &value, 1);
	if (err < 0) {
		printf("failed to configure SD3: %d\n", err);
		return err;
	}

	value = 0x60;
	err = dm_i2c_write(pmic, 0x19, &value, 1);
	if (err < 0) {
		printf("failed to set SD3 to 1.8V: %d\n", err);
		return err;
	}

	return 0;
}

int board_eth_init(bd_t *bd)
{
	return pci_eth_init(bd);
}
#endif /* CONFIG_PCI_TEGRA */
