#include <math.h>
#include "ArduinoOTA.h"
#include "updatehandler.h"
#include "WiFi.h"
#include "Wire.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include "MPU6050_light.h"

#include "operational_modes/operationalmode.h"
#include "operational_modes/OpModeCharge.h"
#include "operational_modes/OpModeMow.h"
#include "operational_modes/OpModeMowOnce.h"
#include "operational_modes/OpModeIdle.h"
#include "operational_modes/OpModeUpgrade.h"

#include "behaviors/behavior.h"
#include "behaviors/charge.h"
#include "behaviors/followbwf.h"
#include "behaviors/goaround.h"
#include "behaviors/idle.h"
#include "behaviors/launch.h"
#include "behaviors/lookforbwf.h"
#include "behaviors/mow.h"
#include "behaviors/sensordebug.h"

#include "Controller.h"
#include "webui.h"
#include "definitions.h"
#include "mowermodel.h"
#include "sensor.h"
#include "secrets.h"
#include "battery.h"
#include "gyro.h"
#include "display.h"


unsigned long lastPrint= 0;
int expectedMode = 0;
int expectedBehavior = 0;
int manualMode = -1;
bool updateInProgress = false;

TaskHandle_t pollTask;

MowerModel mowerModel;

void setManualMode(int manualMode_) {
  manualMode = manualMode_;
}

void updateEvent(int percentDone_) {
  updateInProgress = true;

  if (percentDone_ > 100) {
    //ESP.restart();
  }
}

MPU6050 mpu(Wire);
AsyncWebServer webServer(80);
AsyncWebSocket webSocket("/ws");
LOGGER logger(&webSocket, &mowerModel);
MOWERDISPLAY display;
WEBUI webUi(&webServer, &webSocket, &logger, *setManualMode);
GYRO gyro(&logger, &mpu);
SENSOR leftSensor(LEFT_SENSOR_PIN, false, &logger);
SENSOR rightSensor(RIGHT_SENSOR_PIN, false, &logger);
BATTERY battery(BATTERY_SENSOR_PIN, BATTERY_CHARGE_PIN);
BUMPER bumper(BUMPER_PIN);

MOTOR leftMotor(LEFT_MOTOR_SENSE_PIN, LEFT_MOTOR_FORWARD_PWM_PIN, LEFT_MOTOR_BACKWARDS_PWM_PIN, LEFT_MOTOR_PWM_CHANNEL_FORWARD, LEFT_MOTOR_PWM_CHANNEL_BACKWARDS, &logger);
MOTOR rightMotor(RIGHT_MOTOR_SENSE_PIN, RIGHT_MOTOR_FORWARD_PWM_PIN, RIGHT_MOTOR_BACKWARDS_PWM_PIN, RIGHT_MOTOR_PWM_CHANNEL_FORWARD, RIGHT_MOTOR_PWM_CHANNEL_BACKWARDS, &logger);
MOTOR cutterMotor(CUTTER_MOTOR_SENSE_PIN, CUTTER_MOTOR_FORWARD_PWM_PIN, CUTTER_MOTOR_BACKWARDS_PWM_PIN, CUTTER_MOTOR_PWM_CHANNEL_FORWARD, CUTTER_MOTOR_PWM_CHANNEL_BACKWARDS, &logger);

Controller controller(&leftMotor, &rightMotor, &cutterMotor, &gyro, &bumper);

UPDATEHANDLER uh(&logger, &controller);

OpModeCharge opModeCharge(&controller, &logger, &battery);
OpModeMow opModeMow(&controller, &logger, &battery);
OpModeMowOnce opModeMowOnce(&controller, &logger, &battery);
OpModeIdle opModeIdle(&controller, &logger);
OpModeUpgrade opModeUpgrade(&controller, &logger);

OPERATIONALMODE* availableOpModes[] = { 
  &opModeIdle,
  &opModeMow,
  &opModeMowOnce,
  &opModeCharge,
  &opModeUpgrade,
};
OPERATIONALMODE* currentMode = availableOpModes[0];


Charge charge(&controller, &logger, &battery);
FollowBWF followBWF(&controller, &logger, &battery);
GoAround goAround(&controller, &logger, &battery, &leftSensor, &rightSensor);
Idle idle(&controller, &logger, &battery);
SensorDebug sensorDebug(&controller, &logger, &battery, &leftSensor, &rightSensor);
Launch launch(&controller, &logger, &battery);
LookForBWF lookForBwf(&controller, &logger, &battery, *setManualMode, currentMode);
Mow mow(&controller, &logger, &battery);


BEHAVIOR* currentBehavior;
BEHAVIOR* availableBehaviors[] = {
  &charge,
  &followBWF,
  &goAround,
  &idle,
  &launch,
  &lookForBwf,
  &mow,
  &sensorDebug,
};

void handleInterruptLeft() {
  leftSensor.handleInterrupt();
}

void handleInterruptRight() {
  rightSensor.handleInterrupt();
}


void pollPollables(void * parameter) {
  //Wire must be begin():ed on the same core it's going to be used
  Wire.begin();

  display.setup();
  mowerModel.message = "BOOTING";
  gyro.setup();

  while (!updateInProgress)
  {
    if (millis() - lastPrint >= 200) {
      
      lastPrint = millis();
      display.DrawMowerModel(&mowerModel);
      
      
    }

    leftMotor.doLoop();
    mowerModel.LeftMotorLoad = leftMotor.getLoad();
    mowerModel.LeftMotorSpeed = leftMotor.getSpeed();

    rightMotor.doLoop();
    mowerModel.RightMotorLoad = rightMotor.getLoad();
    mowerModel.RightMotorSpeed = rightMotor.getSpeed();

    cutterMotor.doLoop();
    mowerModel.CutterMotorLoad = cutterMotor.getLoad();
    mowerModel.CutterMotorSpeed = cutterMotor.getSpeed();

    gyro.loop();
    mowerModel.Tilt = gyro.getTilt();
    mowerModel.Heading = gyro.getHeading();

    mowerModel.BatteryVoltage = battery.updateVoltage();

    mowerModel.LeftSensorIsOutOfBounds = leftSensor.IsOutOfBounds();
    mowerModel.RightSensorIsOutOfBounds = rightSensor.IsOutOfBounds();

    delay(2);
  }

  while (true)
  {
    if (millis() - lastPrint >= 200) {
      
      lastPrint = millis();
      display.DrawMowerModel(&mowerModel);
    }
  }

}


void setup() {
  Serial.begin(115200);

  xTaskCreatePinnedToCore(pollPollables, "pollTask", 8192, NULL, 5, &pollTask, 1);


  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }


  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection Failed! Rebooting...");
      mowerModel.message = "Connection Failed! Rebooting...";
      delay(5000);
      ESP.restart();
    }

  logger.log("IP: " + WiFi.localIP().toString(), true);
  delay(1000);

  uh.setup();
  webUi.setup();

  battery.setup();

  leftSensor.setup();
  attachInterrupt(LEFT_SENSOR_PIN, handleInterruptLeft, RISING);

  rightSensor.setup();
  attachInterrupt(RIGHT_SENSOR_PIN, handleInterruptRight, RISING);

  leftMotor.setup();
  rightMotor.setup();
  cutterMotor.setup();


  expectedMode = currentMode->id();
  expectedBehavior = currentMode->start();
  currentBehavior = availableBehaviors[0]; //Just something
  mowerModel.OpMode = currentMode->desc();
  mowerModel.Behavior = currentBehavior->desc();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SWITCH_3_PIN, INPUT_PULLDOWN);

}



void loop() {

  
  digitalWrite(LED_PIN, !digitalRead(RIGHT_SENSOR_PIN));
  //Handle
  uh.handle();
  webUi.handle();
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
        mowerModel.OpMode = currentMode->desc();
        expectedBehavior = currentMode->start();
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
        mowerModel.Behavior = currentBehavior->desc();
        currentBehavior->start();
        break;
      }
    }

    if (!foundIt) {
      logger.log("Could not locate behavior with id:" + String(expectedBehavior), true);
    }
  }

  expectedMode = currentMode->loop();
  expectedBehavior = currentBehavior->loop();

  
}