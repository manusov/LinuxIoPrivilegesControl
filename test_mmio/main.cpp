// Linux hardware access test for Memory-mapped I/O.
// Example 2 of 3 = MMIO access with physical/virtual address space mapping,
// use /dev/mem device.

using namespace std;
#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <ctime>
#include <errno.h>
#include <cstring>

#define FILE_NAME     "/dev/mem"
#define PAGE_SIZE     4096
#define REGION_BASE   0xFFFFF000   // base address of memory-mapped IO (ROM)
// #define REGION_OFFSET 0xFF0     // offset of last 16 bytes, usually JMP FAR
// #define REGION_SIZE   16        // region length, bytes
#define REGION_OFFSET 0xE00
#define REGION_SIZE   512

int fd = 0;               // file descriptor
off_t mmio_physical = 0;  // pointer for memory mapped io in the PHYSICAL space
void* mmio_virtual = 0;   // same region at application VIRTUAL space

volatile unsigned char *targetPointer;  // useable pointer
unsigned char result = 0;               // read result
int data = 0;                           // this for masking result and visual

int main() 
{
    // open device file
    printf("\nStart.");
    fd = open ( FILE_NAME , O_RDWR|O_SYNC );
    if ( fd <= 0 )
        {
        printf ( "\nFile open error: %s ( %s )\n" , 
                 FILE_NAME, strerror(errno) );
        exit(1);
        }
    
    // initializing physical address
    mmio_physical = REGION_BASE;
    printf( "\nPhysical page base address  = %p", (void*)mmio_physical );
    
    // mapping physical address to application virtual address
    mmio_virtual = mmap( NULL, PAGE_SIZE, 
                         PROT_WRITE|PROT_READ|PROT_EXEC, MAP_SHARED, 
                         fd, mmio_physical );
    if ( mmio_virtual == MAP_FAILED )
        {
        printf ( "\nMapping error ( %s )\n" , strerror(errno) );
        exit(1);
        }
    printf( "\nVirtual page base address   = %p", mmio_virtual );
    
    targetPointer = (unsigned char *) ( mmio_virtual );
    targetPointer +=  + REGION_OFFSET;
    printf( "\nVirtual target address      = %p", targetPointer );
    
    // this for simple debug read one byte
    // result = *targetPointer;
    // data = result & 0xFF;
    // printf( "\nRead = %08X" , data );
    
    // read data and visual small dump
    unsigned long visualAddress = REGION_BASE + REGION_OFFSET;
    int i = 0;
    printf("\nRead data:");
    for ( i=0; i<REGION_SIZE; i++ )
        {                                    // readb, ioread8 is better
        if ( i%16 == 0 )
            {
            printf( "\n%08lx  " , visualAddress );
            visualAddress += 16;
            }
        result = *targetPointer++;           // direct read not recommended
        // result = readb(targetPointer++);  // but this n/a for user applic.
        data = result & 0xFF;
        printf ( "%02X " , data );
        }
    
    // unmap region
    if ( munmap ( mmio_virtual, PAGE_SIZE) < 0 )
        {
        printf ( "\nUnmap error ( %s )\n" , strerror(errno) );
        exit(1);
        }
    
    // done
    printf("\nStop OK.\n");
    exit(0);
}

