#include <stdio.h>
#include "xparameters.h"
#include "xil_types.h"
#include "xil_io.h"

int main() {

    printf("Testing AXIL registers...\n");
    u32 test_val = 0xdeadbeef;
    Xil_Out32(XPAR_TCM_AXI_TEST_0_S_AXIL_BASEADDR, test_val);

    u32 tcm_control_reg;
    tcm_control_reg = Xil_In32(XPAR_TCM_AXI_TEST_0_S_AXIL_BASEADDR);

    if (test_val == tcm_control_reg)
    	printf("[PASS]\n");
    else
    	printf("[FAIL]\n");

    return 0;
}
