#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RESET   "\033[0m"

static void TEST_ASSERT(condition, message)
{
	do 
	{ 
		if (condition)
		{
			++tests_passed; 
			printf(GREEN "  [PASS] " RESET "%s\n", message); 
		}
		else
		{
			++tests_failed;
			printf(RED "  [FAIL] " RESET "%s\n", message);
		} 
		
	} while(0)

void test_memset_basic(void)
{
    printf("\n=== MEMSET Basic Tests ===\n");
    
    /* Test 1: Zero size */
    {
        char buf[16] = "test";
        void *ret = my_memset(buf, 'x', 0);
        TEST_ASSERT(ret == buf && 0 == strcmp(buf, "test"), 
                    "Zero size - buffer unchanged");
    }
    
    /* Test 2: Single byte */
    {
        char buf[16] = {0};
        my_memset(buf, 'A', 1);
        TEST_ASSERT('A' == buf[0] && 0 == buf[1], 
                    "Single byte set");
    }
    
    /* Test 3: Multiple bytes with value 0 */
    {
        char buf[16];
        int i = 0;
        int correct = 1;
        
        memset(buf, 'X', 16);
        my_memset(buf, 0, 10);
        
        for (; i < 10; ++i)
        {
            if (0 != buf[i])
            {
            	correct = 0;
        	}
        }
        	
        TEST_ASSERT(correct && 'X' == buf[10], 
                    "Set multiple bytes to 0");
    }
    
    /* Test 4: Multiple bytes with value 0xFF */
    {
        unsigned char buf[16] = {0};
        int correct = 1;
        int i = 0;
        
        my_memset(buf, 0xFF, 10);
        for (; i < 10; ++i)
        {
            if (0xFF != buf[i])
            {
            	correct = 0;
 			{       
        }
        
        TEST_ASSERT(correct && 0 == buf[10], 
                    "Set multiple bytes to 0xFF");
    }
    
    /* Test 5: Return value */
    {
        char buf[16];
        void *ret = my_memset(buf, 'Z', 8);
        TEST_ASSERT(ret == buf, 
                    "Returns destination pointer");
    }
}
