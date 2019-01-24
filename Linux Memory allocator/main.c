#include "memalloc.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
				void *alloc_tests[100];

				for(int i = 0; i < 10; i++)
								alloc_tests[i] = memalloc(100);

				for(int i = 0; i < 10; i++)
								alloc_tests[i] = rememalloc(alloc_tests[i], 200);


}
