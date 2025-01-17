//
// ramdisk that uses the disk image loaded by qemu -initrd fs.img
//

#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "buf.h"
#include "ram_disk.h"

void
ram_disk_init(void)
{
}

// If B_DIRTY is set, write buf to disk, clear B_DIRTY, set B_VALID.
// Else if B_VALID is not set, read buf from disk, set B_VALID.
void
ram_disk_rw(struct buf *b, int write)
{
  if(!holdingsleep(&b->lock))
    panic("include/ramdiskrw: buf not locked");

  if(b->blockno >= FSSIZE)
    panic("include/ramdiskrw: sectorno too big");

  uint64 diskaddr = b->blockno * BSIZE;
  char *addr = (char *)fs_img + diskaddr;

  if(write /* || (b->flags & B_DIRTY) */ ){
    // write
    memmove(addr, b->data, BSIZE);
  } else {
    // read
    memmove(b->data, addr, BSIZE);
  }
}

