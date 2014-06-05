/* second allocation is too big to fit */
#include <assert.h>
#include <stdlib.h>
#include "mem.h"
#include <stdio.h>







 
 int main() {
    assert(Mem_Init(4096) == 0);
    void* ptr[4];
	Mem_Dump(); 
    ptr[0] = Mem_Alloc(8, FIRSTFIT);
    printf("8 (+8) should have been allocated\n\n");
	Mem_Dump(); 
    ptr[1] = Mem_Alloc(16, FIRSTFIT);
    printf("16 (+8) should have been allocated\n\n");
    

	Mem_Dump(); 
    Mem_Free(ptr[0]);
   printf("8(+8) should have been freed\n\n");
	Mem_Dump(); 
    //assert(Mem_Free(ptr[0]) == 0);
    Mem_Free(ptr[1]);
    printf("16 (+8) should have been freed\n");
    printf("all memory should be free\n\n");
	Mem_Dump(); 
   // assert(Mem_Free(ptr[1]) == 0);
 
    ptr[2] = Mem_Alloc(32, FIRSTFIT);
    printf("32 (+8) should have been allocated\n\n");
	Mem_Dump(); 
    ptr[3] = Mem_Alloc(8, FIRSTFIT);
    printf("8(+8) should have been allocated\n\n");
	Mem_Dump(); 
    Mem_Free(ptr[2]);
    printf("32 (+8)should have been freed\n\n");
	Mem_Dump(); 
    //assert(Mem_Free(ptr[2]) == 0);
    Mem_Free(ptr[3]);
    printf("8  (+8) should have been freed\n\n");
	Mem_Dump(); 
   // assert(Mem_Free(ptr[3]) == 0);
 





	exit(0);
}
