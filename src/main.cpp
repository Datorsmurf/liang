

#include <math.h>
#include "ArduinoOTA.h"
#include "WebSocketsServer.h"
#include "updatehandler.h"
#include "WiFi.h"
#include "WebServer.h"
#include "webui.h"
#include "SPIFFS.h"

#include "operational_modes/operationalmode.h"
#include "operational_modes/OpModeCharge.h"
#include "operational_modes/OpModeMow.h"
#include "operational_modes/OpModeIdle.h"

#include "behaviors/behavior.h"
#include "behaviors/charge.h"
#include "behaviors/followbwf.h"
#include "behaviors/goaround.h"
#include "behaviors/idle.h"
#include "behaviors/launch.h"
#include "behaviors/lookforbwf.h"
#include "behaviors/mow.h"



#include "Controller.h"
#include "definitions.h"
#include "mowermodel.h"
#include "sensor.h"
#include "secrets.h"
#include "battery.h"
#include "gyro.h"



int expectedMode = 0;
int expectedBehavior = 0;
int manualMode = -1;

MowerModel mowerModel;

void setManualMode(int manualMode_) {
  manualMode = manualMode_;
}

WebSocketsServer webSocket = WebSocketsServer(81);
LOGGER logger(&webSocket);
UPDATEHANDLER uh(&logger);
WebServer webServer(80);
WEBUI webUi(&webServer, &webSocket, &logger, *setManualMode);
GYRO gyro(&logger, MPU_INTERRUPT_PIN);
SENSOR leftSensor(LEFT_SENSOR_PIN, false);
SENSOR rightSensor(LEFT_SENSOR_PIN, false);
BATTERY battery(BATTERY_SENSOR_PIN, BATTERY_CHARGE_PIN);
BUMPER bumper(BUMPER_PIN);

MOTOR leftMotor(LEFT_MOTOR_PWM_PIN, LEFT_MOTOR_DIRECTION_PIN, LEFT_MOTOR_PWM_CHANNEL, &logger);
MOTOR rightMotor(RIGHT_MOTOR_PWM_PIN, RIGHT_MOTOR_DIRECTION_PIN, RIGHT_MOTOR_PWM_CHANNEL, &logger);
MOTOR cutterMotor(CUTTER_MOTOR_PWM_PIN, CUTTER_MOTOR_PWM_CHANNEL, &logger);

Controller controller(&leftMotor, &rightMotor, &cutterMotor, &gyro, &bumper);

OpModeCharge opModeCharge(&controller, &logger, &battery);
OpModeMow opModeMow(&controller, &logger, &battery);
OpModeIdle opModeIdle(&controller, &logger);

OPERATIONALMODE* availableOpModes[] = { 
  &opModeMow,
  &opModeCharge,
  &opModeIdle,
};
OPERATIONALMODE* currentMode = availableOpModes[0];


Charge charge(&controller, &logger, &battery);
FollowBWF followBWF(&controller, &logger, &battery);
GoAround goAround(&controller, &logger, &battery);
Idle idle(&controller, &logger, &battery);
Launch launch(&controller, &logger, &battery);
LookForBWF lookForBwf(&controller, &logger, &battery);
Mow mow(&controller, &logger, &battery);

BEHAVIOR* currentBehavior;
BEHAVIOR* availableBehaviors[] = {
  &charge,
  &followBWF,
  &goAround,
  &idle,
  &launch,
  &lookForBwf,
  &mow
};

int status;

float filter = 0.3;
float xAccOffset;
float deadZone = 0.02;


void handleInterruptLeft() {
  leftSensor.handleInterrupt();
}

void handleInterruptRight() {
  rightSensor.handleInterrupt();
}

void handleInterruptGyro() {
  gyro.dmpDataReady();
}


void setup() {
  Serial.begin(115200);

  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  Wire.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection Failed! Rebooting...");
      delay(5000);
      ESP.restart();
    }

  uh.setup();
  webUi.setup();
  gyro.setup();
  attachInterrupt(MPU_INTERRUPT_PIN, handleInterruptGyro, RISING);

  leftSensor.setup();
  attachInterrupt(LEFT_SENSOR_PIN, handleInterruptLeft, RISING);

  rightSensor.setup();
  attachInterrupt(RIGHT_SENSOR_PIN, handleInterruptRight, RISING);

  expectedMode = currentMode->id();
  expectedBehavior = currentMode->start();

  delay(1000);
}



unsigned long lastPrint= 0;



void loop() {



  //Handle
  gyro.loop();
  uh.handle();
  webUi.handle();
  webSocket.loop();

  if (manualMode >= 0) {
    expectedMode = manualMode;
    manualMode = -1;
  }

  if (expectedMode != currentMode->id()) {
    int c = sizeof(availableOpModes) / sizeof(availableOpModes[0]);
    bool foundIt = false;
    for (int i = 0; i < c; i++)
    {
      if (availableOpModes[i]->id() == expectedMode) {
        foundIt = true;
        currentMode = availableOpModes[i];
        currentMode->start();
        break;
      }
    }

    if (!foundIt) {
      logger.log("Could not locate OpMode with id:" + String(expectedMode), true);
    }
  }

  if (expectedBehavior != currentBehavior->id()) {
    int c = sizeof(availableBehaviors) / sizeof(availableBehaviors[0]);
    bool foundIt = false;
    for (int i = 0; i < c; i++)
    {
      if (availableBehaviors[i]->id() == expectedBehavior) {
        foundIt = true;
        currentBehavior = availableBehaviors[i];
        currentBehavior->start();
        break;
      }
    }

    if (!foundIt) {
      logger.log("Could not locate OpMode with id:" + String(expectedMode), true);
    }

  }


  expectedMode = currentMode->loop();
  expectedBehavior = currentBehavior->loop();


  

  if (millis()- lastPrint > 10000) {
    
    lastPrint = millis();
    logger.log("Log timing test...", false);
    logger.log("Log timing test...", false);
    logger.log("Log timing test...", false);
    logger.log("Log timing test...", false);
    logger.log("Time: " + String(trunc(millis() - lastPrint),0), false);
  }
}