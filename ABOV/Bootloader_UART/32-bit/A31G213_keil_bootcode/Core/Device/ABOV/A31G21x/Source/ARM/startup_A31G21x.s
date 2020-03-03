;/*****************************************************************************
; * @file:    startup_A31G213.s
; * @purpose: CMSIS Cortex-M0+ Core Device Startup File 
; *           for the ARM 'Microcontroller Prototyping System' 
; * @version: V1.0
; * @date:    19. Aug. 2009
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; *
; * Copyright (C) 2008-2009 ARM Limited. All rights reserved.
; * ARM Limited (ARM) is supplying this software for use with Cortex-M0 
; * processor based microcontrollers.  This file can be freely distributed 
; * within development tools that are supporting such ARM based processors. 
; *
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; *****************************************************************************/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              	; Top of Stack
                DCD     Reset_Handler             		; Reset Handler
                DCD     NMI_Handler               		; NMI Handler
                DCD     HardFault_Handler         	; Hard Fault Handler
                DCD     0                         			; Reserved
                DCD     0                         			; Reserved
                DCD     0                         			; Reserved
                DCD     0                         			; Reserved
                DCD     0                         			; Reserved
                DCD     0                         			; Reserved
                DCD     0                        			; Reserved
                DCD     SVC_Handler           		; SVCall Handler
                DCD     0                         			; Reserved
                DCD     0                         			; Reserved
                DCD     PendSV_Handler           		; PendSV Handler
                DCD     SysTick_Handler           		; SysTick Handler

                ; External Interrupts
                DCD     LVI_IRQHandler                        	; IRQ0
                DCD     SYSCLKFAIL_IRQHandler          ; IRQ1
                DCD     WDT_IRQHandler                     	; IRQ2
                DCD     GPIOAB_IRQHandler                	; IRQ3
                DCD     GPIOCD_IRQHandler	                ; IRQ4
                DCD     GPIOE_IRQHandler                  	; IRQ5
                DCD     GPIOF_IRQHandler                   ; IRQ6
                DCD     TIMER10_IRQHandler                ; IRQ7
                DCD     TIMER11_IRQHandler                ; IRQ8
                DCD     TIMER12_IRQHandler                ; IRQ9
                DCD     I2C0_IRQHandler                       ; IRQ10
                DCD     USART10_IRQHandler               ; IRQ11
                DCD     WT_IRQHandler                        ; IRQ12
                DCD     TIMER30_IRQHandler                ; IRQ13
                DCD     I2C1_IRQHandler                       ; IRQ14
                DCD     TIMER20_IRQHandler                ; IRQ15
                DCD     TIMER21_IRQHandler                ; IRQ16
                DCD     USART11_IRQHandler               ; IRQ17
                DCD     ADC_IRQHandler                       ; IRQ18
                DCD     UART0_IRQHandler                   ; IRQ19
                DCD     UART1_IRQHandler                   ; IRQ20
                DCD     TIMER13_IRQHandler                ; IRQ21
				DCD     NULL_IRQHandler                ; IRQ22
                DCD     NULL_IRQHandler                ; IRQ23
                DCD     NULL_IRQHandler                ; IRQ24
                DCD     SPI20_IRQHandler                      ; IRQ25
                DCD     SPI21_IRQHandler        			 ; IRQ26
                DCD     NULL_IRQHandler         ; IRQ27 DAC
                DCD     LED_IRQHandler                       ; IRQ28
                DCD     TOUCH_IRQHandler                  ; IRQ29
				DCD     NULL_IRQHandler                      ; IRQ30
                DCD     CRC_IRQHandler           ; IRQ31


                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             	[WEAK]
                IMPORT  __main
                LDR		R0, =__main
                BX		R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                     [WEAK]
                B       .
                ENDP
HardFault_Handler		PROC
                EXPORT  HardFault_Handler            [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                     [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler               [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler                [WEAK]
                B       .
                ENDP

NULL_IRQHandler PROC
                EXPORT  LVI_IRQHandler                         [WEAK]
                EXPORT  SYSCLKFAIL_IRQHandler           [WEAK]
                EXPORT  WDT_IRQHandler                      [WEAK]
                EXPORT  GPIOAB_IRQHandler                 [WEAK]		
                EXPORT  GPIOCD_IRQHandler                 [WEAK]
                EXPORT  GPIOE_IRQHandler                    [WEAK]
                EXPORT  GPIOF_IRQHandler                    [WEAK]
                EXPORT  TIMER10_IRQHandler                 [WEAK]
                EXPORT  TIMER11_IRQHandler                 [WEAK]                
                EXPORT  TIMER12_IRQHandler                 [WEAK]
                EXPORT  I2C0_IRQHandler                        [WEAK]
                EXPORT  USART10_IRQHandler                [WEAK]
                EXPORT  WT_IRQHandler                         [WEAK]
                EXPORT  TIMER30_IRQHandler                 [WEAK]
                EXPORT  I2C1_IRQHandler                       [WEAK]
                EXPORT  TIMER20_IRQHandler                [WEAK]
                EXPORT  TIMER21_IRQHandler                [WEAK]	
                EXPORT  USART11_IRQHandler               [WEAK]					
                EXPORT  ADC_IRQHandler                       [WEAK]		
                EXPORT  UART0_IRQHandler                   [WEAK]		
                EXPORT  UART1_IRQHandler                   [WEAK]	
                EXPORT  TIMER13_IRQHandler                [WEAK]
				EXPORT  SPI20_IRQHandler                       [WEAK]		
                EXPORT  SPI21_IRQHandler          [WEAK]		
                EXPORT  LED_IRQHandler                        [WEAK]		
                EXPORT  TOUCH_IRQHandler                   [WEAK]
                EXPORT  CRC_IRQHandler           [WEAK]		

		
LVI_IRQHandler                        ; IRQ0
SYSCLKFAIL_IRQHandler          ; IRQ1
WDT_IRQHandler                     	; IRQ2
GPIOAB_IRQHandler                	; IRQ3
GPIOCD_IRQHandler                ; IRQ4
GPIOE_IRQHandler                  	; IRQ5
GPIOF_IRQHandler                   ; IRQ6
TIMER10_IRQHandler                ; IRQ7
TIMER11_IRQHandler                ; IRQ8
TIMER12_IRQHandler                ; IRQ9
I2C0_IRQHandler                       ; IRQ10
USART10_IRQHandler               ; IRQ11
WT_IRQHandler                        ; IRQ12
TIMER30_IRQHandler                ; IRQ13
I2C1_IRQHandler                       ; IRQ14
TIMER20_IRQHandler                ; IRQ15
TIMER21_IRQHandler                ; IRQ16
USART11_IRQHandler               ; IRQ17
ADC_IRQHandler                       ; IRQ18
UART0_IRQHandler                   ; IRQ19
UART1_IRQHandler                   ; IRQ20
TIMER13_IRQHandler                ; IRQ21
;TIMER14_IRQHandler                ; IRQ22
;TIMER15_IRQHandler                ; IRQ23
;TIMER16_IRQHandler                ; IRQ24
;I2C2_IRQHandler                      ; IRQ25
;USART12_13_IRQHandler         ; IRQ26
SPI20_IRQHandler                      ; IRQ25
SPI21_IRQHandler         		; IRQ26
;DAC_IRQHandler         ; IRQ27
LED_IRQHandler                       ; IRQ28
TOUCH_IRQHandler                  ; IRQ29
;USB_IRQHandler                      ; IRQ30
CRC_IRQHandler           ; IRQ31
                
                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
