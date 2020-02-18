$NOMOD51
;------------------------------------------------------------------------------
;  This file is part of the C51 Compiler package
;  Copyright (c) 1988-2005 Keil Elektronik GmbH and Keil Software, Inc.
;  Version 8.01
;
;  *** <<< Use Configuration Wizard in Context Menu >>> ***
;------------------------------------------------------------------------------
;  STARTUP.A51:  This code is executed after processor reset.
;
;  To translate this file use A51 with the following invocation:
;
;     A51 STARTUP.A51
;
;  To link the modified STARTUP.OBJ file to your application use the following
;  Lx51 invocation:
;
;     Lx51 your object file list, STARTUP.OBJ  controls
;
;------------------------------------------------------------------------------
;
;  User-defined <h> Power-On Initialization of Memory
;
;  With the following EQU statements the initialization of memory
;  at processor reset can be defined:
;
; <o> IDATALEN: IDATA memory size <0x0-0x100>
;     <i> Note: The absolute start-address of IDATA memory is always 0
;     <i>       The IDATA space overlaps physically the DATA and BIT areas.
IDATALEN        EQU     100H
;
; <o> XDATASTART: XDATA memory start address <0x0-0xFFFF> 
;     <i> The absolute start address of XDATA memory
XDATASTART      EQU     0     
;
; <o> XDATALEN: XDATA memory size <0x0-0xFFFF> 
;     <i> The length of XDATA memory in bytes.
XDATALEN        EQU     300H      
;
; <o> PDATASTART: PDATA memory start address <0x0-0xFFFF> 
;     <i> The absolute start address of PDATA memory
PDATASTART      EQU     0H
;
; <o> PDATALEN: PDATA memory size <0x0-0xFF> 
;     <i> The length of PDATA memory in bytes.
PDATALEN        EQU     0H
;
;</h>
;------------------------------------------------------------------------------
;
;<h> Reentrant Stack Initialization
;
;  The following EQU statements define the stack pointer for reentrant
;  functions and initialized it:
;
; <h> Stack Space for reentrant functions in the SMALL model.
;  <q> IBPSTACK: Enable SMALL model reentrant stack
;     <i> Stack space for reentrant functions in the SMALL model.
IBPSTACK        EQU     0       ; set to 1 if small reentrant is used.
;  <o> IBPSTACKTOP: End address of SMALL model stack <0x0-0xFF>
;     <i> Set the top of the stack to the highest location.
IBPSTACKTOP     EQU     0xFF +1     ; default 0FFH+1  
; </h>
;
; <h> Stack Space for reentrant functions in the LARGE model.      
;  <q> XBPSTACK: Enable LARGE model reentrant stack
;     <i> Stack space for reentrant functions in the LARGE model.
XBPSTACK        EQU     0       ; set to 1 if large reentrant is used.
;  <o> XBPSTACKTOP: End address of LARGE model stack <0x0-0xFFFF>
;     <i> Set the top of the stack to the highest location.
XBPSTACKTOP     EQU     0xFFFF +1   ; default 0FFFFH+1 
; </h>
;
; <h> Stack Space for reentrant functions in the COMPACT model.    
;  <q> PBPSTACK: Enable COMPACT model reentrant stack
;     <i> Stack space for reentrant functions in the COMPACT model.
PBPSTACK        EQU     0       ; set to 1 if compact reentrant is used.
;
;   <o> PBPSTACKTOP: End address of COMPACT model stack <0x0-0xFFFF>
;     <i> Set the top of the stack to the highest location.
PBPSTACKTOP     EQU     0xFF +1     ; default 0FFH+1  
; </h>
;</h>
;------------------------------------------------------------------------------
;
;  Memory Page for Using the Compact Model with 64 KByte xdata RAM
;  <e>Compact Model Page Definition
;
;  <i>Define the XDATA page used for PDATA variables. 
;  <i>PPAGE must conform with the PPAGE set in the linker invocation.
;
; Enable pdata memory page initalization
PPAGEENABLE     EQU     0       ; set to 1 if pdata object are used.
;
; <o> PPAGE number <0x0-0xFF> 
; <i> uppermost 256-byte address of the page used for PDATA variables.
PPAGE           EQU     0
;
; <o> SFR address which supplies uppermost address byte <0x0-0xFF> 
; <i> most 8051 variants use P2 as uppermost address byte
PPAGE_SFR       DATA    0A0H
;
; </e>
;------------------------------------------------------------------------------
;*************************************************************************************
INTVEC_OFFADDR   EQU	0800H
;*************************************************************************************
; Standard SFR Symbols 
ACC     DATA    0E0H
B       DATA    0F0H
SP      DATA    81H
DPL     DATA    82H
DPH     DATA    83H
IE		DATA	0A8H								;hsj
EA		BIT		0AFH								;hsj, EA==IE.7
	
                NAME    ?C_STARTUP


;?C_C51STARTUP   SEGMENT   CODE
?STACK          SEGMENT   IDATA

                RSEG    ?STACK
                DS      1

                EXTRN CODE (?C_START)
                PUBLIC  ?C_STARTUP

                CSEG    AT      0
?C_STARTUP:     LJMP    STARTUP1
;hsj_start
                CSEG	AT	03H	; INT0
                LJMP	INTVEC_OFFADDR+03H
                ;DS      5				

                CSEG	AT	0BH	; INT1
                LJMP	INTVEC_OFFADDR+0BH
                ;DS      5
				
                CSEG	AT	13H	; INT2
                LJMP	INTVEC_OFFADDR+13H
                ;DS      5
				
                CSEG	AT	1BH	; INT3
                LJMP	INTVEC_OFFADDR+1BH
                ;DS      5
				
                CSEG	AT	23H	; INT4
                LJMP	INTVEC_OFFADDR+23H	
                ;DS      5
				
                CSEG	AT	2BH	; INT5
                LJMP	INTVEC_OFFADDR+2BH
                ;DS      5
				
                CSEG	AT	33H	; INT6
                LJMP	INTVEC_OFFADDR+33H
                ;DS      5
				
                CSEG	AT	3BH	; INT7
                LJMP	INTVEC_OFFADDR+3BH
                ;DS      5
				
                CSEG	AT	43H	; INT8
                LJMP	INTVEC_OFFADDR+43H
                ;DS      5
				
                CSEG	AT	4BH	; INT9
                LJMP	INTVEC_OFFADDR+4BH
                ;DS      5
				
                CSEG	AT	53H	; INT10
                LJMP	INTVEC_OFFADDR+53H
                ;DS      5
				
                CSEG	AT	5BH	; INT11
                LJMP	INTVEC_OFFADDR+5BH
                ;DS      5
				
                CSEG	AT	63H	; INT12
                LJMP	INTVEC_OFFADDR+63H
                ;DS      5
				
                CSEG	AT	6BH	; INT13
                LJMP	INTVEC_OFFADDR+6BH
                ;DS      5
				
                CSEG	AT	73H	; INT14
                LJMP	INTVEC_OFFADDR+73H	
                ;DS      5
				
                CSEG	AT	7BH	; INT15
                LJMP	INTVEC_OFFADDR+7BH
                ;DS      5
				
                CSEG	AT	83H	; INT16
                LJMP	INTVEC_OFFADDR+83H
                ;DS      5
				
                CSEG	AT	8BH	; INT17
                LJMP	INTVEC_OFFADDR+8BH
                ;DS      5
				
                CSEG	AT	93H	; INT18
                LJMP	INTVEC_OFFADDR+93H
                ;DS      5
				
                CSEG	AT	9BH	; INT19
                LJMP	INTVEC_OFFADDR+9BH
                ;DS      5
				
                CSEG	AT	0A3H	; INT20
                LJMP	INTVEC_OFFADDR+0A3H
                ;DS      5
				
                CSEG	AT	0ABH	; INT21
                LJMP	INTVEC_OFFADDR+0ABH
                ;DS      5
				
                CSEG	AT	0B3H	; INT22
                LJMP	INTVEC_OFFADDR+0B3H
                ;DS      5
				
                CSEG	AT	0BBH	; INT23
                LJMP	INTVEC_OFFADDR+0BBH
                ;DS      5
				
                CSEG	AT	0C3H	; INT24
                LJMP	INTVEC_OFFADDR+0C3H	
                ;DS      5
				
                CSEG	AT	0CBH	; INT25
                LJMP	INTVEC_OFFADDR+0CBH
                ;DS      5
				
                CSEG	AT	0D3H	; INT26
                LJMP	INTVEC_OFFADDR+0D3H
                ;DS      5
				
                CSEG	AT	0DBH	; INT27
                LJMP	INTVEC_OFFADDR+0DBH
                ;DS      5
				
                CSEG	AT	0E3H	; INT28
                LJMP	INTVEC_OFFADDR+0E3H
                ;DS      5
				
                CSEG	AT	0EBH	; INT29
                LJMP	INTVEC_OFFADDR+0EBH
                ;DS      5
				
                CSEG	AT	0F3H	; INT30
                LJMP	INTVEC_OFFADDR+0F3H
                ;DS      5
				
                CSEG	AT	0FBH	; INT31
                LJMP	INTVEC_OFFADDR+0FBH
                ;DS      2
;hsj_end

                CSEG    AT      0100H			;hsj
 ;               RSEG    ?C_C51STARTUP	;The RSET statement selects a relocatable segment that was previously declared using the SEGMENT statement.
															;switch to this code segment
STARTUP1:
				CLR		EA							;hsj, No need,  default(EA==IE.7==0)

IF IDATALEN <> 0
                MOV     R0,#IDATALEN - 1
                CLR     A
IDATALOOP:      MOV     @R0,A
                DJNZ    R0,IDATALOOP
ENDIF

IF XDATALEN <> 0
                MOV     DPTR,#XDATASTART
                MOV     R7,#LOW (XDATALEN)
  IF (LOW (XDATALEN)) <> 0
                MOV     R6,#(HIGH (XDATALEN)) +1
  ELSE
                MOV     R6,#HIGH (XDATALEN)
  ENDIF
                CLR     A
XDATALOOP:      MOVX    @DPTR,A
                INC     DPTR
                DJNZ    R7,XDATALOOP
                DJNZ    R6,XDATALOOP
ENDIF

IF PPAGEENABLE <> 0
                MOV     PPAGE_SFR,#PPAGE
ENDIF

IF PDATALEN <> 0
                MOV     R0,#LOW (PDATASTART)
                MOV     R7,#LOW (PDATALEN)
                CLR     A
PDATALOOP:      MOVX    @R0,A
                INC     R0
                DJNZ    R7,PDATALOOP
ENDIF

IF IBPSTACK <> 0
EXTRN DATA (?C_IBP)

                MOV     ?C_IBP,#LOW IBPSTACKTOP
ENDIF

IF XBPSTACK <> 0
EXTRN DATA (?C_XBP)

                MOV     ?C_XBP,#HIGH XBPSTACKTOP
                MOV     ?C_XBP+1,#LOW XBPSTACKTOP
ENDIF

IF PBPSTACK <> 0
EXTRN DATA (?C_PBP)
                MOV     ?C_PBP,#LOW PBPSTACKTOP
ENDIF

                MOV     SP,#?STACK-1

; This code is required if you use L51_BANK.A51 with Banking Mode 4
;<h> Code Banking
; <q> Select Bank 0 for L51_BANK.A51 Mode 4
#if 0   
;     <i> Initialize bank mechanism to code bank 0 when using L51_BANK.A51 with Banking Mode 4.
EXTRN CODE (?B_SWITCH0)
                CALL    ?B_SWITCH0      ; init bank mechanism to code bank 0
#endif
;</h>
                LJMP    ?C_START			;Jump to main()

;*************************************************************************************
?PR?UserModeStart??C_STARTUP	SEGMENT CODE
		PUBLIC	UserModeStart
	
		RSEG	?PR?UserModeStart??C_STARTUP		;switch ?PR?UserModeStart??C_STARTUP
		USING	0
UserModeStart:
		LJMP	INTVEC_OFFADDR
		RET
; END OF UserModeStart
;*************************************************************************************
				

                END
