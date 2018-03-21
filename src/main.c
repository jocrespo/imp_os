/*==================[inclusions]=============================================*/

#include "../../imp_os/inc/main.h"

#include "board.h"

/*==================[macros and definitions]=================================*/

/** */
#define STACK_SIZE 128

/** delay in milliseconds */
#define DELAY_MS 500
/** led number to toggle */
#define LED_1 0
#define LED_2 1

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/** @brief hardware initialization function
 *	@return none
 */
//static void initHardware(void);
//void start_os(void);
//void init_task(void (*entry_point)(void) ,uint8_t * stack, uint32_t stack_pointer);


/*==================[internal data definition]===============================*/
/** @brief Stacks de la tareas*/
static uint32_t pila1[STACK_SIZE];
static uint32_t pila2[STACK_SIZE];

/** @brief used for delay counter */
static uint32_t pausems_count_1;
static uint32_t pausems_count_2;

/*==================[external data definition]===============================*/

uint32_t sp1;
uint32_t sp2;

/*==================[internal functions definition]==========================*/

static void pausems1(uint32_t t)
{
	pausems_count_1 = t;
	while (pausems_count_1 != 0) {
		__WFI();
	}
}

static void pausems2(uint32_t t)
{
	pausems_count_2 = t;
	while (pausems_count_2 != 0) {
		__WFI();
	}
}

static void start_os(void)
{

};

static void init_task(
	void (*entry_point)(void) , /* vector reservado */
	uint32_t * stack, /* puntero (por referencia) */
	uint32_t stack_len,/*long. del vector (en 4B)*/
	uint32_t * sp) /*long. del vector (words)*/
{
	*sp = (uint32_t)(stack+stack_len-9); // Stack pointer
	stack[stack_len-1] = 1<<24; // PSR, PSR.T=1
	stack[stack_len-2] = (uint32_t)entry_point; // PC
	stack[stack_len-9] = 0xFFFFFFF9; // PC
};


static void initHardware(void)
{
	Board_Init();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
	NVIC_SetPriority(PendSV_IRQn,(1 << __NVIC_PRIO_BITS)-1); // Menor prioridad  100000b - 1 = 011111b
	// El resto de prioridades se dan por defecto, con valor 0 (mayor prioridad)
}

/*==================[external functions definition]==========================*/

void task1(void)
{
	int i;
	while (1) {
		i++;
		//Board_LED_Toggle(LED_1);
		//pausems1(DELAY_MS);
	}
}

void task2(void)
{
	int j;
	while (1) {
		j++;
		//Board_LED_Toggle(LED_2);
		//pausems2(DELAY_MS);
	}
}

void SysTick_Handler(void){
	SCB->ICSR= SCB_ICSR_PENDSVSET_Msk;


	__ISB();


	__DSB();
}

uint32_t get_Next_Context(uint32_t sp){

	static int actual_task = -1;
	uint32_t next_sp;

	switch(actual_task){
	case 1:
		sp1 = sp;
		next_sp = sp2;
		actual_task = 2;
		break;
	case 2:
		sp2 = sp;
		next_sp = sp1;
		actual_task = 1;
		break;
	default:
		next_sp = sp1;
		actual_task = 1;
		break;
	}

	return next_sp;
}

int main(void)
{
	initHardware();

	init_task(task1, pila1, STACK_SIZE ,&sp1);
	init_task(task2, pila2, STACK_SIZE, &sp2);

	start_os();

	while (1) {
	}
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
