#include <string.h>
#include "common.h"

#include "module.h"

/* Todo: refer to the device header file */
#define USE_TEMPLATE_HEADER		(1)		// for template debugging
#if (USE_TEMPLATE_HEADER)
typedef struct
{
	volatile unsigned int REG;
}MODULE_Type;
#define MODULE0		((MODULE_Type*) 0x40030000UL)
#define MODULE1		((MODULE_Type*) 0x40030004UL)
#else
#include "<device>.h"
#endif

typedef struct
{
	MODULE_Type* module;
	MODULE_ConfigParameters configParameters;
}MODULE_Connection;

typedef struct
{
	MODULE_Connection con[MODULE_DEVICE_NUMBER];
}MODULE_Cnxt;

static MODULE_Type* devices[MODULE_DEVICE_NUMBER] = {MODULE0, MODULE1};
static MODULE_Cnxt gCnxt;


void MODULE_Init(void)
{
	unsigned char deviceIndex;

	memset(&gCnxt, 0x0, sizeof(gCnxt));

	for(deviceIndex=0; deviceIndex<(sizeof(devices)/sizeof(devices[0])); deviceIndex++)
	{
		memset(&gCnxt.con[deviceIndex], 0x0, sizeof(gCnxt.con[deviceIndex]));
		gCnxt.con[deviceIndex].module = devices[deviceIndex];

		/* Todo: insert your code */
		gCnxt.con[deviceIndex].configParameters.timeOut = INFINITE_TIMEOUT;
	}
}

void MODULE_GetConfigParameters(DeviceId deviceId, MODULE_ConfigParameters *configParameters)
{
	*configParameters = gCnxt.con[deviceId].configParameters;
}

void MODULE_SetConfigParameters(DeviceId deviceId, MODULE_ConfigParameters *configParameters)
{
	unsigned int regValue32;

	/* Todo: insert your code */
	if(configParameters->timeOut) {
		regValue32 = gCnxt.con[deviceId].module->REG;
		gCnxt.con[deviceId].module->REG = regValue32;
	}

	gCnxt.con[deviceId].configParameters = *configParameters;
}

sERR_Code MODULE_FunctionTemplate(DeviceId deviceId)
{
	sERR_Code ret = sERR_SUCCESS;
	unsigned int regValue32;

	/* Todo: insert your code */
	regValue32 = gCnxt.con[deviceId].module->REG;
	gCnxt.con[deviceId].module->REG = regValue32;

	return ret;
}
