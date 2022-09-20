`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    00:10:43 03/05/2016 
// Design Name: 
// Module Name:    subtract8 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module subtract8(
    input [7:0] x,
    input [7:0] y,
    input cin,
    output cout,
    output [7:0] sub,
	 output overflow,
	 output sign,
	 output zero
    );

wire c1,c2,c3,c4,c5,c6,c7;

subtrack2 a0(
x[0],
y[0],
cin,
c1,
sub[0]

);

subtrack2 a1(
x[1],
y[1],
c1,
c2,
sub[1]

);
subtrack2 a2(
x[2],
y[2],
c2,
c3,
sub[2]
);

subtrack2 a3(
x[3],
y[3],
c3,
c4,
sub[3]

);

subtrack2 a4(
x[4],
y[4],
c4,
c5,
sub[4]

);

subtrack2 a5(
x[5],
y[5],
c5,
c6,
sub[5]

);

subtrack2 a6(
x[6],
y[6],
c6,
c7,
sub[6]

);

subtrack2 a7(
x[7],
y[7],
c7,
sub[7],
cout
);

or(zero,sub,0);
xor(overflow,c6,c7);
and(sign,sub,128);

endmodule
