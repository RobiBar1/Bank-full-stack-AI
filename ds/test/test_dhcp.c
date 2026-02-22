#include <stdio.h>
#include <string.h>

#include "dhcp.h"

#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RESET "\033[0m"
#define PASS (1)
#define FAIL (0)
#define UNUSED(x) ((void)(x))
#define RUN_TEST(suite, test_fn)                                               \
  do {                                                                         \
    test_result_t result = {0};                                                \
    (suite).total++;                                                           \
                                                                               \
    if (test_fn(&result) == PASS) {                                            \
      (suite).passed++;                                                        \
    } else {                                                                   \
      printf(COLOR_RED "[FAIL] %s: %s\n" COLOR_RESET, #test_fn,                \
             result.message);                                                  \
    }                                                                          \
  } while (0)

typedef struct {
  size_t total;
  size_t passed;
} test_suite_t;

typedef struct {
  char message[256];
} test_result_t;

const char *dhcp_status_str[] = {"DHCP_SUCCESS", "DHCP_ALLOC_FAIL",
                                 "DHCP_INVALID_ADDRESSES"};

static void PrintTestSummary(test_suite_t *suite_ptr) {
  if (suite_ptr->passed == suite_ptr->total) {
    printf(COLOR_GREEN "PASS: ");
  } else {
    printf(COLOR_RED "FAIL: ");
  }
  printf("%lu/%lu passed\n" COLOR_RESET, suite_ptr->passed, suite_ptr->total);
}

static int CheckDHCPStatus(dhcp_status_t res_status,
                           dhcp_status_t expected_status,
                           test_result_t *result) {
  if (res_status != expected_status) {
    sprintf(result->message, COLOR_RED "res is different! Got %s, expected %s",
            dhcp_status_str[res_status], dhcp_status_str[expected_status]);
    return FAIL;
  }

  return PASS;
}

static int CheckAddr(unsigned char *res_addr, unsigned char *expected_addr,
                     test_result_t *result) {
  if (0 != memcmp(res_addr, expected_addr, LENGTH_OF_IP)) {
    sprintf(result->message,
            COLOR_RED "Addresses is different! Got %d, expected %d",
            *(int *)res_addr, *(int *)expected_addr);
    return FAIL;
  }

  return PASS;
}

static int CheckDHCPFree(dhcp_t *dhcp, size_t expected_free,
                         test_result_t *result) {
  size_t free = DHCPCount(dhcp);

  if (free != expected_free) {
    sprintf(result->message,
            COLOR_RED "Number of free adresses in dhcp is wrong! Got %lu, "
                      "expected %lu" COLOR_RESET,
            free, expected_free);
    return FAIL;
  }

  return PASS;
}

static int TestDHCPCreation(test_result_t *result) {
  unsigned char subnet_id[LENGTH_OF_IP] = {0xa, 0x0, 0x0, 0x0};
  size_t subnet_id_len = 24;
  size_t host_id_len = LENGTH_OF_IP * 8 - subnet_id_len;
  dhcp_t *dhcp = DHCPCreate(subnet_id, subnet_id_len);
  size_t expected_size = (1 << (host_id_len)) - 3;
  int status = PASS;

  status = CheckDHCPFree(dhcp, expected_size, result);

  DHCPDestroy(dhcp);

  return status;
}

static void RunTestDHCPCreation() {
  test_suite_t suite = {0};

  printf(COLOR_BLUE "Run tests for dhcp creation\n" COLOR_RESET);

  RUN_TEST(suite, TestDHCPCreation);

  PrintTestSummary(&suite);
  printf(COLOR_BLUE "Finished tests\n");
}

static int TestDHCPAllocSingle(test_result_t *result) {
  unsigned char subnet_id[LENGTH_OF_IP] = {0xa, 0x0, 0x0, 0x0};
  size_t subnet_id_len = 24;
  size_t host_id_len = LENGTH_OF_IP * 8 - subnet_id_len;
  dhcp_t *dhcp = DHCPCreate(subnet_id, subnet_id_len);
  size_t expected_size = (1 << (host_id_len)) - 3;
  unsigned char addr_to_add[] = {0xa, 0x0, 0x0, 0x1};
  unsigned char res_addr[LENGTH_OF_IP] = {0x0};
  unsigned char expected_addr[] = {0xa, 0x0, 0x0, 0x1};
  dhcp_status_t res_status = DHCP_SUCCESS;
  dhcp_status_t expected_status = DHCP_SUCCESS;
  int status = PASS;

  res_status = DHCPAlloc(dhcp, addr_to_add, res_addr);
  --expected_size;

  status = CheckDHCPStatus(res_status, expected_status, result) &&
           CheckAddr(res_addr, expected_addr, result) &&
           CheckDHCPFree(dhcp, expected_size, result);

  DHCPDestroy(dhcp);

  return status;
}

static int TestDHCPAllocSingleFirstTaken(test_result_t *result) {
  unsigned char subnet_id[LENGTH_OF_IP] = {0xa, 0x0, 0x0, 0x0};
  size_t subnet_id_len = 24;
  size_t host_id_len = LENGTH_OF_IP * 8 - subnet_id_len;
  dhcp_t *dhcp = DHCPCreate(subnet_id, subnet_id_len);
  size_t expected_size = (1 << (host_id_len)) - 3;
  unsigned char addr_to_add[] = {0xa, 0x0, 0x0, 0x0};
  unsigned char res_addr[LENGTH_OF_IP] = {0x0};
  unsigned char expected_addr[] = {0xa, 0x0, 0x0, 0x1};
  dhcp_status_t res_status = DHCP_SUCCESS;
  dhcp_status_t expected_status = DHCP_SUCCESS;
  int status = PASS;

  res_status = DHCPAlloc(dhcp, addr_to_add, res_addr);
  --expected_size;

  status = CheckDHCPStatus(res_status, expected_status, result) &&
           CheckAddr(res_addr, expected_addr, result) &&
           CheckDHCPFree(dhcp, expected_size, result);

  DHCPDestroy(dhcp);

  return status;
}

static int TestDHCPAllocSingleLastTaken(test_result_t *result) {
  unsigned char subnet_id[LENGTH_OF_IP] = {0xa, 0x0, 0x0, 0x0};
  size_t subnet_id_len = 24;
  size_t host_id_len = LENGTH_OF_IP * 8 - subnet_id_len;
  dhcp_t *dhcp = DHCPCreate(subnet_id, subnet_id_len);
  size_t expected_size = (1 << (host_id_len)) - 3;
  unsigned char addr_to_add[] = {0xa, 0x0, 0x0, 0xff};
  unsigned char res_addr[LENGTH_OF_IP] = {0x0};
  unsigned char expected_addr[] = {0xa, 0x0, 0x0, 0x1};
  dhcp_status_t res_status = DHCP_SUCCESS;
  dhcp_status_t expected_status = DHCP_SUCCESS;
  int status = PASS;

  res_status = DHCPAlloc(dhcp, addr_to_add, res_addr);
  --expected_size;

  status = CheckDHCPStatus(res_status, expected_status, result) &&
           CheckAddr(res_addr, expected_addr, result) &&
           CheckDHCPFree(dhcp, expected_size, result);

  DHCPDestroy(dhcp);

  return status;
}

static int TestDHCPAllocFullDHCP(test_result_t *result) {
  unsigned char subnet_id[LENGTH_OF_IP] = {0xa, 0x0, 0x0, 0x0};
  size_t subnet_id_len = 30;
  size_t host_id_len = LENGTH_OF_IP * 8 - subnet_id_len;
  dhcp_t *dhcp = DHCPCreate(subnet_id, subnet_id_len);
  size_t expected_size = (1 << (host_id_len)) - 3;
  unsigned char addr_to_add[] = {0xa, 0x0, 0x0, 0x01};
  unsigned char res_addr[LENGTH_OF_IP] = {0x0};
  dhcp_status_t res_status = DHCP_SUCCESS;
  dhcp_status_t expected_status = DHCP_ALLOC_FAIL;
  int status = PASS;

  DHCPAlloc(dhcp, addr_to_add, res_addr);
  --expected_size;
  res_status = DHCPAlloc(dhcp, addr_to_add, res_addr);

  status = CheckDHCPStatus(res_status, expected_status, result) &&
           CheckDHCPFree(dhcp, expected_size, result);

  DHCPDestroy(dhcp);

  return status;
}

static int TestDHCPAllocNotWholeHostID(test_result_t *result) {
  unsigned char subnet_id[LENGTH_OF_IP] = {0xa, 0x0, 0x0, 0xfc};
  size_t subnet_id_len = 30;
  size_t host_id_len = LENGTH_OF_IP * 8 - subnet_id_len;
  dhcp_t *dhcp = DHCPCreate(subnet_id, subnet_id_len);
  size_t expected_size = (1 << (host_id_len)) - 3;
  unsigned char addr_to_add[] = {0xa, 0x0, 0x0, 0xfd};
  unsigned char res_addr[LENGTH_OF_IP] = {0x0};
  unsigned char expected_addr[] = {0xa, 0x0, 0x0, 0xfd};
  dhcp_status_t res_status = DHCP_SUCCESS;
  dhcp_status_t expected_status = DHCP_SUCCESS;
  int status = PASS;

  res_status = DHCPAlloc(dhcp, addr_to_add, res_addr);
  --expected_size;

  status = CheckDHCPStatus(res_status, expected_status, result) &&
           CheckAddr(res_addr, expected_addr, result) &&
           CheckDHCPFree(dhcp, expected_size, result);

  DHCPDestroy(dhcp);

  return status;
}

static void RunTestDHCPAlloc() {
  test_suite_t suite = {0};

  printf(COLOR_BLUE "Run tests for dhcp alloc\n" COLOR_RESET);

  RUN_TEST(suite, TestDHCPAllocSingle);
  RUN_TEST(suite, TestDHCPAllocSingleFirstTaken);
  RUN_TEST(suite, TestDHCPAllocSingleLastTaken);
  RUN_TEST(suite, TestDHCPAllocFullDHCP);
  RUN_TEST(suite, TestDHCPAllocNotWholeHostID);

  PrintTestSummary(&suite);
  printf(COLOR_BLUE "Finished tests\n");
}

static int TestDHCPFreeSingle(test_result_t *result) {
  unsigned char subnet_id[LENGTH_OF_IP] = {0xa, 0x0, 0x0, 0x0};
  size_t subnet_id_len = 24;
  size_t host_id_len = LENGTH_OF_IP * 8 - subnet_id_len;
  dhcp_t *dhcp = DHCPCreate(subnet_id, subnet_id_len);
  size_t expected_size = (1 << (host_id_len)) - 3;
  unsigned char addr_to_add[] = {0xa, 0x0, 0x0, 0x1};
  unsigned char res_addr[LENGTH_OF_IP] = {0x0};
  dhcp_status_t res_status = DHCP_SUCCESS;
  dhcp_status_t expected_status = DHCP_SUCCESS;
  int status = PASS;

  DHCPAlloc(dhcp, addr_to_add, res_addr);
  res_status = DHCPFree(dhcp, addr_to_add);

  status = CheckDHCPStatus(res_status, expected_status, result) &&
           CheckDHCPFree(dhcp, expected_size, result);

  DHCPDestroy(dhcp);

  return status;
}

static void RunTestDHCPFree() {
  test_suite_t suite = {0};

  printf(COLOR_BLUE "Run tests for dhcp free\n" COLOR_RESET);

  RUN_TEST(suite, TestDHCPFreeSingle);

  PrintTestSummary(&suite);
  printf(COLOR_BLUE "Finished tests\n");
}

static void RunTests() {
  RunTestDHCPCreation();
  RunTestDHCPAlloc();
  RunTestDHCPFree();
}

int main() {
  RunTests();

  return 0;
}
