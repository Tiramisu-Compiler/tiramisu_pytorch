#ifndef __CONV_CONF_HEADER_
#define __CONV_CONF_HEADER_

#include <sys/time.h>

#define LARGE_DATA_SET	0
#define MEDIUM_DATA_SET	1
#define SMALL_DATA_SET	0

#if LARGE_DATA_SET
	#define BATCH_SIZE 100
#elif MEDIUM_DATA_SET
	#define BATCH_SIZE 32
#elif SMALL_DATA_SET
	#define BATCH_SIZE 8
#endif

// Size of one data dimension
// Data is NxNx16
#if LARGE_DATA_SET
	#define N 512
#elif MEDIUM_DATA_SET
	#define N 64
#elif SMALL_DATA_SET
	#define N 32
#endif

// Number of features in the input
#define FIn 64 
// Number of features in the output
#define FOut 16

// Size of convolution filter (KxK)
#define K 4

// negative slopes value
#define NEGATIVE_SLOPES -0

// If this is defined, print 10 array elements only
#define PRINT_ONLY_10 0

#define NB_TESTS 100



#endif
