//////////////////////////////////////////////////////////////////
//                                                              //
//  RAM-based register Bank for Amber Core                      //
//                                                              //
//  This file is part of the Amber project                      //
//  http://www.opencores.org/project,amber                      //
//                                                              //
//  Description                                                 //
//  Contains 37 32-bit registers, 16 of which are visible       //
//  ina any one operating mode.                                 //
//  The block is designed using syncronous RAM primitive,       //
//  and fits well into an FPGA design                           //
//                                                              //
//  Author(s):                                                  //
//      - Dmitry Tarnyagin, dmitry.tarnyagin@lockless.no        //
//                                                              //
//////////////////////////////////////////////////////////////////
//                                                              //
// Copyright (C) 2010 Authors and OPENCORES.ORG                 //
//                                                              //
// This source file may be used and distributed without         //
// restriction provided that this copyright statement is not    //
// removed from the file and that any derivative work contains  //
// the original copyright notice and the associated disclaimer. //
//                                                              //
// This source file is free software; you can redistribute it   //
// and/or modify it under the terms of the GNU Lesser General   //
// Public License as published by the Free Software Foundation; //
// either version 2.1 of the License, or (at your option) any   //
// later version.                                               //
//                                                              //
// This source is distributed in the hope that it will be       //
// useful, but WITHOUT ANY WARRANTY; without even the implied   //
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      //
// PURPOSE.  See the GNU Lesser General Public License for more //
// details.                                                     //
//                                                              //
// You should have received a copy of the GNU Lesser General    //
// Public License along with this source; if not, download it   //
// from http://www.opencores.org/lgpl.shtml                     //
//                                                              //
//////////////////////////////////////////////////////////////////

module a23_ram_register_bank (

input                       i_clk,
input                       i_fetch_stall,

input       [1:0]           i_mode_exec,            // registered cpu mode from execution stage
input       [1:0]           i_mode_exec_nxt,        // 1 periods delayed from i_mode_idec
                                                    // Used for register reads
input       [1:0]           i_mode_rds_exec,        // Use raw version in this implementation,
                                                    // includes i_user_mode_regs_store
input                       i_user_mode_regs_load,
input       [3:0]           i_rm_sel,
input       [3:0]           i_rds_sel,
input       [3:0]           i_rn_sel,

input                       i_pc_wen,
input       [3:0]           i_reg_bank_wsel,

input       [23:0]          i_pc,                   // program counter [25:2]
input       [31:0]          i_reg,

input       [3:0]           i_status_bits_flags,
input                       i_status_bits_irq_mask,
input                       i_status_bits_firq_mask,

output      [31:0]          o_rm,
output      [31:0]          o_rs,
output      [31:0]          o_rd,
output      [31:0]          o_rn,
output      [31:0]          o_pc

);

//////////////////////////////////////////////////////////////////
//                                                              //
//  Parameters file for Amber 2 Core                            //
//                                                              //
//  This file is part of the Amber project                      //
//  http://www.opencores.org/project,amber                      //
//                                                              //
//  Description                                                 //
//  Holds general parameters that are used is several core      //
//  modules                                                     //
//                                                              //
//  Author(s):                                                  //
//      - Conor Santifort, csantifort.amber@gmail.com           //
//                                                              //
//////////////////////////////////////////////////////////////////
//                                                              //
// Copyright (C) 2010 Authors and OPENCORES.ORG                 //
//                                                              //
// This source file may be used and distributed without         //
// restriction provided that this copyright statement is not    //
// removed from the file and that any derivative work contains  //
// the original copyright notice and the associated disclaimer. //
//                                                              //
// This source file is free software; you can redistribute it   //
// and/or modify it under the terms of the GNU Lesser General   //
// Public License as published by the Free Software Foundation; //
// either version 2.1 of the License, or (at your option) any   //
// later version.                                               //
//                                                              //
// This source is distributed in the hope that it will be       //
// useful, but WITHOUT ANY WARRANTY; without even the implied   //
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      //
// PURPOSE.  See the GNU Lesser General Public License for more //
// details.                                                     //
//                                                              //
// You should have received a copy of the GNU Lesser General    //
// Public License along with this source; if not, download it   //
// from http://www.opencores.org/lgpl.shtml                     //
//                                                              //
//////////////////////////////////////////////////////////////////


// Instruction Types
localparam [3:0]    REGOP       = 4'h0, // Data processing
                    MULT        = 4'h1, // Multiply
                    SWAP        = 4'h2, // Single Data Swap
                    TRANS       = 4'h3, // Single data transfer
                    MTRANS      = 4'h4, // Multi-word data transfer
                    BRANCH      = 4'h5, // Branch
                    CODTRANS    = 4'h6, // Co-processor data transfer
                    COREGOP     = 4'h7, // Co-processor data operation
                    CORTRANS    = 4'h8, // Co-processor register transfer
                    SWI         = 4'h9; // software interrupt


// Opcodes
localparam [3:0] AND = 4'h0,        // Logical AND
                 EOR = 4'h1,        // Logical Exclusive OR
                 SUB = 4'h2,        // Subtract
                 RSB = 4'h3,        // Reverse Subtract
                 ADD = 4'h4,        // Add
                 ADC = 4'h5,        // Add with Carry
                 SBC = 4'h6,        // Subtract with Carry
                 RSC = 4'h7,        // Reverse Subtract with Carry
                 TST = 4'h8,        // Test  (using AND operator)
                 TEQ = 4'h9,        // Test Equivalence (using EOR operator)
                 CMP = 4'ha,       // Compare (using Subtract operator)
                 CMN = 4'hb,       // Compare Negated
                 ORR = 4'hc,       // Logical OR
                 MOV = 4'hd,       // Move
                 BIC = 4'he,       // Bit Clear (using AND & NOT operators)
                 MVN = 4'hf;       // Move NOT
                 
// Condition Encoding
localparam [3:0] EQ  = 4'h0,        // Equal            / Z set
                 NE  = 4'h1,        // Not equal        / Z clear
                 CS  = 4'h2,        // Carry set        / C set
                 CC  = 4'h3,        // Carry clear      / C clear
                 MI  = 4'h4,        // Minus            / N set
                 PL  = 4'h5,        // Plus             / N clear
                 VS  = 4'h6,        // Overflow         / V set
                 VC  = 4'h7,        // No overflow      / V clear
                 HI  = 4'h8,        // Unsigned higher  / C set and Z clear
                 LS  = 4'h9,        // Unsigned lower
                                    // or same          / C clear or Z set
                 GE  = 4'ha,        // Signed greater 
                                    // than or equal    / N == V
                 LT  = 4'hb,        // Signed less than / N != V
                 GT  = 4'hc,        // Signed greater
                                    // than             / Z == 0, N == V
                 LE  = 4'hd,        // Signed less than
                                    // or equal         / Z == 1, N != V
                 AL  = 4'he,        // Always
                 NV  = 4'hf;        // Never

// Any instruction with a condition field of 0b1111 is UNPREDICTABLE.                
                
// Shift Types
localparam [1:0] LSL = 2'h0,
                 LSR = 2'h1,
                 ASR = 2'h2,
                 RRX = 2'h3,
                 ROR = 2'h3; 
 
// Modes
localparam [1:0] SVC  =  2'b11,  // Supervisor
                 IRQ  =  2'b10,  // Interrupt
                 FIRQ =  2'b01,  // Fast Interrupt
                 USR  =  2'b00;  // User

// One-Hot Mode encodings
localparam [5:0] OH_USR  = 0,
                 OH_IRQ  = 1,
                 OH_FIRQ = 2,
                 OH_SVC  = 3;


//////////////////////////////////////////////////////////////////
//                                                              //
//  Functions for Amber 2 Core                                  //
//                                                              //
//  This file is part of the Amber project                      //
//  http://www.opencores.org/project,amber                      //
//                                                              //
//  Description                                                 //
//  Functions used in more than one module                      //
//                                                              //
//  Author(s):                                                  //
//      - Conor Santifort, csantifort.amber@gmail.com           //
//                                                              //
//////////////////////////////////////////////////////////////////
//                                                              //
// Copyright (C) 2010 Authors and OPENCORES.ORG                 //
//                                                              //
// This source file may be used and distributed without         //
// restriction provided that this copyright statement is not    //
// removed from the file and that any derivative work contains  //
// the original copyright notice and the associated disclaimer. //
//                                                              //
// This source file is free software; you can redistribute it   //
// and/or modify it under the terms of the GNU Lesser General   //
// Public License as published by the Free Software Foundation; //
// either version 2.1 of the License, or (at your option) any   //
// later version.                                               //
//                                                              //
// This source is distributed in the hope that it will be       //
// useful, but WITHOUT ANY WARRANTY; without even the implied   //
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      //
// PURPOSE.  See the GNU Lesser General Public License for more //
// details.                                                     //
//                                                              //
// You should have received a copy of the GNU Lesser General    //
// Public License along with this source; if not, download it   //
// from http://www.opencores.org/lgpl.shtml                     //
//                                                              //
//////////////////////////////////////////////////////////////////


// ========================================================
// PC Filter - Remove the status bits 
// ========================================================
function [31:0] pcf;
input [31:0] pc_reg;
    begin
    pcf = {6'd0, pc_reg[25:2], 2'd0};
    end
endfunction


// ========================================================
// 4-bit to 16-bit 1-hot decode
// ========================================================
function [14:0] decode;
input [3:0] reg_sel;
begin
case ( reg_sel )
    4'h0:    decode = 15'h0001;
    4'h1:    decode = 15'h0002;
    4'h2:    decode = 15'h0004;
    4'h3:    decode = 15'h0008;
    4'h4:    decode = 15'h0010;
    4'h5:    decode = 15'h0020;
    4'h6:    decode = 15'h0040;
    4'h7:    decode = 15'h0080;
    4'h8:    decode = 15'h0100;
    4'h9:    decode = 15'h0200;
    4'ha:    decode = 15'h0400;
    4'hb:    decode = 15'h0800;
    4'hc:    decode = 15'h1000;
    4'hd:    decode = 15'h2000;
    4'he:    decode = 15'h4000;
    default: decode = 15'h0000;
endcase
end
endfunction


// ========================================================
// Convert Stats Bits Mode to one-hot encoded version
// ========================================================
function [3:0] oh_status_bits_mode;
input [1:0] fn_status_bits_mode;
begin
oh_status_bits_mode = 
    fn_status_bits_mode == SVC  ? 1'd1 << OH_SVC  :
    fn_status_bits_mode == IRQ  ? 1'd1 << OH_IRQ  :
    fn_status_bits_mode == FIRQ ? 1'd1 << OH_FIRQ :
                                  1'd1 << OH_USR  ;
end
endfunction

// ========================================================
// Convert mode into ascii name
// ========================================================
function [(14*8)-1:0]  mode_name;
input [4:0] mode;
begin

mode_name    = mode == USR  ? "User          " :
               mode == SVC  ? "Supervisor    " :
               mode == IRQ  ? "Interrupt     " :
               mode == FIRQ ? "Fast Interrupt" :
                              "UNKNOWN       " ;
end
endfunction


// ========================================================
// Conditional Execution Function
// ========================================================
// EQ Z set
// NE Z clear
// CS C set
// CC C clear
// MI N set
// PL N clear
// VS V set
// VC V clear
// HI C set and Z clear
// LS C clear or Z set
// GE N == V
// LT N != V
// GT Z == 0,N == V
// LE Z == 1 or N != V
// AL Always (unconditional)
// NV Never

function conditional_execute;
input [3:0] condition;
input [3:0] flags;
begin
conditional_execute  
               = ( condition == AL                                        ) ||
                 ( condition == EQ  &&  flags[2]                          ) ||
                 ( condition == NE  && !flags[2]                          ) ||
                 ( condition == CS  &&  flags[1]                          ) ||
                 ( condition == CC  && !flags[1]                          ) ||
                 ( condition == MI  &&  flags[3]                          ) ||
                 ( condition == PL  && !flags[3]                          ) ||
                 ( condition == VS  &&  flags[0]                          ) ||
                 ( condition == VC  && !flags[0]                          ) ||
            
                 ( condition == HI  &&    flags[1] && !flags[2]           ) ||
                 ( condition == LS  &&  (!flags[1] ||  flags[2])          ) ||
            
                 ( condition == GE  &&  flags[3] == flags[0]              ) ||
                 ( condition == LT  &&  flags[3] != flags[0]              ) ||

                 ( condition == GT  &&  !flags[2] && flags[3] == flags[0] ) ||
                 ( condition == LE  &&  (flags[2] || flags[3] != flags[0])) ;
            
end
endfunction


// ========================================================
// Log 2
// ========================================================

function [31:0] log2;
input    [31:0] num;
integer i;
integer out;
begin
  out = 32'd0;
  for (i=0; i<30; i=i+1)
    if ((2**i > num) && (out == 0))
      out = i-1;
  log2 = out;
end
endfunction

wire  [1:0]  mode_idec;
wire  [1:0]  mode_exec;
wire  [1:0]  mode_rds;

wire  [4:0]  rm_addr;
wire  [4:0]  rds_addr;
wire  [4:0]  rn_addr;
wire  [4:0]  wr_addr;

// Register pool in embedded ram memory
reg   [31:0] reg_ram_n[31:0];
reg   [31:0] reg_ram_m[31:0];
reg   [31:0] reg_ram_ds[31:0];

wire  [31:0] rds_out;
wire  [31:0] rm_out;
wire  [31:0] rn_out;

// Synchronous ram input buffering
reg   [4:0]  rm_addr_reg;
reg   [4:0]  rds_addr_reg;
reg   [4:0]  rn_addr_reg;

// User Mode Registers
reg   [23:0] r15 = 24'hc0_ffee;

wire  [31:0] r15_out_rm;
wire  [31:0] r15_out_rm_nxt;
wire  [31:0] r15_out_rn;

// r15 selectors
reg          rn_15 = 1'b0;   
reg          rm_15 = 1'b0;
reg          rds_15 = 1'b0;

// Write Enables from execute stage
assign mode_idec = i_mode_exec_nxt & ~{2{i_user_mode_regs_load}};
assign wr_addr = reg_addr(mode_idec, i_reg_bank_wsel);

// Read Enables from stage 1 (fetch)
assign mode_exec = i_mode_exec_nxt;
assign rm_addr = reg_addr(mode_exec, i_rm_sel);
assign rn_addr = reg_addr(mode_exec, i_rn_sel);

// Rds
assign mode_rds = i_mode_rds_exec;
assign rds_addr = reg_addr(mode_rds, i_rds_sel);

    
// ========================================================
// r15 Register Read based on Mode
// ========================================================
assign r15_out_rm     = { i_status_bits_flags, 
                          i_status_bits_irq_mask, 
                          i_status_bits_firq_mask, 
                          r15, 
                          i_mode_exec};

assign r15_out_rm_nxt = { i_status_bits_flags, 
                          i_status_bits_irq_mask, 
                          i_status_bits_firq_mask, 
                          i_pc, 
                          i_mode_exec};
                      
assign r15_out_rn     = {6'd0, r15, 2'd0};


// ========================================================
// Program Counter out
// ========================================================
assign o_pc = r15_out_rn;

// ========================================================
// Rm Selector
// ========================================================
assign rm_out = reg_ram_m[rm_addr_reg];

assign o_rm =	rm_15 ?				r15_out_rm :
						rm_out;

// ========================================================
// Rds Selector
// ========================================================
assign rds_out = reg_ram_ds[rds_addr_reg];

assign o_rs =	rds_15  ?			r15_out_rn :
						rds_out;

// ========================================================
// Rd Selector
// ========================================================
assign o_rd =	rds_15  ? 			r15_out_rm_nxt :
						rds_out;

// ========================================================
// Rn Selector
// ========================================================
assign rn_out = reg_ram_n[rn_addr_reg];

assign o_rn =	rn_15  ?			r15_out_rn :
						rn_out;
// ========================================================
// Register Update
// ========================================================
always @ ( posedge i_clk )
    if (!i_fetch_stall)
        begin

        // Register write.
        // Actually the code is synthesed as a syncronous ram
        // with an additional  pass-through multiplexor for
        // read-when-write handling.
        reg_ram_n[wr_addr]      <= i_reg;
        reg_ram_m[wr_addr]      <= i_reg;
        reg_ram_ds[wr_addr]     <= i_reg;
        r15                     <= i_pc_wen ? i_pc : r15;

        // The latching is actually implemented in a hard block.
        rn_addr_reg             <= rn_addr;
        rm_addr_reg             <= rm_addr;
        rds_addr_reg            <= rds_addr;

        rn_15                   <= i_rn_sel == 4'hF;
        rm_15                   <= i_rm_sel == 4'hF;
        rds_15                  <= i_rds_sel == 4'hF;
        end
    
// ========================================================
// Register mapping:
// ========================================================
// 0xxxx : r0 - r14
// 10xxx : r8_firq - r14_firq
// 110xx : r13_irq - r14_irq
// 111xx : r13_svc - r14_svc

function [4:0] reg_addr;
input [1:0] mode;
input [3:0] sel;
begin
	casez ({mode, sel}) // synthesis full_case parallel_case
		6'b??0???:	reg_addr = {1'b0, sel};		// r0 - r7
		6'b1?1100:	reg_addr = {1'b0, sel};		// irq and svc r12
		6'b001???:	reg_addr = {1'b0, sel};		// user r8 - r14
		6'b011???:	reg_addr = {2'b10, sel[2:0]};	// fiq r8-r14
		6'b1?10??:	reg_addr = {1'b0, sel};		// irq and svc r8-r11
		6'b101101:	reg_addr = {3'b110, sel[1:0]};	// irq r13
		6'b101110:	reg_addr = {3'b110, sel[1:0]};	// irq r14
		6'b101111:	reg_addr = {3'b110, sel[1:0]};	// irq r15, just to make the case full
		6'b111101:	reg_addr = {3'b111, sel[1:0]};	// svc r13
		6'b111110:	reg_addr = {3'b111, sel[1:0]};	// svc r14
		6'b111111:	reg_addr = {3'b111, sel[1:0]};	// svc r15, just to make the case full
	endcase
end
endfunction

// `ifdef A23_SIMULATION
// synthesis translate_off
// To be used as probes...
wire [31:0] r0;
wire [31:0] r1;
wire [31:0] r2;
wire [31:0] r3;
wire [31:0] r4;
wire [31:0] r5;
wire [31:0] r6;
wire [31:0] r7;
wire [31:0] r8;
wire [31:0] r9;
wire [31:0] r10;
wire [31:0] r11;
wire [31:0] r12;
wire [31:0] r13;
wire [31:0] r14;
wire [31:0] r13_svc;
wire [31:0] r14_svc;
wire [31:0] r13_irq;
wire [31:0] r14_irq;
wire [31:0] r8_firq;
wire [31:0] r9_firq;
wire [31:0] r10_firq;
wire [31:0] r11_firq;
wire [31:0] r12_firq;
wire [31:0] r13_firq;
wire [31:0] r14_firq;
wire [31:0] r0_out;
wire [31:0] r1_out;
wire [31:0] r2_out;
wire [31:0] r3_out;
wire [31:0] r4_out;
wire [31:0] r5_out;
wire [31:0] r6_out;
wire [31:0] r7_out;
wire [31:0] r8_out;
wire [31:0] r9_out;
wire [31:0] r10_out;
wire [31:0] r11_out;
wire [31:0] r12_out;
wire [31:0] r13_out;
wire [31:0] r14_out;

assign r0  = reg_ram_m[ 0];
assign r1  = reg_ram_m[ 1];
assign r2  = reg_ram_m[ 2];
assign r3  = reg_ram_m[ 3];
assign r4  = reg_ram_m[ 4];
assign r5  = reg_ram_m[ 5];
assign r6  = reg_ram_m[ 6];
assign r7  = reg_ram_m[ 7];
assign r8  = reg_ram_m[ 8];
assign r9  = reg_ram_m[ 9];
assign r10 = reg_ram_m[10];
assign r11 = reg_ram_m[11];
assign r12 = reg_ram_m[12];
assign r13 = reg_ram_m[13];
assign r14 = reg_ram_m[14];
assign r13_svc  = reg_ram_m[29];
assign r14_svc  = reg_ram_m[30];
assign r13_irq  = reg_ram_m[25];
assign r14_irq  = reg_ram_m[26];
assign r8_firq  = reg_ram_m[16];
assign r9_firq  = reg_ram_m[17];
assign r10_firq = reg_ram_m[18];
assign r11_firq = reg_ram_m[19];
assign r12_firq = reg_ram_m[20];
assign r13_firq = reg_ram_m[21];
assign r14_firq = reg_ram_m[22];
assign r0_out  = reg_ram_m[reg_addr(mode_exec,  0)];
assign r1_out  = reg_ram_m[reg_addr(mode_exec,  1)];
assign r2_out  = reg_ram_m[reg_addr(mode_exec,  2)];
assign r3_out  = reg_ram_m[reg_addr(mode_exec,  3)];
assign r4_out  = reg_ram_m[reg_addr(mode_exec,  4)];
assign r5_out  = reg_ram_m[reg_addr(mode_exec,  5)];
assign r6_out  = reg_ram_m[reg_addr(mode_exec,  6)];
assign r7_out  = reg_ram_m[reg_addr(mode_exec,  7)];
assign r8_out  = reg_ram_m[reg_addr(mode_exec,  8)];
assign r9_out  = reg_ram_m[reg_addr(mode_exec,  9)];
assign r10_out = reg_ram_m[reg_addr(mode_exec, 10)];
assign r11_out = reg_ram_m[reg_addr(mode_exec, 11)];
assign r12_out = reg_ram_m[reg_addr(mode_exec, 12)];
assign r13_out = reg_ram_m[reg_addr(mode_exec, 13)];
assign r14_out = reg_ram_m[reg_addr(mode_exec, 14)];
// synthesis translate_on
// `endif

endmodule

