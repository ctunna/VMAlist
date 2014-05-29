#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm_types.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/pid.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");

static int pid = 1;

module_param(pid, int, 0);

static int __init start(void){
  struct task_struct *ts;
  struct vm_area_struct *curr;

  ts = pid_task(find_vpid((pid_t)pid), PIDTYPE_PID);
  
  down_read(&(ts->mm->mmap_sem));

  printk(KERN_INFO "Loading VMAlist..\n");
  printk(KERN_INFO "pid: %d\n", pid);
  printk(KERN_INFO "name of process = %s\n", ts->comm);
  
  curr = ts->mm->mmap;

  while(curr){
    char flags[5]={'-','-','-','p','\0'};

    if(curr->vm_flags & 1) flags[0]='r';
    if(curr->vm_flags & 2) flags[1]='w';
    if(curr->vm_flags & 4) flags[2]='x';
      
    printk(KERN_INFO "0%lx-0%lx %s\n", curr->vm_start, curr->vm_end, flags);
    curr = curr->vm_next;
  }
  
  up_read(&(ts->mm->mmap_sem));

  return 0;
}

static void __exit end(void){
	printk(KERN_INFO "Goodbye world. \n");
}

module_init(start);
module_exit(end);

