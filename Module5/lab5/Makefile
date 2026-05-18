obj-m += netlink.o

KCOMPILER = $(shell grep CONFIG_CC_VERSION /boot/config-$(shell uname -r) | cut -d'"' -f2 | awk '{print $$1}')

all:
	gcc user.c -o user
	make CC=$(KCOMPILER) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	rm user
	make CC=$(KCOMPILER) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
