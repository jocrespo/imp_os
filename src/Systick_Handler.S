/*
*
*/
	.syntax unified
	.data
state: .word 0
	.text
	.global SysTick_Handler
	.extern sp1,sp2
	.thumb_func
SysTick_Handler:
	ldr r0, =state // r0 = &state
	ldr r1,[r0] // r1=*r0
	tbb [pc,r1]
tabla:
	.byte (case0-tabla)/2
	.byte (case1-tabla)/2
	.byte (case2-tabla)/2
	.byte (case_end-tabla)/2 // numero de "cases" debe ser potencia de 2

case0: // if (state==0)  Main, pasar a 1
	//state=1
	mov r1,1
	str r1,[r0]
	//sp<-sp1
	ldr r0, =sp1
	ldr r1, [r0]
	msr msp,r1
	//break
	b case_end

case1: // pasar de 1 a 2
	//state=2
	mov r1,2
	str r1,[r0]
	//sp1<-sp
	ldr r0,=sp1
	mrs r1,msp
	str r1,[r0]
	//sp<-sp2
	ldr r0,=sp2
	ldr r1,[r0]
	msr msp,r1
	//break
	b case_end

case2: // else   pasar de 2 a 1
	//state=1
	mov r1,1
	str r1,[r0]
	//sp2<-sp
	ldr r0,=sp2
	mrs r1,msp
	str r1,[r0]
	//sp<-sp1
	ldr r0,=sp1
	ldr r1,[r0]
	msr msp,r1
	//break innecesario

case_end:
	bx lr
