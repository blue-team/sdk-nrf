#ifndef ALS_H
#define ALS_H

int als_init (const struct device *target_device);
int als_read (struct sensor_value *intensity);

#endif //ALS_H