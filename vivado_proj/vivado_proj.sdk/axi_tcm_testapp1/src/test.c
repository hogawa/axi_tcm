#include <stdio.h>
#include "xaxidma.h"
#include "xil_types.h"
#include "axi_tcm_driver.h"

// Definitions for this demo
#define DEBUG          // for debug print
#define NUM_OF_TX  32  // number of transfers through AXI-Stream bus

/**
 * \brief Main function: demo
 * - Transmit data from DMA to TCM through AXI-Stream bus
 * - Read back from TCM through AXI-Lite control register
 */
int main() {
	// DMA device instantiation
	XAxiDma AxiDma;

	// Configure tcm_control_reg: enable write process
	AXI_TCM_CONTROL_REG(STREAM_WRITE_EN);
	// Initialize and configure DMA instance
	AxiDma_Init(AxiDma);

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
	AxiDma_Tx(AxiDma, TxBufferPtr, (4*NUM_OF_TX));

	// Configure tcm_control_reg: enable read process
	for (u8 i = 0; i < NUM_OF_TX; i++) {
		AXI_TCM_CONTROL_REG(READ_ADDR(i));
		printf("Read from TCM address 0x%02x: %08lx\n", i, AXI_TCM_DATA_READ);
	}

	return XST_SUCCESS;
}
