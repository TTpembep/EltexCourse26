obj-m += chardev.o

KCOMPILER = $(shell grep CONFIG_CC_VERSION /boot/config-$(shell uname -r) | cut -d'"' -f2 | awk '{print $$1}')

all:
	make CC=$(KCOMPILER) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make CC=$(KCOMPILER) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
