#include <stdio.h>
#include "xaxidma.h"
#include "xparameters.h"
#include "xdebug.h"
#include "xil_types.h"
#include "xil_io.h"
#include "command_gen.h"

// Definitions for this demo
#define DEBUG          // for debug print
#define NUM_OF_TX  32  // number of transfers through AXI-Stream bus

// DMA definitions
#define DMA_DEV_ID      XPAR_AXIDMA_0_DEVICE_ID
#define MEM_BASE_ADDR   0x01000000
#define TX_BUFFER_BASE  (MEM_BASE_ADDR + 0x00100000)

// DMA device instantiation
XAxiDma AxiDma;
XAxiDma_Config *CfgPtr;

// axi_tcm commands
#define AXI_TCM_CONTROL_REG(command)  Xil_Out32(XPAR_TCM_AXI_TEST_0_S_AXIL_BASEADDR, command)
#define STREAM_WRITE_EN               axi_tcm_command(0x0, TRUE, FALSE)
#define READ_ADDR(addr)               axi_tcm_command(addr, FALSE, TRUE)
#define AXI_TCM_DATA_READ             Xil_In32(XPAR_TCM_AXI_TEST_0_S_AXIL_BASEADDR + 4)

/**
 * \brief Initialize the XAxiDma device:
 * - DMA in simple mode (no scatter-gather)
 * - Disabled interrupts (polling mode)
 */
long int AxiDma_Init() {
	int Status;

	CfgPtr = XAxiDma_LookupConfig(DMA_DEV_ID);

	if (!CfgPtr) {
		printf("No config found for %d\n", DMA_DEV_ID);
		return XST_FAILURE;
	}

	Status = XAxiDma_CfgInitialize(&AxiDma, CfgPtr);
	if (Status != XST_SUCCESS) {
		printf("Initialization failed %d\n", Status);
		return XST_FAILURE;
	}

	if (XAxiDma_HasSg(&AxiDma)){
		printf("Device configured as SG mode\n");
		return XST_FAILURE;
	}

	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	return XST_SUCCESS;
}

/**
 * \brief DMA transfer data:
 * - Flush the SrcBuffer before the DMA transfer, in case the Data Cache is enabled
 * - Sets up the DMA-To-Device so that data is sent
 */
int AxiDma_Tx(u32 *TxBufferPtr, u32 max_pkt_len) {
	int Status;

	Xil_DCacheFlushRange((UINTPTR)TxBufferPtr, max_pkt_len);
	Status = XAxiDma_SimpleTransfer(&AxiDma,(UINTPTR) TxBufferPtr, max_pkt_len, XAXIDMA_DMA_TO_DEVICE);

	if (Status != XST_SUCCESS)
		return XST_FAILURE;

	while (XAxiDma_Busy(&AxiDma,XAXIDMA_DMA_TO_DEVICE)) {/* wait for TX */}

#ifdef DEBUG
	printf("Data sent... DONE\n");
#endif

	return Status;
}

/**
 * \brief Main function: demo
 * - Transmit data from DMA to TCM through AXI-Stream bus
 * - Read back from TCM through AXI-Lite control register
 */
int main() {
	// Configure tcm_control_reg: enable write process
	AXI_TCM_CONTROL_REG(STREAM_WRITE_EN);
	// Initialize and configure DMA instance
	AxiDma_Init();

	// DMA transmission buffer
	u32 *TxBufferPtr;
	TxBufferPtr = (u32 *)TX_BUFFER_BASE;

	// Send 32x32 bits of data to TCM: {0xa0000000,...,0xa000001f}
	u32 test_val = 0xa0000000;
	for (u8 i = 0x00; i < NUM_OF_TX; i++) {
		TxBufferPtr[i] = test_val;
		test_val++;
	}

	// Transmit data through DMA
	AxiDma_Tx(TxBufferPtr, (4*NUM_OF_TX));

	// Configure tcm_control_reg: enable read process
	for (u8 i = 0; i < NUM_OF_TX; i++) {
		AXI_TCM_CONTROL_REG(READ_ADDR(i));
		printf("Read from TCM address 0x%02x: %08lx\n", i, AXI_TCM_DATA_READ);
	}

	return XST_SUCCESS;
}
