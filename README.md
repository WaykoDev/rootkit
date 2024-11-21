# Glactus

### Disclaimer ###

Galactus is an experimental LKM rootkit inside qemu for Linux Kernel (x84_64).

This rookit are by no mean real-world rootkit. Galactus have been kept to the minimum and should be used for educational purposes only. Use it at your own risks.

### Before you start ###

- Don't use these tools to do stupid things like hacking without consent.
- Read books, manuals, articles, be curious and not just a [script kiddie](https://www.wikihow.com/Avoid-Becoming-a-Script-Kiddie).
- Don't use these tools for illegal purposes.

### Repository Structure ###

The repository is structured as follows:

```bash
rootkit/
├── fs/              # System files to be included in the root filesystem of the VM
├── src/             # Source files (.c) for the rootkit and additional modules
├── linux-<version>/ # Source of the kernel
├── setup.sh         # Script to set up the project by creating symbolic links
├── rootkit.sh       # Main script to create, update, mount, unmount, and start the rootkit VM
└── Makefile         # Makefile for building the rootkit module from src/
```

`fs/`: Contains files that will be copied into the VM’s root filesystem (e.g., the compiled `rootkit.ko` module).
`src/`: This folder contains all `.c` files and other source code needed to build the rootkit module.

### Kernel Requirements ###

To use Galactus, you need to clone and build the [Linux Kernel](https://kernel.org/). This ensures compatibility and allows you to run a custom kernel in QEMU.

### Installation ###

**Debian/Ubuntu:**
```bash
sudo apt update && \
sudo apt install -y build-essential linux-headers-$(uname -r) qemu-system grub2 docker parted losetup
```

Clone the project and setup it:
```bash
git clone https://github.com/WaykoDev/rootkit.git && \
cd rootkit && \
./setup.sh
```

### Usage ###

Build and install the rootkit module:
```bash
make
make install
```

Create a disk image:
```bash
rootkit create
```

Update the disk image with the rootkit:
```bash
rootkit update
```

Access the disk image directly:
```bash
rootkit mount
```

>This will mount the disk image to a temporary folder (default: `/tmp/my-rootfs`), allowing you to modify and inspect its contents.

Unmount the disk image after making changes:
```bash
rootkit umount
```

Start the VM:
```bash
rootkit start
```

### Notes ###

- Make sure the kernel is built and accessible for your VM setup before starting the disk creation process.
- Mounting and unmounting the disk is particularly useful for debugging or adding files manually before starting the VM.
- Ensure you properly unmount the disk image (`rootkit umount`) before running `rootkit start` to avoid corruption or conflicts.
- Always test the rootkit in an `isolated` environment, like QEMU, to avoid affecting your host system.