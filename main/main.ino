/**
 * 1 - инициализация мигание всеми фонарями
 * 2 - зелёный - состояние ветки зелёное
 * 3 - красный - состояние ветки красное
 * 4 - мигающий красный - ошибка с серверка
 */
#include <SoftwareSerial.h>
#include <TimerOne.h>
#include "lumen.h"
#define ESP_PIN_TX 7

SoftwareSerial esp(ESP_PIN_TX, 0);

//status:<SUCCESS/FAILURE/ERROR> - list builds with the specified status only
#define G1 5
#define G2 3
#define G3 4
#define G4 6
#define R1 8
#define R2 9
#define R3 11
#define R4 10

#define COMMAND_INIT "init"
#define COMMAND_WAIT_CONNECT "wait connect"
#define COMMAND_CONNECT "connect"
#define COMMAND_SERVER_FAILED "server-failed"

#define COMMAND_TM_ANSWER_SUCCESS "tm-ERROR"
#define COMMAND_TM_ANSWER_SUCCESS "tm-ERROR-*"

#define COMMAND_TM_ANSWER_SUCCESS "tm-SUCCESS-finished"
#define COMMAND_TM_ANSWER_SUCCESS_RUNNING "tm-SUCCESS-running"

#define COMMAND_TM_ANSWER_FAILURE "tm-FAILURE-finished"
#define COMMAND_TM_ANSWER_FAILURE_RUNNING "tm-FAILURE-running"

#define COMMAND_TM_ANSWER_NONE "tm-none"

#define COMMAND_UNDEFINED "undefined"


#define SERIAL_SPEED 9600 

#define SIZE 4
Lumen green[] = {Lumen(G1), Lumen(G2), Lumen(G3), Lumen(G4)};
Lumen red[] = {Lumen(R1), Lumen(R2), Lumen(R3), Lumen(R4)};

String parse(const String name, const String json);

String cmd = COMMAND_INIT;
String newCmd = cmd;
void setup()
{
  for (int i = 0; i < SIZE; i++) {
    green[i].attach();
    red[i].attach();
  }

  Serial.begin(SERIAL_SPEED);
  esp.begin(SERIAL_SPEED);
  Serial.println("Init Traffic-light v0.1");
  Timer1.initialize((500000));         // initialize timer1, and set a 1/2 second period
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
  esp.flush();
}

int i = 0;
void onUpdateCmd() {
  
}

void callback() {
  if (i > 4) {
    if (cmd != newCmd) onUpdateCmd();
    cmd = newCmd;
    i = 0;
  };
  
  if (cmd == COMMAND_INIT) {
    topAndDown(i);
  } else if (cmd == COMMAND_WAIT_CONNECT) {
    topAndDown(i);
  } else if (cmd == COMMAND_CONNECT) {
    for (int k = 0; k < 4; k++) {
      red[k].on();
      green[k].on();
    }
  } else if (cmd == COMMAND_SERVER_FAILED) {
    blinkRed(i);
  } else if (cmd == COMMAND_UNDEFINED) {
    blinkRed(i);
  } else if (cmd == COMMAND_TM_ANSWER_SUCCESS) {
    enadledGreen();
  } else if (cmd == COMMAND_TM_ANSWER_SUCCESS_RUNNING) {
    enadledGreen();
  } else if (cmd == COMMAND_TM_ANSWER_FAILURE) {
    enadledRed();
  } else if (cmd == COMMAND_TM_ANSWER_FAILURE_RUNNING) {
    enadledRed();
  } else if (cmd == COMMAND_TM_ANSWER_NONE) {
    blinkRed(i);
  } else {
    blinkRed(i);
  }

  i++;
}

void loop() {
    if (esp.available() > 0) {
      String json = esp.readString();
      json.trim();
      newCmd = parse("status", json);
      Serial.println(cmd);
    }

  delay(300);
}

void enadledGreen() {
  for (int k = 0; k < 4; k++) {
      red[k].off();
      green[k].on();
    }
}


void enadledRed() {
  for (int k = 0; k < 4; k++) {
      red[k].on();
      green[k].off();
    }
}

void disabled() {
  for (int k = 0; k < 4; k++) {
    red[k].off();
    green[k].off();
  }
}
void blinkGreen(int i) {
  if (i % 2 == 0) {
    for (int k = 0; k < 4; k++) {
      red[k].off();
      green[k].on();
    }
  } else {
    for (int k = 0; k < 4; k++) {
      red[k].off();
      green[k].tick();
    }
  }
}


void blinkRed(int i) {
  if (i % 2 == 0) {
    for (int k = 0; k < 4; k++) {
      red[k].on();
      green[k].off();
    }
  } else {
    for (int k = 0; k < 4; k++) {
      red[k].tick();
      green[k].off();
    }
  }
}

void topAndDown(int i) {
  if (i % 2 == 0) {
    for (int k = 0; k < 4; k++) {
      red[k].off();
      green[k].on();
    }
  } else {
    for (int k = 0; k < 4; k++) {
      red[k].tick();
      green[k].off();
    }
  }
}


String parse(const String name, const String json) {
  byte start = json.indexOf("{\"" + name + "\": \"");
  if (start == -1 || start == 255) {
    return COMMAND_UNDEFINED;
  }

  return json.substring(start + 6 + name.length(), json.indexOf("\"}", start));
}
