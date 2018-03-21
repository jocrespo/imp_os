/*
*
*/
	.syntax unified

	.text

	.global PendSV_Handler

	.extern get_Next_Context

	.thumb_func
PendSV_Handler:
	push {lr}
	mrs r0,msp
	bl get_Next_Context
	msr msp,r0
	pop {lr}
	bx lr
