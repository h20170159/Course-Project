obj-m := driver.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
PROGRAM = driver$(EXE_SUFFIX)

default:

	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) 
clean:
	rm -f *.o $(PROGRAM)
