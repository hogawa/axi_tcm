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
reg[4:0] tcm_addr;

// Write enable signal for BRAM block
// This signal has to be synchronized with the address register
// Make sure that the wr_en is only active when the data is valid
always @ (posedge S_AXIS_ACLK) begin
    if (~S_AXIS_ARESETN) begin
        wr_en <= 1'b0;
    end
    else if (S_AXIS_TVALID & USR_tcm_control[1]) begin
        wr_en <= 1'b1;
    end
    else if (~S_AXIS_TVALID) begin
        wr_en <= 1'b0;
    end
end

// Address generation logic for BRAM block
always @ (posedge S_AXIS_ACLK) begin
    if (~S_AXIS_ARESETN) begin
        tcm_addr <= 0;
    end
    else if (wr_en) begin
        tcm_addr <= tcm_addr + 1'b1;
    end
    else if (S_AXIS_TLAST) begin
        tcm_addr <= 0;
    end
end

// AXI-Stream data buffer register
reg[31:0] tdata_buffer;
always @ (posedge S_AXIS_ACLK) begin
    if (~S_AXIS_ARESETN) begin
        tdata_buffer <= 0;
    end
    else begin
        tdata_buffer <= S_AXIS_TDATA;
    end
end

// Inferred memory block
reg[31:0] bram_block[0:31];

// Transfer data from buffer register to BRAM block
reg[31:0] tcm_rd_out;
wire tcm_rd_addr;
assign tcm_rd_addr = USR_tcm_control[5:2];
always @ (posedge S_AXIS_ACLK) begin
    if (wr_en) begin
        bram_block[tcm_addr] <= tdata_buffer;
    end
    else begin
        tcm_rd_out <= bram_block[tcm_rd_addr];
    end
end

//assign tcm_rd = tcm_rd_out;
assign tcm_rd = tdata_buffer;

endmodule
