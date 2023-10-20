#ifndef MODULE_H
#define MODULE_H

#define MODULE_DEVICE_NUMBER	2

typedef struct
{
	unsigned int timeOut;
}MODULE_ConfigParameters;


void MODULE_Init(void);
void MODULE_GetConfigParameters(DeviceId deviceId, MODULE_ConfigParameters *configParameters);
void MODULE_SetConfigParameters(DeviceId deviceId, MODULE_ConfigParameters *configParameters);


sERR_Code MODULE_FunctionTemplate(DeviceId deviceId);


#endif /*MODULE_H*/
