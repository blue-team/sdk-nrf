
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>

const struct device *dev;

int als_read (struct sensor_value *intensity){
    if (dev == NULL)
        return -EINVAL;

        // Fetch sensor data
    if (sensor_sample_fetch(dev)) {
		printk("sensor_sample_fetch failed\n");
        return -EIO;
	}
        
    // Read sensor ambient light intensity
    if (sensor_channel_get(dev, SENSOR_CHAN_LIGHT, intensity)){
       	printk("sensor_channel_get failed\n");
        return -EIO; 
    };
    return 0;
}

int als_init (const struct device *target_device){
	printk("Ambient Light Sensor Init\n");

    dev = target_device;
    if (dev == NULL)
        return -EINVAL;

    // Check device ready
	if (!device_is_ready(dev)) {
		printk("sensor: device not ready.\n");
		return -ENODEV;
	}

    struct sensor_value intensity;
    if(als_read(&intensity)){
		printk("als_read failed\n");
        return -EIO;
	}

    return 0;
}