#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
/*
Few notes on user memory and kernel memory
They are both memory but each process thinks that they have their own memory (virtual address) 
All processes share the same kernel memory
_> Page table ensures protection

Protection is using flag like PTE_U: user can access
*/
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 startingVA;
  int num;
  uint64 buffer;

  argaddr(0, &startingVA);
  argint(1, &num);
  argaddr(2, &buffer);

  struct proc *p = myproc();

  int ans = 0;

  for (int i = 0; i < num; i++){
    uint64 va = startingVA + i*PGSIZE;

    // the walk function gets the va and use the page table to get the correct page table entry (PTE)
    // the PTE stores a set of bit including the physical page number with flags
    // to get the physical address, use PTE2PA(pte)
    pte_t *pte = walk(p->pagetable, va, 0);

    if (*pte == 0) 
      return -1;

    if (*pte & PTE_A){
      ans += (1 << i);
      *pte &= ~PTE_A;
    }
  }

  // copy from the kernel back to the user space
  if (copyout(p->pagetable, buffer, (char*)&ans, sizeof(ans)) < 0)
    return -1;
  
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
