#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>
#include "als.h"

void main(void)
{
	struct sensor_value intensity;
	printk("OOTD -  Ambient Light Sensor sample code\n");
	als_init(DEVICE_DT_GET(DT_ALIAS(als)));

	while (true) {
		if (als_read(&intensity))
			printk("Error Reading Ambient Light Sensor");
		printk("Ambient Light Intensity %d\n", intensity.val1);
		k_sleep(K_MSEC(1000));
	}
}