#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

long long t = 0;

int main()
{
	long ps = sysconf(_SC_PAGESIZE);
	
	size_t s = ps;
	char* p = 0;
	while (s > 1024)
	{
		p = (char*)malloc(s);
		if (p) 
		{
			mmap(0, s, PROT_READ | PROT_WRITE, MAP_SHARED, 0, 0);
			t += s;
			s *= 2;
		}
		else
		{
			s /= 2;
		}
	}
	
	printf("%lld\n", t); 
	
    return 0;
}
