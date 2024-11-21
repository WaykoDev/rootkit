#include <linux/module.h>  
#include <linux/kernel.h>  
#include <linux/init.h>    

static int __init driver_init(void) {
    printk(KERN_INFO "[+] Module loaded: Hello World! [+]\n");
    return 0;  
}

static void __exit driver_exit(void) {
    printk(KERN_INFO "[+] Module unloaded: Goodbye World! [+]\n");
}

module_init(driver_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wayko");
MODULE_DESCRIPTION("PoC module Hello World");