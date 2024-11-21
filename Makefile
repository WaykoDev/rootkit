KDIR	:= $(PWD)/linux-6.11.5
FS_DIR	:= $(PWD)/fs/lib/secret
SRC_DIR	:= $(PWD)/src

default: 
	$(MAKE) -C $(KDIR) M=$(SRC_DIR) modules
	chmod 644 $(SRC_DIR)/driver.ko
	mkdir -p $(FS_DIR)
	cp $(SRC_DIR)/driver.ko $(FS_DIR)
	$(MAKE) -C $(KDIR) M=$(SRC_DIR) clean
clean:
	$(MAKE) -C $(KDIR) M=$(SRC_DIR) clean