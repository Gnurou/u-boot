/*
 * Copyright (c) 2014-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Tegra vpr routines */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/tegra.h>
#include <asm/arch/mc.h>

#include <fdt_support.h>

DECLARE_GLOBAL_DATA_PTR;

static bool _configured;

static void config_vpr(void)
{
	struct mc_ctlr *mc = (struct mc_ctlr *)NV_PA_MC_BASE;

	/* Turn VPR off */
	writel(0, &mc->mc_video_protect_size_mb);
	writel(TEGRA_MC_VIDEO_PROTECT_REG_WRITE_ACCESS_DISABLED,
	       &mc->mc_video_protect_reg_ctrl);
	/* read back to ensure the write went through */
	readl(&mc->mc_video_protect_reg_ctrl);

	debug("configured VPR\n");
}

#if defined(CONFIG_TEGRA210)
static void config_wpr(void)
{
	struct mc_ctlr *mc = (struct mc_ctlr *)NV_PA_MC_BASE;
	u64 wpr_start = NV_PA_SDRAM_BASE + gd->ram_size;
	u32 reg;

	/*
	 * Carveout2 uses the upper 256KB of upper memory that we reserved as
	 * WPR region for secure firmware loading
	 */
	writel(lower_32_bits(wpr_start), &mc->mc_security_carveout2_bom);
	writel(upper_32_bits(wpr_start), &mc->mc_security_carveout2_bom_hi);
	writel(0x2, &mc->mc_security_carveout2_size_128k);
	reg = readl(&mc->mc_security_carveout2_cfg0);
	reg |= TEGRA_MC_SECURITY_CARVEOUT_CFG_LOCKED;
	writel(reg, &mc->mc_security_carveout2_cfg0);

	/* Carveout3 is left empty */
	writel(0x0, &mc->mc_security_carveout3_bom);
	writel(0x0, &mc->mc_security_carveout3_bom_hi);
	writel(0x0, &mc->mc_security_carveout3_size_128k);
	reg = readl(&mc->mc_security_carveout3_cfg0);
	reg |= TEGRA_MC_SECURITY_CARVEOUT_CFG_LOCKED;
	writel(reg, &mc->mc_security_carveout3_cfg0);

	/* read back to ensure the write went through */
	readl(&mc->mc_security_carveout3_cfg0);

	debug("configured WPR\n");
}
#else
static inline void config_wpr(void)
{
}
#endif

void tegra_gpu_config(void)
{
	config_vpr();
	config_wpr();

	_configured = true;
}

#if defined(CONFIG_OF_LIBFDT)

int tegra_gpu_enable_node(void *blob, const char *gpupath)
{
	int offset;

	if (_configured) {
		offset = fdt_path_offset(blob, gpupath);
		if (offset > 0) {
			fdt_status_okay(blob, offset);
			debug("enabled GPU node %s\n", gpupath);
		}
	}

	return 0;
}

#endif
