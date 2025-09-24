/*
Creation & Computation
FSR Sensor Workshop - Pressure Meter
Keuxan Zhuang - The loop light up in order and blink
*/

int sensor_pin = A0;        // Analog input pin for the pressure sensor
int first_led_pin = 2;      // Digital pin for the first LED (LEDs will use pins 2-7)
int min_pressure = 0;       // Minimum pressure value (no force applied)
int max_pressure = 1023;    // Maximum pressure value (adjust based on your sensor)
const int num_leds = 10;     // Number of LEDs used for the pressure meter display

int led_pins[num_leds];     // Array to store LED pin numbers


unsigned long previousmillis = 0;
const long blinkInterval = 200;
bool blinkState = false;
// Blink without dalay in Arduino example

int ledmode = 0;  
int currentled = 0;  
unsigned long ledstart = 0;  
const long sequence = 300;     
const long blinkduration = 3000;    // The interval of LED lighting in sequence and the duration of blinking

void setup()
{
  Serial.begin(9600);

  for (int i = 0; i < num_leds; i++)
  {
    led_pins[i] = first_led_pin + i;
    pinMode(led_pins[i], OUTPUT);
  }
}

void loop()
{
  int sensor_value = analogRead(sensor_pin);
  sensor_value = constrain(sensor_value, min_pressure, max_pressure);

  int leds_to_light = map(sensor_value, min_pressure, max_pressure, 0, num_leds);

  unsigned long currentmillis = millis();

  if (ledmode == 0)  
  {
    if (currentmillis - previousmillis >= sequence && currentled < leds_to_light)
    {
      previousmillis = currentmillis;
      digitalWrite(led_pins[currentled], HIGH);  // Light up in order
      currentled++;
    }

    // If the lighting is completed, then enter the blinking part
    if (currentled >= leds_to_light)
    {
      ledmode = 1;
      ledstart = currentmillis;
      previousmillis = currentmillis;
    }
  }
  else if (ledmode == 1)  // Blinking
  {
    if (currentmillis - previousmillis >= blinkInterval)
    {
      previousmillis = currentmillis;
      if (blinkState == true) 
      {
        blinkState = false;
      } else {
        blinkState = true;
             }
      for (int i = 0; i < leds_to_light; i++)
    {
      if (blinkState)
      {
        digitalWrite(led_pins[i], HIGH);
      }
      else
      {
        digitalWrite(led_pins[i], LOW);
      }
    }
    }

    // After blinking ends, continue to light up in order
    if (currentmillis - ledstart >= blinkduration)
    {
      ledmode = 0;
      currentled = 0;
      
      for (int i = 0; i < num_leds; i++)
      {
        digitalWrite(led_pins[i], LOW);
      }
    }
  }

  
  Serial.print("Sensor value: ");
  Serial.print(sensor_value);
  Serial.print(" | LEDs lit: ");
  Serial.println(leds_to_light);

  delay(10);
}
//It can work now