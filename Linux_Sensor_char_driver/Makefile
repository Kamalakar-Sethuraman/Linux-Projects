obj-m += day_11/sensor_char.o
obj-m += day_12/sensor_char.o
obj-m += day_13/sensor_char.o
obj-m += day_16/sensor_char.o
obj-m += day_17/sensor_char.o
obj-m += day_18/sensor_char.o
obj-m += upgrade-1/sensor_char.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
