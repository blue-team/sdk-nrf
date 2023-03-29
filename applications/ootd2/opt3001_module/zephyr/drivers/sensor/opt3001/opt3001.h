/*
 * Copyright (c) 2019 Nordic Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __OPT3001_H__
#define __OPT3001_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <zephyr/device.h>

/*
 * This 'Hello World' driver has a 'print' syscall that prints the
 * famous 'Hello World!' string.
 *
 * The string is formatted with some internal driver data to
 * demonstrate that drivers are initialized during the boot process.
 *
 * The driver exists to demonstrate (and test) custom drivers that are
 * maintained outside of Zephyr.
 */
__subsystem struct opt3001_driver_api {
	/* This struct has a member called 'print'. 'print' is function
	 * pointer to a function that takes 'struct device *dev' as an
	 * argument and returns 'void'.
	 */
	void (*print)(const struct device *dev);
};

__syscall     void        opt3001_print(const struct device *dev);
static inline void z_impl_opt3001_print(const struct device *dev)
{
	const struct opt3001_driver_api *api = dev->api;

	__ASSERT(api->print, "Callback pointer should not be NULL");

	api->print(dev);
}

#ifdef __cplusplus
}
#endif

#include <syscalls/opt3001.h>

#endif /* __OPT3001_H__ */
