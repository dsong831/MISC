// @ made by SHG_190612
/**********************************************************************
// # IR Receive Function #
//
// IR format = lead + address(8bit) + address_bar(8bit) + data(8bit) + data_bar(8bit)
//
// Test condition
//  - timer counter 1step = 10us
//  - timer counter range = 16bit (0x0000~0xFFFF; 65535)
//
// To use only external interrupt
//  - falling edge
//  - check timer counter value per external interrupt
***********************************************************************/
#define LEAD_STATUS		0			// check ir lead
#define DATA_STATUS	1			// check ir data
#define LEAD_MAX		1622		// (9ms+4.5ms) + 20%
#define LEAD_MIN		1078		// (9ms+4.5ms) - 20%
#define DATA0_MAX		134			// (560us+560us) + 20%
#define DATA0_MIN		89			// (560us+560us) - 20%
#define DATA1_MAX		272			// (560us+1680us) + 20%
#define DATA1_MIN		179			// (560us+1680us) - 20%

unsigned char gu8IR_status;			// 1: check ir data , 0: check ir lead
unsigned char gu8IR_datacnt;			// 32 is max. (address 8 + address_bar 8 + data 8 + data_bar 8)
unsigned long gu32IR_pre_timercnt;	// save old timercnt (to compare old and new)
unsigned long gu32IR_fulldata;			// address 8 + address_bar 8 + data 8 + data_bar 8

// For user code
unsigned char gu8IR_compflag;	// 1: complete rec. , 0: not yet
unsigned char gu8IR_addrdata;
unsigned char gu8IR_userdata;


void IR_Clear(void)					// IR init
{
	gu8IR_status = 0;
	gu8IR_datacnt = 0;
	gu32IR_pre_timercnt = 0;
	gu32IR_fulldata = 0;
	gu8IR_compflag = 0;
}

///////////////////////////////////////////////////////
// To copy this function then change 3points for using user code
//  - save timercounter value
//  - clear timercounter value
//  - clear external interrupt flag
//////////////////////////////////////////////////////
void EXINT_IRQ(void)
{
	unsigned long u32IR_timercnt;
	unsigned long u32IR_period;

	u32IR_timercnt = Timer_GetCnt(T0);			/* save timercounter value */
	u32IR_period = u32IR_timercnt - gu32IR_pre_timercnt;

	// complete receive
	if(gu8IR_datacnt == 32)
	{
		gu8IR_addrdata = (unsigned char)((gu32IR_fulldata>>24)&(0xFF));
		gu8IR_userdata = (unsigned char)((gu32IR_fulldata>>8)&(0xFF));
		IR_Clear();
		gu8IR_compflag = 1;
	}
	// continue receive
	else
	{
		switch(gu8IR_status)
		{
			// check ir lead
			case LEAD_STATUS:
				if((u32IR_period>LEAD_MIN) && (u32IR_period<LEAD_MAX))
				{
					gu8IR_datacnt = 0;
					gu32IR_pre_timercnt = 0;
					gu8IR_status = 1;
					Timer_MatInt.FlagClr(T0);			/* clear match-interrupt flag & timercounter value */
				}
				else
				{
					IR_Clear();
				}
				break;
			// check ir data
			case DATA_STATUS:
				gu32IR_fulldata = gu32IR_fulldata << 1;
				// data is '0'
				if((u32IR_period>DATA0_MIN) && (u32IR_period<DATA0_MAX))
				{
					gu8IR_datacnt++;
				}
				// data is '1'
				else if((u32IR_period>DATA1_MIN) && (u32IR_period<DATA1_MAX))
				{
					gu32IR_fulldata |= 0x1;
					gu8IR_datacnt++;
				}
				else
				{
					IR_Clear();
				}
				break;
			default:
				IR_Clear();
				break;	
		}
	}

	gu32IR_pre_timercnt = u32IR_timercnt;		// save timercounter old value
	ExInt_ClrFlag(PB0);						/* clear external interrupt flag */
}

// Example main code
void API_IR_REC(void)
{
	// 1st IR init
	IR_Clear();

	// 2nd IR roop start
	while(1)
	{
		switch(gu8IR_userdata)
		{
			case 0x00:
				// Add user function
				break;
			case 0x01:
				// Add user function
				break;
			default:
				break;
		}
	}
}

