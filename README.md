Hello this is my first rootkit build with heart.

This rootkit is based on an alpine image.

### Requierements

```bash
sudo apt update
sudo apt install -y qemu grub2 docker docker.io losetup parted build-essential
```

We also need to download the kernel and compile it:

```bash
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.11.tar.xz
tar -xvf linux-6.11.tar.xz
cd linux-6.11
make defconfig
make -j$(nproc) bzImage

```
