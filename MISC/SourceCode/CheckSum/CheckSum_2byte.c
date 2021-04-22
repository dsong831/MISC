/* ########################################################################### */
/* CheckSum 2-byte */

void CheckSum_2byte(void)
{
	uint32_t i;
	uint16_t sum;
	uint16_t checksum;
	uint32_t startAddr = 0x00000000;
	uint32_t stopAddr = 0x60000;

	sum = 0;
	for(i=0; i<stopAddr; i++)
	{
		sum += *((volatile unsigned char *) startAddr + i);
	}

	checksum = (sum & 0xffff);
}
