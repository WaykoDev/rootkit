# Glactus

### Disclaimer ###

Galactus is an experimental LKM rootkit for Linux Kernel 6.X (x84_64).

This rookit are by no mean real-world rootkit. Galactus have been kept to the minimum and should be used for educational purposes only. Use it at your own risks.

### Before you start ###

- Don't use these tools to do stupid things like hacking without consent.
- Read books, manuals, articles, be curious and not just a [script kiddie](https://gettinggeek.com/how-not-to-become-a-script-kiddie/).
- Don't use these tools for illegal purposes.

### Installation ###

Verify if the kernel is compatible:
```bash
~ uname -r
6.8.0-47-generic
```

Download the latest stable release from The [Linux Kernel Archives](https://kernel.org/) :
```bash
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.11.tar.xz
tar -xvf linux-6.11.tar.xz
cd linux-6.11
make defconfig
make -j$(nproc) bzImage
```

```
sudo apt update
sudo apt install -y qemu git curl grub2 docker docker.io losetup parted build-essential
```




