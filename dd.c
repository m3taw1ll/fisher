/* Custom Device Driver: Interfacing the device and the kernel */

#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <asm/uaccess.h>
#include <linux/init.h>

#define count 1
#define ma 234
#define mi 0

const char fisher;
volatile static int dev_open = 0;

struct fisher_dev
{
        unsigned long size;
        const char buff;
        static char message[1024];
        struct cdev cdev;
};

static const struct file_operations fisher_ops = {
        .owner = THIS_MODULE,
        .read = fisher_read
};

static int fisher_open(struct inode *inode, struct file *file)
{
        if(dev_open)
                return -EBUSY;

        struct fisher_dev *dev = container_of(inode->i_cdev, struct fisher_dev, cdev);
        file->private_data = dev;
        return 0;
}

static int fisher_release(struct inode *inode, struct file *file)
{
        dev_open--;
        return 0;
}

static int fisher_read(struct file *file,
                       char __user *buffer,
                       size_t size,
                       loff_t *offset)
{
        struct fisher_dev *dev = file->private_data;

}

static int fisher_write(struct file *file,
                        const char __user *buffer,
                        size_t size,
                        loff_t *offset)
{
        return -EINVAL;
}

/*--------------------------------------------------------------------
 *              ------ init and exit functions ------
 *--------------------------------------------------------------------
 */

static int __init fisher_init(void) {

        int retval;
        retval = register_chrdev_region(MKDEV(ma, mi), count, "fisher");
        if (retval != 0) {
                printk(KERN_ALERT "Device Registration Error");
                return -1;
        }

        printk(KERN_ALERT "Fisher Successfully Registered.\n");
        return 0;

}

static void __exit fisher_exit(void) {

        unregister_chrdev_region(MKDEV(ma, mi), count);
        printk(KERN_ALERT "Module Successfully Removed");

}

/* ONLY introduce the cdev_add & cdev_init when the driver is able to fully handle operations on the device */
/* The cdev_add & cdev_init functions should reside in a separate function and not in main */

MODULE_LICENSE("GPL");
module_init(fisher_init);
module_exit(fisher_exit);
