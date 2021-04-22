///////////////////////////////////////////////////////////////////////////////
/* ## device_templete.h ## */

enum bit_name
{

};

struct config_name
{

};

enum callback_event
{
	CALLBACK_EVENT_DONE		=	(1 << 0),
	CALLBACK_EVENT_ERROR	=	(1 << 1),
	//...
};

typedef void (*user_irq_handler_t)(enum callback_event event, void *context);

///////////////////////////////////////////////////////////////////////////////
/* ## device_templete.c ## */

struct task_ctrl_block
{
	user_irq_handler_t	handler;
	void				*context;
	//...
}

static struct task_ctrl_block g_tcb;

int32_t HAL_XXX_Init(..., user_irq_handler_t handler, void *context)
{
	struct task_ctrl_block *tcb;
	
	tcb = &g_tcb;
	//...
	memset(tcb, 0, sizeof(struct task_ctrl_block));
	tcb->handler = handler;
	tcb->context = context;
	//...
}

static void XXX_IRQHandler(enum channel_id id)
{
	struct task_ctrl_block *tcb;
	volatile uint32_t event = 0;
	
	tcb = &g_tcb;
	//...
	event |= CALLBACK_EVENT_DONE;
	//...
	if (tcb->handler && event)
	{
		tcb->handler(event, tcb->context);
	}
}

