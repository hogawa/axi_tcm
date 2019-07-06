#ifndef SRC_AXI_TCM_DRIVER_H_
#define SRC_AXI_TCM_DRIVER_H_

#include "xaxidma.h"
#include "xparameters.h"
#include "xdebug.h"
#include "xil_types.h"
#include "xil_io.h"
#include "command_gen.h"

// DMA definitions
#define DMA_DEV_ID      XPAR_AXIDMA_0_DEVICE_ID
#define MEM_BASE_ADDR   0x01000000
#define TX_BUFFER_BASE  (MEM_BASE_ADDR + 0x00100000)

// axi_tcm commands
#define AXI_TCM_CONTROL_REG(command)  Xil_Out32(XPAR_TCM_AXI_TEST_0_S_AXIL_BASEADDR, command)
#define STREAM_WRITE_EN               axi_tcm_command(0x0, TRUE, FALSE)
#define READ_ADDR(addr)               axi_tcm_command(addr, FALSE, TRUE)
#define AXI_TCM_DATA_READ             Xil_In32(XPAR_TCM_AXI_TEST_0_S_AXIL_BASEADDR + 4)

// DMA functions
long int AxiDma_Init(XAxiDma AxiDma);
int AxiDma_Tx(XAxiDma AxiDma, u32 *TxBufferPtr, u32 max_pkt_len);

#endif /* SRC_AXI_TCM_DRIVER_H_ */
