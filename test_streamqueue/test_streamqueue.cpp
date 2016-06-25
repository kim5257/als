/*
 * test_streamqueue.cpp
 *
 *  Created on: 2015. 12. 19.
 *      Author: arun
 */

#include <structure/streamqueue.h>
#include <stdio.h>
#include <string.h>

#define	TEST_ONE_QUEUE_SIZE		(16)
#define	NUM_OF_TEST_ONE_LOOP		(64)

#define	TEST_TWO_QUEUE_SIZE		(16)
#define	NUM_OF_TEST_TWO_LOOP		(16)

#define	TEST_THREE_QUEUE_SIZE	(12)
#define	TEST_THREE_SAMPLE_ONE	{0,1,2,3,4,5,6,7,8,9}
#define	TEST_THREE_SAMPLE_TWO	{'A', 'B', 'C', 'D', 'E', 'F'}

#define	TEST_FOUR_QUEUE_SIZE		(12)
#define	TEST_FOUR_SAMPLE_ONE		{0,1,2,3,4,5,6,7,8,9}

#define	TEST_FIVE_QUEUE_SIZE		(16)
#define	TEST_FIVE_SAMPLE_ONE		{0,1,2,3,4,5,6,7,8,9}
#define	TEST_FIVE_COMPARE_ONE	{0,1,2,3}
#define	TEST_FIVE_COMPARE_TWO	{4,5,6,7,8,9,0,1,2,3}

bool
testCase1 (void);
bool
testCase2 (void);
bool
testCase3 (void);
bool
testCase4 (void);
bool
testCase5 (void);

int
main (	int argc,
		char* argv[])
{
	if ( testCase1 () )
	{
		printf ("testCase1 성공\n");
	}
	else
	{
		printf ("testCase1 실패\n");
	}

	if ( testCase2 () )
	{
		printf ("testCase2 성공\n");
	}
	else
	{
		printf ("testCase2 실패\n");
	}

	if ( testCase3 () )
	{
		printf ("testCase3 성공\n");
	}
	else
	{
		printf ("testCase3 실패\n");
	}

	if ( testCase4 () )
	{
		printf ("testCase4 성공\n");
	}
	else
	{
		printf ("testCase4 실패\n");
	}

	if ( testCase5 () )
	{
		printf ("testCase5 성공\n");
	}
	else
	{
		printf ("testCase5 실패\n");
	}

	return 0;
}

bool
testCase1 (void)
{
	bool ret = false;

	do
	{
		bool fail = false;
		char val = 0;
		char pollVal = 0;
		datastruct::StreamQueue queue (TEST_ONE_QUEUE_SIZE);

		for(size_t cnt = 0;cnt < NUM_OF_TEST_ONE_LOOP;++cnt)
		{
			if ( !queue.push ((uint8_t*)&val, sizeof(val)) )
			{
				fail = true;
				break;
			}

			if ( !queue.poll ((uint8_t*)&pollVal, sizeof(pollVal)) )
			{
				fail = true;
				break;
			}

			if ( val != pollVal )
			{
				fail = true;
				break;
			}

			++val;
		}

		if ( !fail )
		{
			ret = true;
		}
	}
	while( 0 );

	return ret;
}

bool
testCase2 (void)
{
	bool ret = false;

	do
	{
		bool fail = false;
		char pollVal = 0;
		datastruct::StreamQueue queue (TEST_TWO_QUEUE_SIZE);

		for(size_t loop = 0;loop < NUM_OF_TEST_TWO_LOOP;++loop)
		{
			for(size_t cnt = 0;cnt < queue.getSize ();++cnt)
			{
				if ( !queue.push ((uint8_t*)&cnt, sizeof(char)) )
				{
					fail = true;
					break;
				}
			}

			if ( fail )
			{
				break;
			}

			for(size_t cnt = 0;cnt < queue.getSize ();++cnt)
			{
				if ( !queue.poll ((uint8_t*)&pollVal, sizeof(pollVal)) )
				{
					fail = true;
					break;
				}
				else
				{
					if ( pollVal != (char)cnt )
					{
						fail = true;
						break;
					}
				}
			}

			if ( fail )
			{
				break;
			}
		}

		if ( !fail )
		{
			ret = true;
		}
	}
	while( 0 );

	return ret;
}

bool
testCase3 (void)
{
	bool ret = false;
	uint8_t sample1[] = TEST_THREE_SAMPLE_ONE;
	uint8_t sample2[] = TEST_THREE_SAMPLE_TWO;

	do
	{
		uint8_t pollVal[sizeof(sample1) + sizeof(sample2)] =
		{ 0, };
		datastruct::StreamQueue queue (TEST_THREE_QUEUE_SIZE);

		if ( !queue.push (sample1, sizeof(sample1)) )
		{
			break;
		}

		if ( !queue.poll (pollVal, sizeof(sample1)) )
		{
			break;
		}

		if ( memcmp (pollVal, sample1, sizeof(sample1)) != 0 )
		{
			break;
		}

		if ( !queue.push (sample2, sizeof(sample2)) )
		{
			break;
		}
		if ( !queue.push (sample2, sizeof(sample2)) )
		{
			break;
		}

		if ( !queue.poll (pollVal, sizeof(sample2) * 2) )
		{
			break;
		}

		if ( (memcmp (pollVal, sample2, sizeof(sample2)) != 0)
				|| (memcmp (&pollVal[sizeof(sample2)], sample2, sizeof(sample2))
					!= 0) )
		{
			break;
		}

		ret = true;
	}
	while( 0 );

	return ret;
}

bool
testCase4 (void)
{
	bool ret = false;
	uint8_t sample1[] = TEST_FOUR_SAMPLE_ONE;

	do
	{
		uint8_t pollVal[sizeof(sample1) * 2] =
		{ 0, };
		datastruct::StreamQueue queue (TEST_FOUR_QUEUE_SIZE);

		if ( !queue.push (sample1, sizeof(sample1)) )
		{
			break;
		}

		if ( queue.push (sample1, sizeof(sample1)) )
		{
			break;
		}

		if ( !queue.poll (pollVal, sizeof(sample1)) )
		{
			break;
		}

		if ( memcmp (pollVal, sample1, sizeof(sample1)) != 0 )
		{
			break;
		}

		if ( queue.poll (pollVal, sizeof(sample1)) )
		{
			break;
		}

		ret = true;
	}
	while( 0 );

	return ret;
}

bool
testCase5 (void)
{
	bool ret = false;
	uint8_t sample1[] = TEST_FIVE_SAMPLE_ONE;
	uint8_t pollVal[sizeof(TEST_FIVE_QUEUE_SIZE)] = TEST_FIVE_COMPARE_ONE;
	uint8_t compVal1[] = TEST_FIVE_COMPARE_ONE;
	uint8_t compVal2[] = TEST_FIVE_COMPARE_TWO;

	do
	{
		datastruct::StreamQueue queue (TEST_FIVE_QUEUE_SIZE);
		int off = 0;

		queue.push (sample1, sizeof(sample1));

		off = queue.findByte (3);
		queue.poll (pollVal, off);
		if ( memcmp (compVal1, pollVal, sizeof(compVal1)) != 0 )
		{
			break;
		}

		off = queue.findByte (3);
		if( off != 0 )
		{
			break;
		}

		queue.push (sample1, sizeof(sample1));
		off = queue.findByte (3);
		queue.poll (pollVal, off);
		if ( memcmp (compVal2, pollVal, sizeof(compVal2)) != 0 )
		{
			break;
		}

		ret = true;
	}
	while( 0 );

	return ret;
}
