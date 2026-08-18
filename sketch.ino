#define LED_PIN 2
#define BUTTON_PIN 4
#define BUTTON_EVENT (1 << 0)
QueueHandle_t sensorQueue;
SemaphoreHandle_t serialMutex;
EventGroupHandle_t systemEvents;
// ---------------- LED TASK ----------------
void ledTask(void *parameter)
{
  while (1)
  {
    digitalWrite(LED_PIN, HIGH);

  if (xSemaphoreTake(serialMutex, portMAX_DELAY))
  {
    Serial.println("[LED TASK] LED ON");

    xSemaphoreGive(serialMutex);
  }

    vTaskDelay(pdMS_TO_TICKS(1000));

    digitalWrite(LED_PIN, LOW);

  if (xSemaphoreTake(serialMutex, portMAX_DELAY))
  {
    Serial.println("[LED TASK] LED OFF");

    xSemaphoreGive(serialMutex);
  }

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// ---------------- UART TASK ----------------
void uartTask(void *parameter)
{
  while (1)
  {
    if (xSemaphoreTake(serialMutex, portMAX_DELAY))
    {
      Serial.println("[UART TASK] FreeRTOS system running");

      xSemaphoreGive(serialMutex);
    }

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
// ---------------- SENSOR TASK ----------------
void sensorTask(void *parameter)
{
  int sensorValue;

  while (1)
  {
    sensorValue = random(0, 4096);
    xQueueSend(sensorQueue, &sensorValue, portMAX_DELAY);

  if (xSemaphoreTake(serialMutex, portMAX_DELAY))
{
  Serial.print("[SENSOR TASK] Sensor = ");
  Serial.println(sensorValue);

  xSemaphoreGive(serialMutex);
}

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
// ---------------- MONITOR TASK ----------------
void monitorTask(void *parameter)
{
  int receivedValue;

  while (1)
  {
    if (xQueueReceive(sensorQueue, &receivedValue, portMAX_DELAY))
    {
      if (xSemaphoreTake(serialMutex, portMAX_DELAY))
{
  Serial.print("[MONITOR TASK] Received = ");
  Serial.println(receivedValue);

  xSemaphoreGive(serialMutex);
  }
    }
  }
}
// ---------------- BUTTON TASK ----------------
void buttonTask(void *parameter)
{
  while (1)
  {
    if (digitalRead(BUTTON_PIN) == LOW)
    {
      if (xSemaphoreTake(serialMutex, portMAX_DELAY))
      {
        Serial.println("[BUTTON TASK] Button PRESSED");
        xEventGroupSetBits(systemEvents, BUTTON_EVENT);

        xSemaphoreGive(serialMutex);
      }

      vTaskDelay(pdMS_TO_TICKS(500));
    }

    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
// ---------------- EVENT TASK ----------------
void eventTask(void *parameter)
{
  while (1)
  {
    EventBits_t events = xEventGroupWaitBits(
      systemEvents,
      BUTTON_EVENT,
      pdTRUE,
      pdFALSE,
      portMAX_DELAY
    );

    if (events & BUTTON_EVENT)
    {
      if (xSemaphoreTake(serialMutex, portMAX_DELAY))
      {
        Serial.println("[EVENT TASK] Button event received");

        xSemaphoreGive(serialMutex);
      }
    }
  }
}
// ---------------- SYSTEM STATUS TASK ----------------
void systemStatusTask(void *parameter)
{
  while (1)
  {
    if (xSemaphoreTake(serialMutex, portMAX_DELAY))
    {
      Serial.println("[SYSTEM TASK] System Status: RUNNING");
      Serial.println("[SYSTEM TASK] FreeRTOS multitasking active");

      xSemaphoreGive(serialMutex);
    }

    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

// ---------------- SETUP ----------------
void setup()
{
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Create Event Group
  systemEvents = xEventGroupCreate();

  if (systemEvents == NULL)
  {
    Serial.println("Event group creation FAILED");
    while (1);
  }

  Serial.println("Event group created successfully");

  // Create Mutex
  serialMutex = xSemaphoreCreateMutex();

  if (serialMutex == NULL)
  {
    Serial.println("Mutex creation FAILED");
    while (1);
  }

  Serial.println("Mutex created successfully");

  // Create Queue
  sensorQueue = xQueueCreate(5, sizeof(int));

  if (sensorQueue == NULL)
  {
    Serial.println("Queue creation FAILED");
    while (1);
  }

  Serial.println("Queue created successfully");

  Serial.println("================================");
  Serial.println(" FREE RTOS MULTITASKING SYSTEM");
  Serial.println("================================");

  // Create LED task
  xTaskCreate(
    ledTask,
    "LED Task",
    2048,
    NULL,
    1,
    NULL
  );
  // Create Sensor task
  xTaskCreate(
    sensorTask,
    "Sensor Task",
    2048,
    NULL,
    2,
    NULL
  );
  // Create Monitor task
  xTaskCreate(
    monitorTask,
    "Monitor Task",
    2048,
    NULL,
    1,
    NULL
);
  // Create Button task
  xTaskCreate(
    buttonTask,
    "Button Task",
    2048,
    NULL,
    1,
    NULL
);
  // Create Event task
  xTaskCreate(
    eventTask,
    "Event Task",
    2048,
    NULL,
    1,
    NULL
);
  // Create System Status task
  xTaskCreate(
    systemStatusTask,
    "System Status Task",
    2048,
    NULL,
    1,
    NULL
);

  // Create UART task
  xTaskCreate(
    uartTask,
    "UART Task",
    2048,
    NULL,
    1,
    NULL
  );
}

// ---------------- LOOP ----------------
void loop()
{
}