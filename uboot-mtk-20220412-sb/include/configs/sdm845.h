/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Configuration file for boards, based on Qualcomm SDM845 chip
 *
 * (C) Copyright 2021 Dzmitry Sankouski <dsankouski@gmail.com>
 */

#ifndef __CONFIGS_SDM845_H
#define __CONFIGS_SDM845_H

#include <linux/sizes.h>
#include <asm/arch/sysmap-sdm845.h>

#define CONFIG_SYS_BAUDRATE_TABLE	{ 115200, 230400, 460800, 921600 }

/* Generic Timer Definitions */
#define COUNTER_FREQUENCY	19000000

#define CONFIG_EXTRA_ENV_SETTINGS \
	"bootm_size=0x4000000\0"	\
	"bootm_low=0x80000000\0"	\
	"stdout=vidconsole\0"	\
	"stderr=vidconsole\0"	\
	"preboot=source $prevbl_initrd_start_addr:prebootscript\0" \
	"bootcmd=source $prevbl_initrd_start_addr:bootscript\0"

/* Size of malloc() pool */
#define CONFIG_SYS_BOOTM_LEN	SZ_64M

/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE	512
#define CONFIG_SYS_MAXARGS	64

#endif
