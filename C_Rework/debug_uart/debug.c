
static uint8_t ascii2hex(uint8_t asciiData) {
	uint8_t hexData;

	switch(asciiData) {
		case 0x41:
			hexData = 0x0A;
			break;
		case 0x42:
			hexData = 0x0B;
			break;
		case 0x43:
			hexData = 0x0C;
			break;
		case 0x44:
			hexData = 0x0D;
			break;
		case 0x45:
			hexData = 0x0E;
			break;
		case 0x46:
			hexData = 0x0F;
			break;
		case 0x61:
			hexData = 0x0a;
			break;
		case 0x62:
			hexData = 0x0b;
			break;
		case 0x63:
			hexData = 0x0c;
			break;
		case 0x64:
			hexData = 0x0d;
			break;
		case 0x65:
			hexData = 0x0e;
			break;
		case 0x66:
			hexData = 0x0f;
			break;
		default:
			hexData = asciiData;
			break;
	}

	return hexData;
}

#define DEBUG_STATUS_PARSING_DONE		(1<<4)
#define DEBUG_STATUS_ADDRESS_DONE		(1<<1)
#define DEBUG_STATUS_WRITE_CONTINUE		(1<<0)
#define DEBUG_ADDR_MIN					(0x40030000UL)
#define DEBUG_ADDR_MAX					(0x40035000UL)
void Debug_UartPolling(void) {
	int i;
	uint8_t *dataArray;
	static uint32_t dataIndex = 0;

	static uint8_t status = 0;
	static uint32_t address = 0;

	uint8_t regValue8;
	uint32_t regValue32;

	if(CMSDK_UART2->STATE & CMSDK_UART_STATE_RXBF_Msk) {
		dataArray[dataIndex] = CMSDK_UART2->DATA;
		while((CMSDK_UART2->STATE & CMSDK_UART_STATE_TXBF_Msk));
		CMSDK_UART2->DATA = dataArray[dataIndex];

		/* Parsing the data */
		regValue32 = 0;
		if((dataArray[dataIndex] == 0x0A) || (dataArray[dataIndex] == 0x0D)) {
			for(i=0; i<dataIndex; i++) {
				regValue8 = ascii2hex(dataArray[i]);
				regValue32 |= ((regValue8 & 0x0F) << ((dataIndex-i-1)*4));
			}
			dataIndex = 0;
			status |= DEBUG_STATUS_PARSING_DONE;
		} else if((dataIndex+1) != 0) {
			dataIndex += 1;
		}

		/* Register R/W Sequence */
		if(status & DEBUG_STATUS_PARSING_DONE) {
			status &= ~DEBUG_STATUS_PARSING_DONE;

			if(!(status & DEBUG_STATUS_ADDRESS_DONE)) {
				status |= DEBUG_STATUS_ADDRESS_DONE;

				if(regValue32 & (uint32_t)(0x80000000)) {
					status |= DEBUG_STATUS_WRITE_CONTINUE;
					regValue32 &= ~(uint32_t)(0x80000000);
				}

				if((regValue32 >= DEBUG_ADDR_MIN) && (regValue32 <= DEBUG_ADDR_MAX)) {
					address = regValue32;
					if(status & DEBUG_STATUS_WRITE_CONTINUE) {
						printf(">> Address 0x%.8X = 0x%.8X [WRITE CONTINUE]\n\r", address, *(volatile uint32_t *)address);
					} else {
						status &= ~DEBUG_STATUS_ADDRESS_DONE;
						printf(">> Address 0x%.8X = 0x%.8X [READ ONLY]\n\r", address, *(volatile uint32_t *)address);
					}
				} else {
					status = 0;
					address = 0;
				}
			} else {
				status &= ~DEBUG_STATUS_ADDRESS_DONE;
				if(status & DEBUG_STATUS_WRITE_CONTINUE) {
					printf(">> [WRITE_DATA] 0x%.8X\n\r", regValue32);
					*(volatile uint32_t *)address = regValue32;
				}
				status = 0;
				address = 0;
			}
		}
	}
}
