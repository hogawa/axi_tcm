#include <stdio.h>
#include <stdint.h>

#define u8     uint8_t
#define u32    uint32_t
#define FALSE  0
#define TRUE   1

//=====================================================================
//                         tcm_control_reg[31:0]
//        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  Index | 31  :   7 |  6  :  2  |       1       |      0      |
//        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Signal | NOT USED  | read_addr |  tcm_write_en | tcm_read_en |
//        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//=====================================================================

u32 command_gen(u8 read_addr, u8 tcm_wr_en, u8 tcm_read_en) {
    u32 command = 0;
    // control fields
    command = command | tcm_read_en;
    command = command | (tcm_wr_en << 1);
    command = command | (read_addr << 2);
    return command;
}

// Commands
#define AXI_TCM_STREAM_WRITE_EN  command_gen(0x0, TRUE, FALSE)
#define AXI_TCM_READ_ADDR(addr)  command_gen(addr, FALSE, TRUE)

int main() {
    printf("Stream write = 0x%08x\n", AXI_TCM_STREAM_WRITE_EN);
    for (int i = 0; i < 32; i++) {
        printf("Read from addr 5'h%02x = 0x%08x\n", i, AXI_TCM_READ_ADDR(i));
    }
    return 0;
}