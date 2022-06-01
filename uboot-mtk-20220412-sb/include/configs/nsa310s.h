/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2015, 2021 Tony Dinh <mibodhi@gmail.com>
 * Copyright (C) 2015
 * Gerald Kerma <dreagle@doukki.net>
 * Luka Perkov <luka.perkov@sartura.hr>
 */

#ifndef _CONFIG_NSA310S_H
#define _CONFIG_NSA310S_H

#include "mv-common.h"

/* environment variables configuration */

/* default environment variables */

#define CONFIG_EXTRA_ENV_SETTINGS \
	"console=console=ttyS0,115200\0" \
	"mtdids=nand0=orion_nand\0" \
	"mtdparts="CONFIG_MTDPARTS_DEFAULT \
	"kernel=/boot/zImage\0" \
	"fdt=/boot/nsa310s.dtb\0" \
	"bootargs_root=ubi.mtd=3 root=ubi0:rootfs rootfstype=ubifs rw\0"

/* Ethernet driver configuration */
#ifdef CONFIG_CMD_NET
#define CONFIG_MVGBE_PORTS	{1, 0}	/* enable port 0 only */
#define CONFIG_PHY_BASE_ADR	1
#endif /* CONFIG_CMD_NET */

/* SATA driver configuration */
#ifdef CONFIG_SATA
#define CONFIG_LBA48
#endif /* CONFIG_SATA */

#endif /* _CONFIG_NSA310S_H */
