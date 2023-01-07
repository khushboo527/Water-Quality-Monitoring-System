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

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

float phVal;

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // for ph Sensor

  if (phVal > 8.5 || phVal < 6.5)
    {
      if(phVal < 6.5){
        Blynk.logEvent("unsafe_", String("Water too acidic ,Ph Detected! : ") + phVal);
      }
      else{
        Blynk.logEvent("unsafe_", String("Water too basic ,Ph Detected!: ") + phVal);
      }
    }
  Blynk.virtualWrite(V2, phVal); //sending to Blynk
}



void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

float ph (float voltage) {

  // if(voltage>=3.96){
  //   //acidic region
  //   return 7 - ((voltage - 3.96)/1.04)*7;
  // }
  // else{
  //   //basic region
  //   return 7 + ((3.96-voltage)/3.96)*7;
  // }

  return 7 + ((2.5 - voltage) / 0.18) - 0.2;
}

int sensor_ph = A0;
int samples = 10;
float resolutions = 1024.0;

void loop() {

  int pHvalue = 0;

  for (int i = 0; i < samples; i++)
  {
      pHvalue += analogRead(sensor_ph);
      delay(10);
  }

  float voltage = 5 / resolutions * pHvalue/samples;
  Serial.print("\npH= ");
  phVal = ph(voltage);
  Serial.print(phVal);

  Blynk.run();
  timer.run();

  // put your main code here, to run repeatedly:
  
}
