- Device associates with number range will appear in ```/proc/devices``` and ```sysfs```

- ```dev_t``` defined in ```<linux/types.h>``` is used to hold device numbers, ```dev_t``` is 32 bit (12 bit for `major` and 20 bit for `minor`)