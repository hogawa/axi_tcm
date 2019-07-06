#include <stdio.h>
#include "axi_tcm_driver.h"

/**
 * \brief Initialize the XAxiDma device:
 * - DMA in simple mode (no scatter-gather)
 * - Disabled interrupts (polling mode)
 */
long int AxiDma_Init(XAxiDma AxiDma) {
	XAxiDma_Config *CfgPtr;
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
int AxiDma_Tx(XAxiDma AxiDma, u32 *TxBufferPtr, u32 max_pkt_len) {
	int Status;

	Xil_DCacheFlushRange((UINTPTR)TxBufferPtr, max_pkt_len);
	Status = XAxiDma_SimpleTransfer(&AxiDma,(UINTPTR) TxBufferPtr, max_pkt_len, XAXIDMA_DMA_TO_DEVICE);

	if (Status != XST_SUCCESS)
		return XST_FAILURE;

	while (XAxiDma_Busy(&AxiDma, XAXIDMA_DMA_TO_DEVICE)) {/* wait for TX */}

#ifdef DEBUG
	printf("Data sent... DONE\n");
#endif

	return Status;
}
