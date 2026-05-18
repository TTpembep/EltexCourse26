/* 
 * chardev.c: Creates a read-only char device that says how many times 
 * you have read from the dev file
 * src: https://sysprog21.github.io/lkmpg/#device-drivers 
 */ 

#include <linux/cdev.h> 
#include <linux/delay.h> 
#include <linux/device.h> 
#include <linux/fs.h> 
#include <linux/init.h> 
#include <linux/irq.h> 
#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/poll.h> 

int len,temp;
/*  Prototypes - this would normally go in a .h file */ 
static int device_open(struct inode *, struct file *); 
static int device_release(struct inode *, struct file *); 
static ssize_t device_read(struct file *filp, char __user *buf, size_t count, loff_t *offp); 
static ssize_t device_write(struct file *filp, const char __user *buff, size_t count, loff_t *off);

#define SUCCESS 0 
#define DEVICE_NAME "chardev" /* Dev name as it appears in /proc/devices   */ 
#define BUF_LEN 80 /* Max length of the message from the device */ 
/* Global variables are declared as static, so are global within the file. */ 

static int major; /* major number assigned to our device driver */ 
enum { 
    CDEV_NOT_USED = 0, 
    CDEV_EXCLUSIVE_OPEN = 1, 
}; 

/* Is device open? Used to prevent multiple access to device */ 
static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED); 
static char msg[BUF_LEN]; /* The msg the device will give when asked */ 
static struct class *cls; 
static struct file_operations chardev_fops = { 
    .read = device_read, 
    .write = device_write, 
    .open = device_open, 
    .release = device_release, 
}; 

static int __init chardev_init(void) { 
    major = register_chrdev(0, DEVICE_NAME, &chardev_fops); 

    if (major < 0) { 
        pr_alert("Registering char device failed with %d\n", major); 
        return major; 
    } 
    pr_info("I was assigned major number %d.\n", major); 

    //cls = class_create(THIS_MODULE, DEVICE_NAME);
    cls = class_create(DEVICE_NAME); 
    device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME); 
    pr_info("Device created on /dev/%s\n", DEVICE_NAME); 
    return SUCCESS; 
} 

static void __exit chardev_exit(void){ 
    device_destroy(cls, MKDEV(major, 0)); 
    class_destroy(cls); 

	/* Unregister the device */ 
    unregister_chrdev(major, DEVICE_NAME); 
} 

/* Methods */ 
/* Called when a process tries to open the device file, like 
 * "sudo cat /dev/chardev" 
 */ 
static int device_open(struct inode *inode, struct file *file){ 
    //static int counter = 0; 

    if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN)) 
        return -EBUSY; 

    //sprintf(msg, "I already told you %d times Hello world!\n", counter++); 
    try_module_get(THIS_MODULE); 
    return SUCCESS; 
} 

/* Called when a process closes the device file. */ 
static int device_release(struct inode *inode, struct file *file){ 
    /* We're now ready for our next caller */ 
    atomic_set(&already_open, CDEV_NOT_USED); 
    /* Decrement the usage count, or else once you opened the file, you will 
     * never get rid of the module. 
     */ 
    module_put(THIS_MODULE); 
    return SUCCESS; 
} 

/* Called when a process, which already opened the dev file, attempts to 
 * read from it. 
 */ 
static ssize_t device_read(struct file *filp, /* see include/linux/fs.h   */ 
                           char __user *buf, /* buffer to fill with data */ 
                           size_t count, /* length of the buffer     */ 
                           loff_t *offp) { /* Number of bytes actually written to the buffer */
	printk(KERN_INFO "Entering READ func\n");
	/*
    int bytes_read = 0; 
    const char *msg_ptr = msg; 
    if (!*(msg_ptr + *offset)) { // we are at the end of message
        *offset = 0; // reset the offset
        return 0; // signify end of file
    } 
    msg_ptr += *offset; */
    /* Actually put the data into the buffer */ 
    //while (length && *msg_ptr) { 
        /* The buffer is in the user data segment, not the kernel 
         * segment so "*" assignment won't work.  We have to use 
         * put_user which copies data from the kernel data segment to 
         * the user data segment. 
         */ 
        //put_user(*(msg_ptr++), buffer++); 
        //length--; 
       // bytes_read++; 
    //} 
    //*offset += bytes_read; 
    /* Most read functions return the number of bytes put into the buffer. */ 
    //return bytes_read;
    if(count > temp) {
        count = temp;
    }
    temp = temp - count;
    (void)!copy_to_user(buf, msg, count);
    if(count == 0)
        temp = len;
    return count;
} 

/* Called when a process writes to dev file: echo "hi" > /dev/hello */ 
static ssize_t device_write(struct file *filp, const char __user *buff, 
                            size_t count, loff_t *off){ 
    //pr_alert("Sorry, this operation is not supported.\n"); 
    //return -EINVAL; 
    (void)!copy_from_user(msg, buff, count);
    len = count;
    temp = len;
    return count;
} 

module_init(chardev_init); 
module_exit(chardev_exit); 
MODULE_LICENSE("GPL");
