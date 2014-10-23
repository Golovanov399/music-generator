#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "Instrument.h"
#include "Track.h"
#include "Test.h"

void testADSR()
{
	Instrument unknown(std::vector<double>(0), 0.5, 0.5, 0.5, 0.5);
	double answers[] = {0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	for (int i = 0; i <= 20; ++i)
		if (fabs(unknown.ADSR((double) i / 10.0, 1.0) - answers[i]) >= 0.000001)
			fprintf(stderr, "ADSR(%d) = %f (right answer is %f)\n", i, unknown.ADSR((double) i / 10.0, 1.0), answers[i]);
	return;
}

// int main()
// {
// 	testADSR();
// 	return 0;	
// }
