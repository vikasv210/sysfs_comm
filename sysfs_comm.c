#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/unistd.h>

/* linux kernel object */
static struct kobject *sysfs_kobject;
/* variable to store the value read from the file */
static int sysfs_value;

/*
 * sysfs_show : API that will get called when the file is read in sysfs
 * arguments  : kobj --> kernel object
 *              attr --> attribute
 *              buf  --> the buffer
 */
static ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
        return sprintf(buf, "%d\n", sysfs_value);
}

/*
 * sysfs_store : API that will get called when the file is written in sysfs
 * arguments   : kobj  --> kernel object
 *               attr  --> attribute
 *               buf   --> the buffer
 *               count --> size of the buffer
 */
static ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf, size_t count)
{
        sscanf(buf, "%d", &sysfs_value);
	printk("\nbuf = %s", buf);
        return count;
}

/*
 * sysfs_value   --> name
 * 0755          --> read and write permission
 * sysfs_show    --> show pointer
 * sysfs_store   --> store pointer
 */
static struct kobj_attribute sysfs_attribute =__ATTR(sysfs_value, 0755, sysfs_show,
                                                   sysfs_store);
/*
 * mymodule_init : init API
 */
static int __init mymodule_init (void)
{
        int ret = 0;

	printk("\nModule initialized successfully");

	/* Creating the file /sys/kernel/igx_kobject.
	 * The file is created inside /sys/kernel since the parent
         * is kernel_kobj
	 */
        sysfs_kobject = kobject_create_and_add("igx_kobject",
                                                 kernel_kobj);
        if(!sysfs_kobject)
                return -ENOMEM;

	/* Creating the file /sys/kernel/igx_kobject/sysfs_value */
        ret = sysfs_create_file(sysfs_kobject, &sysfs_attribute.attr);
        if (ret) {
                pr_debug("failed to create the foo file in /sys/kernel/kobject_example \n");
        }

        return ret;
}

/*
 * mymodule_exit : deinit API
 */
static void __exit mymodule_exit (void)
{
        pr_debug ("Module un initialized successfully \n");
	/* Decrementing the reference counter */
        kobject_put(sysfs_kobject);
}

MODULE_LICENSE("GPL");

module_init(mymodule_init);
module_exit(mymodule_exit);
