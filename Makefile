KDIR     := $(PWD)/linux-6.11.5
FS_DIR   := $(PWD)/fs/lib/secret
SRC_DIR  := $(PWD)/src

default: 
	@echo "[+] Building the kernel module..."
	$(MAKE) -C $(KDIR) M=$(SRC_DIR) modules

	@echo "[+] Ensuring proper permissions for the module..."
	@if [ -f $(SRC_DIR)/driver.ko ]; then chmod 644 $(SRC_DIR)/driver.ko; else echo "[!] driver.ko not found!"; exit 1; fi

	@echo "[+] Preparing the destination directory for the module..."
	mkdir -p $(FS_DIR)

	@echo "[+] Copying the module to the filesystem..."
	cp $(SRC_DIR)/driver.ko $(FS_DIR)

	@echo "[+] Cleaning up build artifacts..."
	$(MAKE) -C $(KDIR) M=$(SRC_DIR) clean

clean:
	@echo "[+] Cleaning up build files..."
	$(MAKE) -C $(KDIR) M=$(SRC_DIR) clean