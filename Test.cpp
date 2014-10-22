#include <cstdio>
#include <cstdlib>
#include <vector>
#include "Test.h"
#include "Track.h"

void testADSR(int length)
{
	for (int i = 0; i <= 2 * length; ++i)
		fprintf(stderr, "%f\n", ADSR(i, (double) i / (double) length, 1.0));
	return;
}

int main()
{
	testADSR(10);
	return 0;	
}
