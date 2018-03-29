#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define MAJOR_NUMBER 61
MODULE_AUTHOR("Xu Yudong");
/* forward declaration ????? */
int onebyte_open(struct inode *inode, struct file * filep);
int onebyte_release(struct inode *inode, struct file * filep);
ssize_t onebyte_read(struct file * filep, char * buf, size_t count, loff_t *f_pos);
ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos);
static void onebyte_exit(void);

/* definition of file_operation structure */
struct file_operations onebyte_fops = {
      read: onebyte_read,
      write: onebyte_write,
      open: onebyte_open,
      release: onebyte_release
};

char * onebyte_data = NULL;
bool byteAvail=false;
int onebyte_open(struct inode *inode, struct file *filep)
{
	return 0; //always successfull
}

int onebyte_release(struct inode *inode, struct file* filep)
{
	return 0; //always 
}

ssize_t onebyte_read(struct file*filep, char*buf, size_t count, loff_t * f_pos)
{
	int byte_read=0;
	if(byteAvail){
		put_user(*onebyte_data,	buf);
		byte_read=1;
		byteAvail=false;
	}
	//return 0 indicating EOF
	return byte_read;
	//I need to fill this function later
}

ssize_t onebyte_write(struct file*filep, const char*buf, size_t count, loff_t *f_pos)
{
	if(count>0){
		get_user(*onebyte_data, buf);
		byteAvail=true;
		if(count>1){
			return -28;
		}
		return 1;
	}
	return 0;
	//I need to fill this function later
}


//Below is the module related function
static int onebyte_init(void)
{
	int result;
	//register the device
	result = register_chrdev(MAJOR_NUMBER, "onebyte",&onebyte_fops);
	if(result<0){
		return result;
	}
	//allocate one byte of memory for storage
	//kmalloc is just like malloc, the second parameter is 
	//the typeof memory to be allocated
	//To release the memory allocated by kmalloc, use kfree.
	onebyte_data = kmalloc(sizeof(char), GFP_KERNEL);
	if(!onebyte_data){
		onebyte_exit();
		//if cannot allocate memory, free it and return
		return -ENOMEM;
	}
	*onebyte_data='X';
	byteAvail=true;
	printk(KERN_ALERT "This is a onebyte device module\n");
	return 0;
}

static void onebyte_exit(void){
	if(onebyte_data){
		kfree(onebyte_data);
		onebyte_data=NULL;
	}
	unregister_chrdev(MAJOR_NUMBER, "onebyte");
	printk(KERN_ALERT "Onebyte device module is unloaded\n");
}

MODULE_LICENSE("GPL");
module_init(onebyte_init);
module_exit(onebyte_exit);
