#include <Arduino.h>
#define LED 2
#define DAC_PIN 25  // DAC pin for ESP32 (DAC1)

hw_timer_t *My_timer = NULL;

// Define the flag to toggle the LED
volatile bool ledState = false;
volatile int currentIndex = 0; // Index to keep track of the position in the array

const int seq[] =
    {66, 66, 66, 66, 66, 72, 78, 82, 76, 71, 70, 66, 66, 66, 66, 66, 30, 90, 132, 180, 255, 168, 72, 0, 31,
     66, 66, 66, 66, 66, 72, 78, 90, 86, 72, 66, 66, 66, 66, 66};

const int dataSize = sizeof(seq) / sizeof(seq[0]); // Size of the array

void IRAM_ATTR onTimer()
{
  ledState = !ledState; // Toggle the LED state
  digitalWrite(LED, ledState ? HIGH : LOW); // Update the LED

  // Print the current element in the array
  Serial.print("array value = ");
  Serial.println(seq[currentIndex]);


  
  // Convert the seq value to voltage (0-3.3V) for DAC
  float dacValue = (3.3 / 4095) * seq[currentIndex];

  // Write the value to the DAC pin
  dacWrite(DAC_PIN, dacValue);

  // //Print DAC value (reboots "Core  1 panic'ed (LoadProhibited). Exception was unhandled." )
  // Serial.print("dac value = ");
  // Serial.println(dacValue); 


  currentIndex = (currentIndex + 1) % dataSize; // Increment index and wrap around if necessary
}

void setup()
{
  Serial.begin(115200);
  // Initialize the LED pin as an output
  pinMode(LED, OUTPUT);
  
  // Create a periodic timer
  My_timer = timerBegin(0, 80, true); // Timer 0, divider 80, count up
  timerAttachInterrupt(My_timer, &onTimer, true); // Attach the ISR function
  timerAlarmWrite(My_timer, 1000000, true); // Set the alarm interval in microseconds
  timerAlarmEnable(My_timer); // Enable the timer alarm
}

void loop()
{
  delay(100);
}