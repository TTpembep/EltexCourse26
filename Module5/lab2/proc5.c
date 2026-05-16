#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define FILE_NAME "hello"
#define MAGIC_NUM 10
#define MODE 0

ssize_t read_proc(struct file *filp, char *buf, size_t count, loff_t *offp );
ssize_t write_proc(struct file *filp, const char *buf, size_t count, loff_t *offp);
void create_new_proc_entry(void);
int proc_init (void);
void proc_cleanup(void);

int len,temp;
char *msg;
 
ssize_t read_proc(struct file *filp, char *buf, size_t count, loff_t *offp ) {
    if(count > temp) {
        count = temp;
    }
    temp = temp - count;
    (void)!copy_to_user(buf, msg, count);
    if(count == 0)
        temp = len;
    return count;
}
 
ssize_t write_proc(struct file *filp, const char *buf, size_t count, loff_t *offp) {
    (void)!copy_from_user(msg, buf, count);
    len = count;
    temp = len;
    return count;
}
 
static const struct proc_ops proc_fops = {
    proc_read: read_proc,
    proc_write: write_proc,
};
 
void create_new_proc_entry(void) { //use of void for no arguments is compulsory now
    proc_create(FILE_NAME, MODE, NULL, &proc_fops);
    msg = kmalloc(MAGIC_NUM * sizeof(char), GFP_KERNEL);
}

int proc_init (void) {
    create_new_proc_entry();
    return 0;
}
 
void proc_cleanup(void) {
    remove_proc_entry(FILE_NAME, NULL);
    kfree(msg);
}
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TTpembep");
MODULE_DESCRIPTION("Example of communication between user and kernel- space using proc.");
module_init(proc_init);
module_exit(proc_cleanup);
