#include <linux/module.h>

extern int symbol_variable;

module_param(symbol_variable,int,0);
MODULE_PARM_DESC(symbol_variable,"The input argument");


int init_module(void) {
        printk(KERN_INFO "module_C insert successfully\n");
	printk("symbol_variable:%d\n", symbol_variable);
        return 0;
}

void cleanup_module(void) {
        printk(KERN_INFO "module_C remove successfully\n");
}
