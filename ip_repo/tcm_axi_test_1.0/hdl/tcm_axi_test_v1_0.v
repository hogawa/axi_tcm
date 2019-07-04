`timescale 1 ns / 1 ps
/**
 * \file  tcm_axi_test_v1_0.v
 * \brief TODO
 */
module tcm_axi_test_v1_0 # (
	parameter integer C_S_AXIL_DATA_WIDTH = 32,
	parameter integer C_S_AXIL_ADDR_WIDTH = 4,
	parameter integer C_S_AXIS_TDATA_WIDTH = 32
)(
	// User ports here
    // NONE
	// AXI-Lite ports
	input wire s_axil_aclk,
	input wire s_axil_aresetn,
	input wire [C_S_AXIL_ADDR_WIDTH-1 : 0] s_axil_awaddr,
	input wire [2 : 0] s_axil_awprot,
	input wire s_axil_awvalid,
	output wire s_axil_awready,
	input wire [C_S_AXIL_DATA_WIDTH-1 : 0] s_axil_wdata,
	input wire [(C_S_AXIL_DATA_WIDTH/8)-1 : 0] s_axil_wstrb,
	input wire s_axil_wvalid,
	output wire s_axil_wready,
	output wire [1 : 0] s_axil_bresp,
	output wire s_axil_bvalid,
	input wire s_axil_bready,
	input wire [C_S_AXIL_ADDR_WIDTH-1 : 0] s_axil_araddr,
	input wire [2 : 0] s_axil_arprot,
	input wire s_axil_arvalid,
	output wire s_axil_arready,
	output wire [C_S_AXIL_DATA_WIDTH-1 : 0] s_axil_rdata,
	output wire [1 : 0] s_axil_rresp,
	output wire s_axil_rvalid,
	input wire s_axil_rready,
	// AXI-Stream ports
	input wire s_axis_aclk,
	input wire s_axis_aresetn,
	output wire s_axis_tready,
	input wire [C_S_AXIS_TDATA_WIDTH-1 : 0] s_axis_tdata,
	input wire [(C_S_AXIS_TDATA_WIDTH/8)-1 : 0] s_axis_tstrb,
	input wire s_axis_tlast,
	input wire s_axis_tvalid
);

// User logic here
wire[31:0] USR_tcm_control_w;
wire[31:0] USR_tcm_rd_w;
// User logic ends

// Instantiation of AXI Bus Interface S_AXIL
tcm_axi_test_v1_0_S_AXIL # (
	.C_S_AXI_DATA_WIDTH(C_S_AXIL_DATA_WIDTH),
	.C_S_AXI_ADDR_WIDTH(C_S_AXIL_ADDR_WIDTH)
) tcm_axi_test_v1_0_S_AXIL_inst (
	.USR_tcm_control(USR_tcm_control_w),
	.USR_tcm_rd(USR_tcm_rd_w),
	.S_AXI_ACLK(s_axil_aclk),
	.S_AXI_ARESETN(s_axil_aresetn),
	.S_AXI_AWADDR(s_axil_awaddr),
	.S_AXI_AWPROT(s_axil_awprot),
	.S_AXI_AWVALID(s_axil_awvalid),
	.S_AXI_AWREADY(s_axil_awready),
	.S_AXI_WDATA(s_axil_wdata),
	.S_AXI_WSTRB(s_axil_wstrb),
	.S_AXI_WVALID(s_axil_wvalid),
	.S_AXI_WREADY(s_axil_wready),
	.S_AXI_BRESP(s_axil_bresp),
	.S_AXI_BVALID(s_axil_bvalid),
	.S_AXI_BREADY(s_axil_bready),
	.S_AXI_ARADDR(s_axil_araddr),
	.S_AXI_ARPROT(s_axil_arprot),
	.S_AXI_ARVALID(s_axil_arvalid),
	.S_AXI_ARREADY(s_axil_arready),
	.S_AXI_RDATA(s_axil_rdata),
	.S_AXI_RRESP(s_axil_rresp),
	.S_AXI_RVALID(s_axil_rvalid),
	.S_AXI_RREADY(s_axil_rready)
);

// Instantiation of AXI Bus Interface S_AXIS
tcm_axi_test_v1_0_S_AXIS # (
	.C_S_AXIS_TDATA_WIDTH(C_S_AXIS_TDATA_WIDTH)
) tcm_axi_test_v1_0_S_AXIS_inst (
    .USR_tcm_control(USR_tcm_control_w),
    .USR_tcm_rd(USR_tcm_rd_w),
	.S_AXIS_ACLK(s_axis_aclk),
	.S_AXIS_ARESETN(s_axis_aresetn),
	.S_AXIS_TREADY(s_axis_tready),
	.S_AXIS_TDATA(s_axis_tdata),
	.S_AXIS_TLAST(s_axis_tlast),
	.S_AXIS_TVALID(s_axis_tvalid)
);

endmodule