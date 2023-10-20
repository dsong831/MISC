#include <string.h>
#include "common.h"

#include "module.h"


typedef struct
{
	DeviceId moduleDeviceId;
}Main_Cnxt;

static Main_Cnxt gMainCnxt;


int main(void)
{
	sERR_Code ret;
	MODULE_ConfigParameters moduleConfigParameters;

	memset(&gMainCnxt, 0x0, sizeof(gMainCnxt));

	gMainCnxt.moduleDeviceId = 0;

	MODULE_Init();
	MODULE_GetConfigParameters(gMainCnxt.moduleDeviceId, &moduleConfigParameters);

	moduleConfigParameters.timeOut = 0xF00000;

	MODULE_SetConfigParameters(gMainCnxt.moduleDeviceId, &moduleConfigParameters);

	ret = MODULE_FunctionTemplate(gMainCnxt.moduleDeviceId);

	/* Common Function Example */
	WRITE_BYTE(ADDR_CONFIG_REG, sREG_FIELD_DATA(CONFIG_REG, field0, 1) | sREG_FIELD_DATA(CONFIG_REG, field4, 0xA));
	printf(">>>> 0x%.2X\n\r", READ_BYTE(ADDR_CONFIG_REG));
	printf(">>>> 0x%.1X\n\r", READ_FIELD(ADDR_CONFIG_REG, CONFIG_REG, field0));
	WRITE_FIELD(ADDR_CONFIG_REG, CONFIG_REG, field0, 0);
	printf(">>>> 0x%.2X\n\r", READ_BYTE(ADDR_CONFIG_REG));


	while(1)
	{
		__nop();
	}
}
