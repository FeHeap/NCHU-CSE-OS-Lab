#include <linux/module.h> /* Needed by all modules */

int init_module(void) {
	printk(KERN_INFO "Insert hello_module successfully.\n");	//KERN_INFO = information
	/*
	 * A non 0 return means init_module failed; module can't be loaded
	*/
	return 0;
}

void cleanup_module(void) {
	printk(KERN_INFO "Remove hello_module successfully.\n");
}

//Module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("FE");
MODULE_DESCRIPTION("OSLAB_MODULE: hello_module");
