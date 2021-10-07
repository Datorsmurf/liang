#include "display.h"
#include "ssd1306.h"
#include "SPI.h"

MOWERDISPLAY::MOWERDISPLAY() {
    printedModel = new MowerModel();
    printedModel->Tilt = -1;
    printedModel->Heading = -1;
}
void MOWERDISPLAY::setup() {
    //my_mutex.lock();

  // Setup display
  /* Select the font to use with menu and all font functions */
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  ssd1306_128x64_i2c_init();  
  ssd1306_clearScreen();
  _cleared = true;
  //my_mutex.unlock();

//   ssd1306_setFixedFont(ssd1306xled_font6x8);
//   ssd1306_128x64_i2c_init();  
//   ssd1306_clearScreen();
}


void MOWERDISPLAY::print(String msg, int row) {
  if (blocked) return;
  char buffer[50];
  sprintf(buffer, "%-21s", msg.substring(0, 21).c_str());
  ssd1306_printFixed(0, rowHeight*row,  buffer, STYLE_NORMAL);
}


void MOWERDISPLAY::SendLog(LogEvent *e) {
  msg = e->msg;
}

void MOWERDISPLAY::PresentMowerModel(MowerModel* model, bool forceFullPresentation){

  if (blocked) return;

  if (_cleared || (printedModel->OpMode.compareTo(model->OpMode) != 0) || (printedModel->Behavior.compareTo(model->Behavior) != 0)) {
    print(model->OpMode + "/" + model->Behavior, 0);
    printedModel->OpMode = model->OpMode;
    printedModel->Behavior = model->Behavior;
  } 

  if (msg.length() > 0) {
    print(msg, 1);
    messageDisplayedAt = millis();
    msg = "";
  } else if (hasTimeout(messageDisplayedAt, 5000)) {
    print("", 1);
  }

  if (_cleared || printedModel->LeftSensorIsOutOfBounds != model->LeftSensorIsOutOfBounds || printedModel->RightSensorIsOutOfBounds != model->RightSensorIsOutOfBounds) {
      print("Sensors: " + String(model->LeftSensorIsOutOfBounds ? "OUT/" : " IN/") + String(model->RightSensorIsOutOfBounds ? "OUT" : "IN"), 2);
      printedModel->LeftSensorIsOutOfBounds = model->LeftSensorIsOutOfBounds;
      printedModel->RightSensorIsOutOfBounds = model->RightSensorIsOutOfBounds;
  }


  if (_cleared || printedModel->CutterMotorSpeed != model->CutterMotorSpeed || printedModel->CutterMotorLoad != model->CutterMotorLoad) {
      print("Cutter S:" + String(model->CutterMotorSpeed) + " L: " + String(model->CutterMotorLoad), 3);
      printedModel->CutterMotorSpeed = model->CutterMotorSpeed;
      printedModel->CutterMotorLoad = model->CutterMotorLoad;
  }

  if (_cleared || printedModel->LeftMotorSpeed != model->LeftMotorSpeed || printedModel->LeftMotorLoad != model->LeftMotorLoad) {
      print("Left   S:" + String(model->LeftMotorSpeed) + " L: " + String(model->LeftMotorLoad), 4);
      printedModel->LeftMotorSpeed = model->LeftMotorSpeed;
      printedModel->LeftMotorLoad = model->LeftMotorLoad;
  }

  if (_cleared || printedModel->RightMotorSpeed != model->RightMotorSpeed || printedModel->RightMotorLoad != model->RightMotorLoad) {
      print("Right  S:" + String(model->RightMotorSpeed) + " L: " + String(model->RightMotorLoad), 5);
      printedModel->RightMotorSpeed = model->RightMotorSpeed;
      printedModel->RightMotorLoad = model->RightMotorLoad;
  }


  if (_cleared || printedModel->Heading != model->Heading || printedModel->Tilt != model->Tilt) {
      print("H: " + String(model->Heading, 2) + " T: " + String(model->Tilt, 2), 6);
      printedModel->Heading = model->Heading;
      printedModel->Tilt = model->Tilt;
  }
  
  if (_cleared || printedModel->BatteryVoltage != model->BatteryVoltage || (printedModel->IsDocked != model->IsDocked)) {
    print("Batt:" + String(model->BatteryVoltage, 2)+ " Dock: " + (model->IsDocked ? "Y" : "N"), 7) ;
    printedModel->BatteryVoltage = model->BatteryVoltage;
    printedModel->IsDocked = model->IsDocked;
  }

  _cleared = false;
}