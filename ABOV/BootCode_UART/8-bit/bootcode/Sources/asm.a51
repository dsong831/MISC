
$NOMOD51

NAME	CODEASM

_Flash_                SEGMENT CODE
	
	PUBLIC	Flash_Enter
	PUBLIC	_Flash_LoadData

	RSEG  _Flash_
_Flash_LoadData:
	  mov a, r7
	  mov dph, r4
	  mov dpl, r5
	  db  0a5H              ;download @dptr with Acc. movc @(dptr++),a
	RET  	
; END OF _Flash_LoadData

Flash_Enter:
	  mov a, #0aaH         ;load Acc with AAH
	  mov dph, #0f5H       ;load DPH with F5H
	  mov dpl, #55H       ;load DPL with 55H
	  db  0a5H              ;download @dptr with Acc. movc @(dptr++),a

	  mov a, #55H         ;load Acc with 55H
	  mov dph, #0faH       ;load DPH with FAH
	  mov dpl, #0aaH       ;load DPL with AAH
	  db  0a5H              ;download @dptr with Acc. movc @(dptr++),a

	  mov a, #0a5H         ;load Acc with A5H
	  mov dph, #0f5H       ;load DPH with F5H
	  mov dpl, #55H       ;load DPL with 55H
	  db  0a5H              ;download @dptr with Acc. movc @(dptr++),a
	RET  	
; END OF Flash_Enter

	END
