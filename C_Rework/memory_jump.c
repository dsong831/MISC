static void SwitchExecution(void)
{
	volatile uint32_t *appVectorTable = (volatile uint32_t *)APP_TEXT_ADDR;
	void (*function_ptr)() = (void (*)())appVectorTable[1];

	NVIC_SetPriorityGrouping(0);
	__set_PRIMASK(0);
	__set_MSP(appVectorTable[0]);
	function_ptr();
}
