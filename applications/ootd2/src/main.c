/*
 * Copyright (c) 2019 Nordic Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "opt3001.h"
#include <stdio.h>
#include <zephyr/kernel.h>

const struct device *dev;

static void user_entry(void *p1, void *p2, void *p3)
{
	opt3001_print(dev);
}

void main(void)
{
	printk("Hello World from the app!\n");

	dev = device_get_binding("CUSTOM_DRIVER");

	__ASSERT(dev, "Failed to get device binding");

	printk("device is %p, name is %s\n", dev, dev->name);

	k_object_access_grant(dev, k_current_get());
	k_thread_user_mode_enter(user_entry, NULL, NULL, NULL);
}
