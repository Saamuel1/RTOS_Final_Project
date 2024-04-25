#include <stdio.h>
#include <pthread.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h"

/* Priorities at which the tasks are created. */
#define mainTASK1_PRIORITY    ( tskIDLE_PRIORITY + 4 )
#define mainTASK2_PRIORITY    ( tskIDLE_PRIORITY + 3 )
#define mainTASK3_PRIORITY    ( tskIDLE_PRIORITY + 2 )
#define mainTASK4_PRIORITY    ( tskIDLE_PRIORITY + 1 )
#define mainTASK5_PRIORITY    ( tskIDLE_PRIORITY )


/* The rate at which data is sent to the queue.  The times are converted from
 * milliseconds to ticks using the pdMS_TO_TICKS() macro. */
#define mainTASK_SEND_FREQUENCY_MS         pdMS_TO_TICKS( 200UL )
#define mainTIMER_SEND_FREQUENCY_MS        pdMS_TO_TICKS( 2000UL )

/* The number of items the queue can hold at once. */
#define mainQUEUE_LENGTH                   ( 2 )

/* The values sent to the queue receive task from the queue send task and the
 * queue send software timer respectively. */
#define mainVALUE_SENT_FROM_TASK           ( 100UL )
#define mainVALUE_SENT_FROM_TIMER          ( 200UL )

/*-----------------------------------------------------------*/

/*
 * The tasks as described in the comments at the top of this file.
 */
static void Task1( void * pvParameters );
static void Task2( void * pvParameters );
static void Task3( void * pvParameters );
static void Task4( void * pvParameters );
static void Task5( void * pvParameters );



/*
 * The callback function executed when the software timer expires.
 */
static void prvQueueSendTimerCallback( TimerHandle_t xTimerHandle );

/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

/* A software timer that is started from the tick hook. */
static TimerHandle_t xTimer = NULL;

/*-----------------------------------------------------------*/

/*** SEE THE COMMENTS AT THE TOP OF THIS FILE ***/
void ipsa_sched( void )
{
    const TickType_t xTimerPeriod = mainTIMER_SEND_FREQUENCY_MS;

    /* Create the queue. */
    xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( uint32_t ) );

    if( xQueue != NULL )
    {
        /* Start the two tasks as described in the comments at the top of this
         * file. */
        xTaskCreate( Task1, "AX",configMINIMAL_STACK_SIZE, NULL, mainTASK1_PRIORITY , NULL );                          
        xTaskCreate( Task2, "BX", configMINIMAL_STACK_SIZE, NULL, mainTASK2_PRIORITY, NULL );
        xTaskCreate( Task3, "CX", configMINIMAL_STACK_SIZE, NULL, mainTASK3_PRIORITY, NULL );
        xTaskCreate( Task4, "DX", configMINIMAL_STACK_SIZE, NULL, mainTASK4_PRIORITY, NULL );
        xTaskCreate( Task5, "EX", configMINIMAL_STACK_SIZE, NULL, mainTASK5_PRIORITY, NULL );


        /* Create the software timer, but don't start it yet. */
        xTimer = xTimerCreate( "Timer", xTimerPeriod, pdTrue, NULL, prvQueueSendTimerCallback ); 

        if( xTimer != NULL )
        {
            xTimerStart( xTimer, 0 );
        }

        /* Start the tasks and timer running. */
        vTaskStartScheduler();
    }

    /* If all is well, the scheduler will now be running, and the following
     * line will never be reached.  If the following line does execute, then
     * there was insufficient FreeRTOS heap memory available for the idle and/or
     * timer tasks	to be created.  See the memory management section on the
     * FreeRTOS web site for more details. */
    for( ; ; )
    {
    }
}
/*-----------------------------------------------------------*/

static void Task1(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(500); // 500 ms
    for (;;) {
        printf("Working...\n");
        endTick = xTaskGetTickCount();
        executionTime = endTick - startTick;
        if (executionTime > maxExecutionTime) {
            maxExecutionTime = executionTime;
        }
 
        // Compter jusqu'à 500 et ensuite afficher le WCET
        count++;
        if (count >= 500) {
            printf("Worst Case Execution Time: %u ticks.\n", maxExecutionTime);
            maxExecutionTime = 0; // Réinitialiser pour la prochaine mesure
            count = 0; // Réinitialiser le compteur
        }
 
        vTaskDelayUntil(&xLastWakeTime, xDelay);
    }
}
 
static void vTask2_ConvertTemperature(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xDelay = pdMS_TO_TICKS(1000); // 1 seconde
    TickType_t startTick, endTick;
    uint32_t executionTime, maxExecutionTime = 0;
    uint32_t count = 0;
 
    for (;;) {
        startTick = xTaskGetTickCount();
 
        // Code de la tâche
        float fahrenheit = 68.0;
        float celsius = (fahrenheit - 32) * 5 / 9;
        printf("Temperature: %.2f F = %.2f C\n", fahrenheit, celsius);
 
        endTick = xTaskGetTickCount();
        executionTime = endTick - startTick;
        if (executionTime > maxExecutionTime) {
            maxExecutionTime = executionTime;
        }
 
        count++;
        if (count >= 500) {
            printf("Task 2 WCET: %u ticks.\n", maxExecutionTime);
            maxExecutionTime = 0;
            count = 0;
        }
 
        vTaskDelayUntil(&xLastWakeTime, xDelay);
    }
}
 
 
static void vTask3_MultiplyBigNumbers(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xDelay = pdMS_TO_TICKS(1500); // 1.5 secondes
    TickType_t startTick, endTick;
    uint32_t executionTime, maxExecutionTime = 0;
    uint32_t count = 0;
 
    for (;;) {
        startTick = xTaskGetTickCount();
 
        // Code de la tâche
        long int num1 = 123456789;
        long int num2 = 987654321;
        long int result = num1 * num2;
        printf("Multiplication: %ld * %ld = %ld\n", num1, num2, result);
 
        endTick = xTaskGetTickCount();
        executionTime = endTick - startTick;
        if (executionTime > maxExecutionTime) {
            maxExecutionTime = executionTime;
        }
 
        count++;
        if (count >= 500) {
            printf("Task 3 WCET: %u ticks.\n", maxExecutionTime);
            maxExecutionTime = 0;
            count = 0;
        }
 
        vTaskDelayUntil(&xLastWakeTime, xDelay);
    }
}
 
 
static void vTask4_BinarySearch(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xDelay = pdMS_TO_TICKS(2000); // 2 secondes
    TickType_t startTick, endTick;
    uint32_t executionTime, maxExecutionTime = 0;
    uint32_t count = 0;
 
    for (;;) {
        startTick = xTaskGetTickCount();
 
        // Code de la tâche
        int result = binarySearch(array, 0, ARRAY_SIZE - 1, target);
        if (result != -1)
            printf("Element found at index: %d\n", result);
        else
            printf("Element not found\n");
 
        endTick = xTaskGetTickCount();
        executionTime = endTick - startTick;
        if (executionTime > maxExecutionTime) {
            maxExecutionTime = executionTime;
        }
 
        count++;
        if (count >= 500) {
            printf("Task 4 WCET: %u ticks.\n", maxExecutionTime);
            maxExecutionTime = 0;
            count = 0;
        }
 
        vTaskDelayUntil(&xLastWakeTime, xDelay);
    }
}
 
 
static void vTask5_Aperiodic(void *pvParameters) {
    TickType_t startTick, endTick;
 
    for (;;) {
        startTick = xTaskGetTickCount();
 
        // Simuler le travail
        vTaskDelay(pdMS_TO_TICKS(100)); // Simuler le travail
 
        endTick = xTaskGetTickCount();
        printf("Execution Time of Task 5: %u ticks.\n", (unsigned)(endTick - startTick));
 
        // Suspendre jusqu'à l'événement suivant
    }
}
 
int binarySearch(int arr[], int l, int r, int x) {
    while (l <= r) {
        int m = l + (r - l) / 2;
 
        if (arr[m] == x) {
            return m;
        }
 
        if (arr[m] < x) {
            l = m + 1;
        } else {
            r = m - 1;
        }
    }
 
    return -1;
}
 