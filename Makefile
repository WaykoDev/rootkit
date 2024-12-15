KDIR     := $(PWD)/linux-6.11.5
FS_DIR   := $(PWD)/fs/lib/secret
SRC_DIR  := $(PWD)/src
MODULE   := driver

default: 
	@echo "[+] Building the kernel module with multiple source files..."
	$(MAKE) -C $(KDIR) M=$(SRC_DIR) modules

	@echo "[+] Ensuring proper permissions for the module..."
	@if [ -f $(SRC_DIR)/$(MODULE).ko ]; then chmod 644 $(SRC_DIR)/$(MODULE).ko; else echo "[!] $(MODULE).ko not found!"; exit 1; fi

	@echo "[+] Preparing the destination directory for the module..."
	mkdir -p $(FS_DIR)

	@echo "[+] Copying the module to the filesystem..."
	cp $(SRC_DIR)/$(MODULE).ko $(FS_DIR)

	@echo "[+] Cleaning up build artifacts..."
	$(MAKE) -C $(KDIR) M=$(SRC_DIR) clean

clean:
	@echo "[+] Cleaning up build files..."
	$(MAKE) -C $(KDIR) M=$(SRC_DIR) clean
	rm -f $(FS_DIR)/$(MODULE).ko