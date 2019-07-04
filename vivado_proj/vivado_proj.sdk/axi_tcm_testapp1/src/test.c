#include <stdio.h>
// DMA drivers
#include "xaxidma.h"
#include "xparameters.h"
#include "xdebug.h"
// MMIO drivers
#include "xil_types.h"
#include "xil_io.h"

#define DMA_DEV_ID      XPAR_AXIDMA_0_DEVICE_ID
#define MEM_BASE_ADDR   0x01000000
#define TX_BUFFER_BASE  (MEM_BASE_ADDR + 0x00100000)
#define MAX_PKT_LEN		0xC // This is in bytes

// DMA instance
XAxiDma AxiDma;
XAxiDma_Config *CfgPtr;
int Status;

/**
 * \brief Initialize the XAxiDma device.
 */
long int AxiDma_Init() {
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

	// Disable interrupts, we use polling mode
	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	return XST_SUCCESS;
}

int main() {
	u32 *TxBufferPtr;
	TxBufferPtr = (u32 *)TX_BUFFER_BASE;

	// Configure tcm_control_reg
	u32 test_val = 0x2;
	Xil_Out32(XPAR_TCM_AXI_TEST_0_S_AXIL_BASEADDR, test_val);

	// Initialize and configure DMA
	AxiDma_Init();

	// Created the TxBuffer
	TxBufferPtr[0] = 0xdeadbeef;
	TxBufferPtr[1] = 0xdeadbef0;
	TxBufferPtr[2] = 0xdeadbef1;

	// Flush the SrcBuffer before the DMA transfer, in case the Data Cache is enabled
	// Sets up the DMA-To-Device so that data is sent
	Xil_DCacheFlushRange((UINTPTR)TxBufferPtr, MAX_PKT_LEN);
	Status = XAxiDma_SimpleTransfer(&AxiDma,(UINTPTR) TxBufferPtr, MAX_PKT_LEN, XAXIDMA_DMA_TO_DEVICE);

	if (Status != XST_SUCCESS)
		return XST_FAILURE;

	while (XAxiDma_Busy(&AxiDma,XAXIDMA_DMA_TO_DEVICE)) {/* wait... */}
	printf("Data sent... DONE\n");
	return XST_SUCCESS;
}
