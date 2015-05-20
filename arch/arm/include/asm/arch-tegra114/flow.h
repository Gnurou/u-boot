/*
 * Copyright (c) 2010-2013, NVIDIA CORPORATION.  All rights reserved.
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

#ifndef _TEGRA114_FLOW_H_
#define _TEGRA114_FLOW_H_

struct flow_ctlr {
	u32 halt_cpu_events;
	u32 halt_cop_events;
	u32 cpu_csr;
	u32 cop_csr;
	u32 xrq_events;
	u32 halt_cpu1_events;
	u32 cpu1_csr;
	u32 halt_cpu2_events;
	u32 cpu2_csr;
	u32 halt_cpu3_events;
	u32 cpu3_csr;
	u32 cluster_control;
};

/* HALT_COP_EVENTS_0, 0x04 */
#define EVENT_MSEC		(1 << 24)
#define EVENT_USEC		(1 << 25)
#define EVENT_JTAG		(1 << 28)
#define EVENT_MODE_STOP		(2 << 29)

/* CPUn_CSR_0 */
#define CSR_ENABLE		(1 << 0)
#define CSR_IMMEDIATE_WAKE	(1 << 3)
#define CSR_WAIT_WFI_SHIFT	8
#define CSR_PWR_OFF_STS		(1 << 16)

#endif	/* _TEGRA114_FLOW_H_ */
