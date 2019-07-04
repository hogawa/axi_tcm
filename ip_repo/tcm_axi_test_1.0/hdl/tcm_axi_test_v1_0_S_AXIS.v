`timescale 1 ns / 1 ps
/**
 * \file  tcm_axi_test_v1_0_S_AXIS.v
 * \brief TODO
 */
module tcm_axi_test_v1_0_S_AXIS # (
    parameter integer C_S_AXIS_TDATA_WIDTH = 32
)(
    // Users ports here
    input[31:0] USR_tcm_control,
    output[31:0] USR_tcm_rd,
    // AXI-Stream ports
    input wire S_AXIS_ACLK,
    input wire S_AXIS_ARESETN,
    output wire S_AXIS_TREADY,
    input wire [C_S_AXIS_TDATA_WIDTH-1 : 0] S_AXIS_TDATA,
    input wire S_AXIS_TLAST,
    input wire S_AXIS_TVALID
);

// AXI stream slave output
assign S_AXIS_TREADY = USR_tcm_control[1];

// TCM BRAM signals
reg wr_en;              // Memory write enable signal
reg[4:0] counter_addr;  // This counter is used to delay the address increment in one cycle
reg[4:0] tcm_addr;      // This is the actual address value used for memory writing process

// Write enable signal for BRAM block:
// - This signal has to be synchronized with the address register
// - Make sure that the wr_en is only active when the data is valid
always @ (posedge S_AXIS_ACLK) begin
    if (~S_AXIS_ARESETN | ~S_AXIS_TVALID) begin
        wr_en <= 1'b0;
        counter_addr <= 0;
        tcm_addr <= 0;
    end
    else if (S_AXIS_TVALID & USR_tcm_control[1]) begin
        wr_en <= 1'b1;
        counter_addr <= counter_addr + 1'b1;
        tcm_addr <= counter_addr;
    end
end

// AXI-Stream data buffer register: data arriving from AXIS TDATA bus
reg[31:0] tdata_buffer;
always @ (posedge S_AXIS_ACLK) begin
    if (~S_AXIS_ARESETN) begin
        tdata_buffer <= 0;
    end
    else if (S_AXIS_TVALID) begin
        tdata_buffer <= S_AXIS_TDATA;
    end
end

// Inferred memory block (BRAM)
reg[31:0] bram_block[0:31];

// BRAM write process: transfer data from buffer register to BRAM block
// BRAM read process: transfer data from the specified memory address to
// the tcm_rd_out output register
reg[31:0] tcm_rd_out;
always @ (posedge S_AXIS_ACLK) begin
    if (wr_en) begin
        bram_block[tcm_addr] <= tdata_buffer;
    end
    else if (USR_tcm_control[0]) begin
        tcm_rd_out <= bram_block[USR_tcm_control[6:2]];
    end
end

// Memory read port for AXIL's slv_reg1 slave register
assign USR_tcm_rd = tcm_rd_out;

endmodule