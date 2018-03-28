/**
 * Mejorar get next context, init_task... para un programa de N tareas
 *
 * Separar el codigo de las tareas del codigo del SO
 */


/*==================[inclusions]=============================================*/

#include "../../imp_os/inc/main.h"

#include "board.h"
#include "so.h"
#include <string.h>

/*==================[macros and definitions]=================================*/

/** Size del stack en palabras de 32bits*/
#define STACK_SIZE 128

/** delay in milliseconds */
#define DELAY_MS 500
/** led number to toggle */
#define LED_1 0
#define LED_2 1

#define INITIAL_xPSR 1<<24

#define EXCEPTION_RETURN 0xFFFFFFF9

/** @brief Nmero maximo de tareas*/
#define MAX_NUM_TASKS 16

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

/** @brief numero de tasks del sistema, usado como indice en la struct general */
static uint8_t num_tasks = 0;

uint32_t sp_vector[MAX_NUM_TASKS];
uint32_t sp1;
uint32_t sp2;
/*==================[external data definition]===============================*/



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
	uint32_t stack_len/*long. del vector (en 4B)*/
	)
{
	memset(stack,0,STACK_SIZE*sizeof(uint32_t));
	sp_vector[num_tasks]=(uint32_t)(stack+stack_len-17); // Stack pointer
	stack[stack_len-1] = INITIAL_xPSR; // PSR, PSR.T=1
	stack[stack_len-2] = (uint32_t)entry_point; // PC
	stack[stack_len-3] = 0; // LR
	stack[stack_len-4] = 0; // R12
	stack[stack_len-5] = 0; // R3
	stack[stack_len-6] = 0; // R2
	stack[stack_len-7] = 0; // R1
	stack[stack_len-8] = 0; // R0 (primer parametro, y retorno)
	stack[stack_len-9] = EXCEPTION_RETURN; // Exception return
	stack[stack_len-10] = 0; // R11
	stack[stack_len-11] = 0; // R10
	stack[stack_len-12] = 0; // R9
	stack[stack_len-13] = 0; // R8
	stack[stack_len-14] = 0; // R7
	stack[stack_len-15] = 0; // R6
	stack[stack_len-16] = 0; // R5
	stack[stack_len-17] = 0; // R4

	num_tasks++;

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

	// Sin prioridades aún. Toma el contexto de la siguiente tarea.
	static int actual_task = -1;
	uint32_t next_sp;

	sp_vector[actual_task]=sp;
	next_sp=0;

	//incremento de actual_task para pasar a la siguiente
	actual_task++;
	actual_task=actual_task%num_tasks; // overflow control

	next_sp=sp_vector[actual_task];

	return next_sp;
}

int main(void)
{
	initHardware();

	init_task(task1, pila1, STACK_SIZE);
	init_task(task2, pila2, STACK_SIZE);

	start_os();

	while (1) {
	}
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
