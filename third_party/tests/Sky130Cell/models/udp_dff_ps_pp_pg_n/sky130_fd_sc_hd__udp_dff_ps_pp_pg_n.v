/**
 * Copyright 2020 The SkyWater PDK Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

`ifndef SKY130_FD_SC_HD__UDP_DFF_PS_PP_PG_N_V
`define SKY130_FD_SC_HD__UDP_DFF_PS_PP_PG_N_V

/**
 * udp_dff$PS_pp$PG$N: Positive edge triggered D flip-flop with active
 *                     high
 *
 * Verilog primitive definition.
 *
 * WARNING: This file is autogenerated, do not modify directly!
 */

`timescale 1ns / 1ps
`default_nettype none

`ifdef NO_PRIMITIVES
`include "./sky130_fd_sc_hd__udp_dff_ps_pp_pg_n.blackbox.v"
`else
primitive sky130_fd_sc_hd__udp_dff$PS_pp$PG$N (
    Q       ,
    D       ,
    CLK     ,
    SET     ,
    NOTIFIER,
    VPWR    ,
    VGND
);

    output Q       ;
    input  D       ;
    input  CLK     ;
    input  SET     ;
    input  NOTIFIER;
    input  VPWR    ;
    input  VGND    ;

    reg Q;

    table
     //          D           CLK  SET  NOTIFIER VPWR VGND :  Qt : Qt+1
                 *            b    0      ?      1    0   :  ?  :  -    ; // data event, hold unless CP==x
                 ?           (?0)  0      ?      1    0   :  ?  :  -    ; // CP => 0, hold
                 ?            b   (?0)    ?      1    0   :  ?  :  -    ; // S => 0, hold unless CP==x
                 ?            ?    1      ?      1    0   :  ?  :  1    ; // async set
                 0            r    0      ?      1    0   :  ?  :  0    ; // clock data on CP
                 1            r    ?      ?      1    0   :  ?  :  1    ; // clock data on CP
                 0           (x1)  0      ?      1    0   :  0  :  0    ; // possible CP, hold when D==Q==0
                 1           (x1)  ?      ?      1    0   :  1  :  1    ; // possible CP, hold when D==Q==1
                 0            x    0      ?      1    0   :  0  :  0    ; // unkown CP, hold when D==Q==0
                 1            x    ?      ?      1    0   :  1  :  1    ; // unkown CP, hold when D==Q==1
                 ?            b   (?x)    ?      1    0   :  1  :  1    ; // S=>x, hold when Q==1 unless CP==x
                 ?           (?0)  x      ?      1    0   :  1  :  1    ;
        // ['IfDef(functional)', '']                 ?            ?    ?      *      1    0   :  ?  :  -    ; // Q => - on any change on notifier
        // ['Else', '']                 ?            ?    ?      *      1    0   :  ?  :  x    ; // Q => X on any change on notifier
        // ['EndIfDef(functional)', '']                 ?            ?    ?      ?      *    ?   :  ?  :  x    ; // Q => X on any change on vpwr
                 ?            ?    ?      ?      ?    *   :  ?  :  x    ; // Q => X on any change on vgnd
    endtable
endprimitive
`endif // NO_PRIMITIVES

`default_nettype wire
`endif  // SKY130_FD_SC_HD__UDP_DFF_PS_PP_PG_N_V
