/*
 * Copyright (c) 2018, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>

#include <platform_def.h>

#include <arch_helpers.h>
#include <lib/bakery_lock.h>
#include <lib/mmio.h>

#include <sq_common.h>
#include "sq_mhu.h"

/* SCP MHU secure channel registers */
#define SCP_INTR_S_STAT		0x200
#define SCP_INTR_S_SET		0x208
#define SCP_INTR_S_CLEAR	0x210

/* CPU MHU secure channel registers */
#define CPU_INTR_S_STAT		0x300
#define CPU_INTR_S_SET		0x308
#define CPU_INTR_S_CLEAR	0x310

DEFINE_BAKERY_LOCK(sq_lock);

/*
 * Slot 31 is reserved because the MHU hardware uses this register bit to
 * indicate a non-secure access attempt. The total number of available slots is
 * therefore 31 [30:0].
 */
#define MHU_MAX_SLOT_ID		30

void mhu_secure_message_start(unsigned int slot_id)
{
	assert(slot_id <= MHU_MAX_SLOT_ID);

	bakery_lock_get(&sq_lock);

	/* Make sure any previous command has finished */
	while (mmio_read_32(PLAT_SQ_MHU_BASE + CPU_INTR_S_STAT) &
							(1 << slot_id))
		;
}

void mhu_secure_message_send(unsigned int slot_id)
{
	assert(slot_id <= MHU_MAX_SLOT_ID);
	assert(!(mmio_read_32(PLAT_SQ_MHU_BASE + CPU_INTR_S_STAT) &
							(1 << slot_id)));

	/* Send command to SCP */
	mmio_write_32(PLAT_SQ_MHU_BASE + CPU_INTR_S_SET, 1 << slot_id);
}

uint32_t mhu_secure_message_wait(void)
{
	uint32_t response;

	/* Wait for response from SCP */
	while (!(response = mmio_read_32(PLAT_SQ_MHU_BASE + SCP_INTR_S_STAT)))
		;

	return response;
}

void mhu_secure_message_end(unsigned int slot_id)
{
	assert(slot_id <= MHU_MAX_SLOT_ID);

	/*
	 * Clear any response we got by writing one in the relevant slot bit to
	 * the CLEAR register
	 */
	mmio_write_32(PLAT_SQ_MHU_BASE + SCP_INTR_S_CLEAR, 1 << slot_id);

	bakery_lock_release(&sq_lock);
}

void mhu_secure_init(void)
{
	bakery_lock_init(&sq_lock);

	/*
	 * The STAT register resets to zero. Ensure it is in the expected state,
	 * as a stale or garbage value would make us think it's a message we've
	 * already sent.
	 */
	assert(mmio_read_32(PLAT_SQ_MHU_BASE + CPU_INTR_S_STAT) == 0);
}

void plat_sq_pwrc_setup(void)
{
	mhu_secure_init();
}
