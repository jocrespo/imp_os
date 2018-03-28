/*
*
*/
	.syntax unified

	.text

	.global PendSV_Handler

	.extern get_Next_Context

	.thumb_func
PendSV_Handler:
	push {r4-r11,lr}
	mrs r0,msp
	bl get_Next_Context
	msr msp,r0
	pop {r4-r11,lr} // Mismo orden que el push, la arquitectura lo tiene en cuenta y lo hace inverso
	bx lr
