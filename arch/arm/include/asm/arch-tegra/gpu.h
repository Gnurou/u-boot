/*
 *  (C) Copyright 2015
 *  NVIDIA Corporation <www.nvidia.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __ASM_ARCH_TEGRA_GPU_H
#define __ASM_ARCH_TEGRA_GPU_H

#if defined(CONFIG_TEGRA_GPU)

void config_gpu(void);

#else /* CONFIG_TEGRA_GPU */

static inline void config_gpu(void)
{
}

#endif /* CONFIG_TEGRA_GPU */


#if defined(CONFIG_OF_LIBFDT)

int gpu_enable_node(void *blob, const char *gpupath);

#else /* CONFIG_OF_LIBFDT */

static inline int gpu_enable_node(void *blob, const char *gpupath)
{
	return 0;
}

#endif /* CONFIG_OF_LIBFDT */

#endif
