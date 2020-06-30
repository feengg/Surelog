///////////////////////////////////////////////////////
//     Copyright (c) 2009 Xilinx Inc.
// 
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
// 
//        http://www.apache.org/licenses/LICENSE-2.0
// 
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
///////////////////////////////////////////////////////
//   ____   ___
//  /   /\/   / 
// /___/  \  /     Vendor      : Xilinx 
// \  \    \/      Version : 10.1
//  \  \           Description : 
//  /  /                      
// /__/   /\       Filename    : EFUSE_USR.v
// \  \  /  \      Timestamp : Wed Mar 19 12:34:06 2008
//  \__\/\__ \                    
//                                 
// Revision:
//    03/19/08 - Initial version.  
//    12/13/11 - Added `celldefine and `endcelldefine (CR 524859).
// End Revision

`timescale 1 ps / 1 ps
 

`celldefine


module EFUSE_USR (
  EFUSEUSR
);

  parameter [31:0] SIM_EFUSE_VALUE = 32'h00000000;







    
  output [31:0] EFUSEUSR;

  assign EFUSEUSR = SIM_EFUSE_VALUE;

  specify

      specparam PATHPULSE$ = 0;

  endspecify

endmodule

`endcelldefine

