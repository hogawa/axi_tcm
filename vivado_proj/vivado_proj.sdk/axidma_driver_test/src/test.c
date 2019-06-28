/*****************************************************************************/
/**
 * @file streamExampleSimpleDMA.c
 *
 * In this file the basic set up of streaming a package buffer over the AXI
 * Stream Interface to a Custom IP core in PL. This is example is highly
 * inspired by examples from Xilinx.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who  Date     Changes
 * ----- ---- -------- -------------------------------------------------------
 * 1.00a jcla 03/04/16 The example is created based on other examples from
 * 					   Xilinx

 * ***************************************************************************

 */
/***************************** Include Files *********************************/
#include "xaxidma.h"
#include "xparameters.h"
#include "xdebug.h"


/******************** Constant Definitions **********************************/

/*
 * Device hardware build related constants.
 */

#define DMA_DEV_ID		XPAR_AXIDMA_0_DEVICE_ID

#define MEM_BASE_ADDR		0x01000000

#define TX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00100000)

// The example can be extended to feature a larger stream package, but for
// simplicity it is now set to 0x1
#define MAX_PKT_LEN		0x1 //0x20

#define TEST_START_VALUE	0x9

#define NUMBER_OF_TRANSFERS	10


/************************** Function Prototypes ******************************/

#if (!defined(DEBUG))
extern void xil_printf(const char *format, ...);
#endif

/************************** Variable Definitions *****************************/
/*
 * Device instance definitions
 */
XAxiDma AxiDma;


/*****************************************************************************/
/*
* The entry point for this example. It invokes the example function,
* and reports the execution status.
*
* @param	None.
*
* @return
*		- XST_SUCCESS if example finishes successfully
*		- XST_FAILURE if example fails.
*
* @note		None.
*
******************************************************************************/
int main()
{
	XAxiDma_Config *CfgPtr;
	int Status;
	int Index;
	u8 *TxBufferPtr;
	u8 Value;

	xil_printf("\r\nStarting sending data\r\n");

	TxBufferPtr = (u8 *)TX_BUFFER_BASE;

	/* Initialize the XAxiDma device.
	 */
	CfgPtr = XAxiDma_LookupConfig(DMA_DEV_ID);
	if (!CfgPtr) {
		xil_printf("No config found for %d\r\n", DMA_DEV_ID);
		return XST_FAILURE;
	}

	Status = XAxiDma_CfgInitialize(&AxiDma, CfgPtr);
	if (Status != XST_SUCCESS) {
		xil_printf("Initialization failed %d\r\n", Status);
		return XST_FAILURE;
	}

	if(XAxiDma_HasSg(&AxiDma)){
		xil_printf("Device configured as SG mode \r\n");
		return XST_FAILURE;
	}

	/* Disable interrupts, we use polling mode
	 */
	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK,
						XAXIDMA_DEVICE_TO_DMA);

	Value = TEST_START_VALUE;

	// Created the TxBuffer. For now it is set to one however it
	// can be extended to a larger number
	for(Index = 0; Index < MAX_PKT_LEN; Index ++) {
			TxBufferPtr[Index] = Value;

			Value = (Value + 1) & 0xFF;
	}

	/* Flush the SrcBuffer before the DMA transfer, in case the Data Cache
	 * is enabled
	 */
	Xil_DCacheFlushRange((UINTPTR)TxBufferPtr, MAX_PKT_LEN);


	//Sets up the DMA-To-Device so that data i send.
	Status = XAxiDma_SimpleTransfer(&AxiDma,(UINTPTR) TxBufferPtr,
				MAX_PKT_LEN, XAXIDMA_DMA_TO_DEVICE);

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	while (XAxiDma_Busy(&AxiDma,XAXIDMA_DMA_TO_DEVICE)) {} //wait

	xil_printf("\r\nDone sending data\r\n");
	/* Test finishes successfully
	 */
	return XST_SUCCESS;
}
