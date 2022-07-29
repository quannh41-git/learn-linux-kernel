##Sysfs entry at 

```/sys/module/passing-arguments/parameters/valueETX```

##Load driver

```sudo insmod hello_world_module.ko valueEtx=14 nameEtx="quannh41" arr_valueEtx=100,102,104,106```

##Verify the parameters by using 

```dmesg```

##Test callback function (module_param_cb)

```sudo sh -c "echo 13 > /sys/module/hello_world_module/parameters/cb_valueETX"```