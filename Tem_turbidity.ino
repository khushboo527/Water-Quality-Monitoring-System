#define BLYNK_TEMPLATE_ID "TMPLN1pHRbur"
#define BLYNK_DEVICE_NAME "Temperature Sensor"
#define BLYNK_AUTH_TOKEN "vNrCHYmtssS7KJdiKxROgeoJo-9Yv0c3"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Redmi";
char pass[] = "";

// #define BLYNK_FIRMWARE_VERSION        "0.1.0"
// #define APP_DEBUG
#define USE_NODE_MCU_BOARD

BlynkTimer timer;
#include <DallasTemperature.h>
#include <OneWire.h>
#define ONE_WIRE_BUS D2                          //D2 pin of nodemcu

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);  

int sensorPin = A0;
float volt;
float NTU;
float PPM;

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

float round_to_dp( float in_value, int decimal_place)
{
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
}


// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // For Temperature Sensor

  sensors.requestTemperatures();                // Send the command to get temperatures  
  float RawValue = sensors.getTempCByIndex(0);
  if( RawValue == DEVICE_DISCONNECTED_C) 
  {
     Serial.println("Error: Could not read temperature data");
  } 
  else{
    float CorrectedValue = RawValue - 0.4; //had an offset of 0.3 degree celcius

    // Serial.println("Temperature is: ");
    // Serial.println(CorrectedValue);

    Blynk.virtualWrite(V0, CorrectedValue); //sending to Blynk
    Serial.println("\ntemp - ");
    Serial.println(CorrectedValue);

    if (CorrectedValue > 70)
    {
      Blynk.logEvent("unsafe_", String("High TemperatureDetected! Tº: ") + CorrectedValue);
    }
    if (PPM*3 >= 1){
      Blynk.logEvent("unsafe_", String("High Turbidity! : ") + PPM + String("ppm"));
    }
  }

  // for Turbudity Sensor
  Blynk.virtualWrite(V1, PPM); //sending to Blynk
}

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  int sensorvalue = analogRead(A0);  
  volt = sensorvalue * (5.0 / 1024.0);
  volt = round_to_dp(volt,1);

  if (volt >= 3.9)
  {
    NTU = 0;
  }
  else
  {
    // NTU = -1120.4*volt*volt+5742.3*volt-4352.9;
    // NTU = -832416.66667 * volt + 271666.66667 * volt*volt + 637542.00000;
    NTU = -166371.03096 * volt + 56829.45084 *  + 121548.80168
    if(NTU<0){
      NTU = 0;
    }
  }
  PPM = NTU/3;
  Serial.print(PPM);
  Serial.print(" ppm\n");

  Blynk.run();
  timer.run();

  // put your main code here, to run repeatedly:
  
}
