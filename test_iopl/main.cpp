// Linux hardware access test for I/O ports.
// Example 1 of 3 = I/O access with IOPL control.

#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <sys/io.h>
#include <errno.h>

using namespace std;

int main() 
{
    // Create variables
    int status = 0;             // API status
    int newLevel = 3;           // I/O privelege level
    unsigned char result = 0;   // Read 8-bit port result
    int port = 0x60;            // Port address
    int data = 0;               // Data for visualization
    // For enable IO, must set numerically: IOPL > CPL
    // IOPL = I/O Privelege Level, changed for enable IO
    // CPL  = Current Privelege Level, for user applications const=3
    status = iopl (newLevel);
    if ( status != 0 )
        {
        printf ( "\nSet iopl error ( %s )\n" , strerror(errno) );
        exit(1);
        }
    // Execute IO: read port
    result = inb ( port );
    // Masking and visual data
    data = result & 0xFF;
    printf( "Port = %08X , Read = %08X" , port , data );
    // Done
    printf("\nDone.\n");
    exit(0);
}

