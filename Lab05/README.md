# Kernel Module

## What is a kernel module ?

Modules are pieces of code that can be loaded and unloaded into the kernel upon demand. They extend the functionality of the kernel without the need to recompile and reboot the system.<br/>
Ex. One type of module is the device driver, which allows the kernel to access hardware connected to the system.Without modules, we would have to build monolithic kernels and add new functionality directly into the kernel image.


## Creating a kernel module
To build a kernel module, we need “gcc”, “make” and linux header files. By default, Ubuntu installed these for us.<br/>
However, if your OS doesn’t have, install gcc, make and linux header files by yourself.

### Simplest module: hello_module.c
<img src="https://user-images.githubusercontent.com/82155803/136664254-60aa93cf-c34d-4cd2-ab1d-60734884f190.png" width="70%">

### hello_module.c
<img src="https://user-images.githubusercontent.com/82155803/136665116-54e0c867-8ab7-42f5-a02f-fb65f2e5fae0.png" width="70%">

### Makefile
<img src="https://user-images.githubusercontent.com/82155803/136664562-44c1b30d-eabe-4459-97ec-2b2e6944054b.png" width="70%">

### Instructions

#### make
Use "make" to make the module.
```
$ make
```

#### modinfo
Use "modinfo" to see the module information.
```
$ modinfo hello_module.ko
```

#### insmod
Use "insmod" to insert module into kernel (root needed).
```
$ sudo insmod hello_module.ko
```

#### lsmod
Use "lsmod" to list modules.
```
$ lsmod
```

#### dmesg
Use "dmesg" to see the log file.
```
$ dmesg
```
(You can use "dmesg | tail" to view tail only.)
