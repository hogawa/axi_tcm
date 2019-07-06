#include "xil_types.h"

//=====================================================================
//                         tcm_control_reg[31:0]
//        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  Index | 31  :   7 |  6  :  2  |       1       |      0      |
//        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Signal | NOT USED  | read_addr |  tcm_write_en | tcm_read_en |
//        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//=====================================================================

u32 axi_tcm_command(u8 read_addr, u8 tcm_wr_en, u8 tcm_read_en) {
    u32 command = 0;
    // control fields
    command = command | tcm_read_en;
    command = command | (tcm_wr_en << 1);
    command = command | (read_addr << 2);
    return command;
}
