// Linux hardware access test for I/O.
// Example 3 of 3 = I/O access with /dev/port device

using namespace std;
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>

#define FILE_NAME     "/dev/port"  // device virtual file name
#define PORT_ADDRESS   0x60        // base address of Keyboard Controller Data

int fd = 0;               // file descriptor
int io_physical = 0;      // pointer for io in the PHYSICAL I/O space
void* io_virtual = 0;     // same region at application VIRTUAL space
int data = 0;             // this for masking result and visual

int main() 
{
    // open device file
    printf("\nStart.");
    fd = open ( FILE_NAME , O_RDWR|O_NDELAY );
    if ( fd <= 0 )
        {
        printf ( "\nFile open error: %s ( %s )\n" , 
                 FILE_NAME, strerror(errno) );
        exit(1);
        }
    
    // initializing physical address
    io_physical = PORT_ADDRESS;
    printf( "\nPhysical port I/O address = %04Xh", io_physical );
    
    // seek at virtual file means select I/O port address
    lseek ( fd, PORT_ADDRESS, SEEK_SET );
    read  ( fd, &data, 1 );
    data &= 0xFF;
    printf( "\nRead data = %02Xh" , data );

    // done
    printf("\nStop OK.\n");
    exit(0);
}
