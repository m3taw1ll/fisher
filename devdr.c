/* Custom Device Driver */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <asm/uaccess.h>

#define count 1         /* Number of devices */
#define ma 234          /*   Free major number */
#define mi 0            /*   Base minor number */

const char fisher;


struct fisher_dev
{
        unsigned long size;
        const void dev_buff;
        struct cdev cdev;
};

static const struct file_operations fisher_fops = {
        .owner = THIS_MODULE,
        .open = fisher_open,
        .write = fisher_write,
        .read = fisher_read
};

static int fisher_open(struct inode *inode, struct file *file)
{
        struct fisher_dev *dev;
        dev = container_of(inode->i_cdev, struct fisher_dev, cdev);
        file->private_data = dev;
}

static int fisher_release(struct inode *inode, struct file *file)
{
        return 0;
}

static int fisher_write(struct file *file, const char __user *buff, size_t size, loff_t *offset)
{
        struct fisher_dev *dev_data = (struct fisher_dev *) file->private_data;
        ssize_t len = min(dev_data->size - *offset, size);

        if(len <= 0 ) {
                return 0;
        }
        raw_copy_from_user(void dev_buff, const void buff, unsigned long count);

        if(raw_copy_from_user(dev_data->buff, buff + *offset, len)) {
                return -EFAULT;
        }
        *offset += len;

        return len;
}

static int fisher_read(struct file *file, char __user *buff, size_t size, loff_t *offset)
{
       struct fisher_dev *dev_data = (struct fisher_dev *) file->private_data;
        ssize_t len = min(dev_data->size - *offset, size);

        if(len <= 0)
        { return 0; }
        raw_copy_to_user(void buff, const void dev_buff, unsigned long count);

        if(raw_copy_to_user(buff, dev_data->dev_buff + *offset, len))
                return -EFAULT;

        *offset += len;

        return len;
}

static int __init fisher_init(void)
{
        int err, res;
        err = register_chrdev_region(MKDEV(ma, mi), count, "fisher");
        if (err != 0)
        {
                printk(KERN_ALERT "Device Registration Error");
                return err;
        }
        printk(KERN_ALERT "Fisher Successfully Registered an Identifier.\n");

        cdev_init(&fisher_dev.cdev, &fisher_fops);
        res = cdev_add(&fisher_dev.cdev, MKDEV(ma, mi), count);
        if (res != 0)
        {
                printk(KERN_ALERT "Error");
                return res;
        }
        else
                return 0;

        return 0;
}

static void __exit fisher_exit(void)
{
        unregister_chrdev_region(MKDEV(ma, mi), count);
        printk(KERN_ALERT "Module Successfully Removed");
        cdev_del(&dev.cdev);
}


MODULE_LICENSE("GPL");
module_init(fisher_init);
module_exit(fisher_exit);


