#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/uaccess.h> //copy_to/from_user()

#define DEVICE_NAME "seven_seg"
char msg[500] = {'0'};
#define MAJOR 255
static int major;
int start_idx = 0;
// static struct class *cls;


static int device_open(struct inode *inode, struct file *file) {
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset) {
    char tousr[16];
    for (int i=0; i<16; ++i) {
        tousr[i] = msg[start_idx++];
    }
    copy_to_user(buffer, tousr, length);
    return 0;
}

static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset) {
    if (copy_from_user(msg, buffer, length)) {
        pr_alert("copy failed\n");
    }
    return length;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

static int __init mydevice_init(void) {
    major = register_chrdev(255, DEVICE_NAME, &fops);
    if (major < 0) {
        pr_alert("Registering the character device failed with %d\n", MAJOR);
        return MAJOR;
    }
    pr_info("I was assigned major number %d.\n", MAJOR);

    // cls = class_create(THIS_MODULE, DEVICE_NAME);
    // device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);

    pr_info("Device created on /dev/%s\n", DEVICE_NAME);
    return 0;
}

static void __exit mydevice_exit(void) {
    // device_destroy(cls, MKDEV(major, 0));
    // class_destroy(cls);
    /*
     * Unregister the device
     */
    unregister_chrdev(MAJOR, DEVICE_NAME);
    pr_info("unregister character device\n");
}

module_init(mydevice_init);
module_exit(mydevice_exit);
MODULE_LICENSE("GPL");
