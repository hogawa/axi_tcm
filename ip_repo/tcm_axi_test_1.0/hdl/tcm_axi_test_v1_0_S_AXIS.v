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
    output[31:0] tcm_rd,
    output tcm_wr_en,
    output[4:0] tcm_addr_out,
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
reg wr_en;
reg[4:0] counter_addr;  // this counter is used to delay the address increment in one cycle
reg[4:0] tcm_addr;

// HSO: debug
assign tcm_wr_en = wr_en;
assign tcm_addr_out = tcm_addr;

// Write enable signal for BRAM block
// This signal has to be synchronized with the address register
// Make sure that the wr_en is only active when the data is valid
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

// AXI-Stream data buffer register
reg[31:0] tdata_buffer;
always @ (posedge S_AXIS_ACLK) begin
    if (~S_AXIS_ARESETN) begin
        tdata_buffer <= 0;
    end
    else if (S_AXIS_TVALID) begin
        tdata_buffer <= S_AXIS_TDATA;
    end
end

// Inferred memory block
reg[31:0] bram_block[0:31];

// Transfer data from buffer register to BRAM block
reg[31:0] tcm_rd_out;  // for debugging port (ILA)
always @ (posedge S_AXIS_ACLK) begin
    if (wr_en) begin
        bram_block[tcm_addr] <= tdata_buffer;
    end
    else if (USR_tcm_control[0]) begin
        tcm_rd_out <= bram_block[USR_tcm_control[6:2]];
    end
end

// ILA debugging port
assign tcm_rd = tcm_rd_out;

endmodule
