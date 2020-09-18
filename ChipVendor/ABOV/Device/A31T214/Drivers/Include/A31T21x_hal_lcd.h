/**********************************************************************//**
* @file				A31T21x_hal_lcd.h
* @brief			Contains all functions support for firmware library on A31T21x
* @version	1.00
* @date			26. JUNE. 2020
* @author	ABOV M team
*
* Copyright(C) 2019, ABOV Semiconductor
* All rights reserved.
*
************************************************************************
* ABOV Disclaimer
*
*IMPORTANT NOTICE ? PLEASE READ CAREFULLY
*ABOV Semiconductor ("ABOV") reserves the right to make changes, corrections, enhancements, 
*modifications, and improvements to ABOV products and/or to this document at any time without notice. 
*ABOV does not give warranties as to the accuracy or completeness of the information included herein.
*Purchasers should obtain the latest relevant information of ABOV products before placing orders. 
*Purchasers are entirely responsible for the choice, selection, and use of ABOV products and 
*ABOV assumes no liability for application assistance or the design of purchasers?? products. No license, 
*express or implied, to any intellectual property rights is granted by ABOV herein. 
*ABOV disclaims all express and implied warranties and shall not be responsible or
*liable for any injuries or damages related to use of ABOV products in such unauthorized applications. 
*ABOV and the ABOV logo are trademarks of ABOV.
*All other product or service names are the property of their respective owners. 
*Information in this document supersedes and replaces the information previously
*supplied in any former versions of this document.
*2020 ABOV Semiconductor  All rights reserved
*
**********************************************************************/


#ifndef A31T21X_HAL_LCD_H
#define A31T21X_HAL_LCD_H


/* Includes -------------------------------------------------------------- */
#include "A31T21x.h"
#include "A31T21x_hal_aa_types.h"


/* Private macros -------------------------------------------------------- */
#define LCD_CR_INTBIAS												(0<<8)
#define LCD_CR_EXTBIAS											(1<<8)

#define LCD_CR_RLCD3													(0<<6)
#define LCD_CR_RLCD1													(1<<6)
#define LCD_CR_RLCD2													(2<<6)
#define LCD_CR_RLCD4													(3<<6)

#define LCD_CR_Duty_div8_Bias_div4		(0<<3)
#define LCD_CR_Duty_div6_Bias_div4		(1<<3)
#define LCD_CR_Duty_div5_Bias_div3		(2<<3)
#define LCD_CR_Duty_div4_Bias_div3		(3<<3)
#define LCD_CR_Duty_div3_Bias_div3		(4<<3)
#define LCD_CR_Duty_div3_Bias_div2		(5<<3)

#define LCD_CR_LCLK_128HZ								(0<<1)
#define LCD_CR_LCLK_256HZ								(1<<1)
#define LCD_CR_LCLK_512HZ								(2<<1)
#define LCD_CR_LCLK_1024HZ							(3<<1)

#define LCD_CR_DISP_ON											(1<<0)

#define LCD_BCCR_AUTOBIAS_OFF				(0<<12)
#define LCD_BCCR_AUTOBIAS_ON				(1<<12)

#define LCD_BCCR_BMSEL_1CLK					(0<<8)
#define LCD_BCCR_BMSEL_2CLK					(1<<8)
#define LCD_BCCR_BMSEL_3CLK					(2<<8)
#define LCD_BCCR_BMSEL_4CLK					(3<<8)
#define LCD_BCCR_BMSEL_5CLK					(4<<8)
#define LCD_BCCR_BMSEL_6CLK					(5<<8)
#define LCD_BCCR_BMSEL_7CLK					(6<<8)
#define LCD_BCCR_BMSEL_8CLK					(7<<8)

#define LCD_BCCR_CONTRAST_OFF			(0<<5)
#define LCD_BCCR_CONTRAST_ON				(1<<5)

#define LCD_BCCR_VLCD_31STEP					(0<<0)
#define LCD_BCCR_VLCD_30STEP					(1<<0)
#define LCD_BCCR_VLCD_29STEP					(2<<0)
#define LCD_BCCR_VLCD_28STEP					(3<<0)
#define LCD_BCCR_VLCD_27STEP					(4<<0)
#define LCD_BCCR_VLCD_26STEP					(5<<0)
#define LCD_BCCR_VLCD_25STEP					(6<<0)
#define LCD_BCCR_VLCD_24STEP					(7<<0)
#define LCD_BCCR_VLCD_23STEP					(8<<0)
#define LCD_BCCR_VLCD_22STEP					(9<<0)
#define LCD_BCCR_VLCD_21STEP					(10<<0)
#define LCD_BCCR_VLCD_20STEP					(11<<0)
#define LCD_BCCR_VLCD_19STEP					(12<<0)
#define LCD_BCCR_VLCD_18STEP					(13<<0)
#define LCD_BCCR_VLCD_17STEP					(14<<0)
#define LCD_BCCR_VLCD_16STEP					(15<<0)


/* Public Types --------------------------------------------------------------- */
typedef struct {
	uint32_t	tLCDDR;
	uint32_t	tIRSEL;
	uint32_t	tDBS;
	uint32_t	tLCLK;
	uint32_t	tDISP;

	uint32_t	tLCDABC;
	uint32_t	tBMSEL;
	uint32_t	tLCTEN;
	uint32_t	tVLCD;
}LCD_CFG_Type;


/* Public Functions ----------------------------------------------------------- */
void HAL_LCD_Init(LCD_Type *LCDn, LCD_CFG_Type *LCD_Config);
void HAL_LCD_DISP_ON(void);
void HAL_LCD_DISP_OFF(void);


#endif /* A31T21X_HAL_LCD_H */


/* --------------------------------- End Of File ------------------------------ */
