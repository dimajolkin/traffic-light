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

#define SIZE 4
#define SERIAL_SPEED 9600

Lumen green[] = {Lumen(G1), Lumen(G2), Lumen(G3), Lumen(G4)};
Lumen red[] = {Lumen(R1), Lumen(R2), Lumen(R3), Lumen(R4)};
LumenFacade map_init[] = {
  LumenFacade(&green[0], LumenFacade::TICK),
  LumenFacade(&green[1], LumenFacade::TICK),
  LumenFacade(&green[2], LumenFacade::TICK),
  LumenFacade(&green[3], LumenFacade::TICK),
  LumenFacade(&red[0], LumenFacade::TICK),
  LumenFacade(&red[1], LumenFacade::TICK),
  LumenFacade(&red[2], LumenFacade::TICK),
  LumenFacade(&red[3], LumenFacade::TICK)
};

LumenFacade map_red[] = {
  LumenFacade(&red[0], LumenFacade::OFF),
  LumenFacade(&red[1], LumenFacade::OFF),
  LumenFacade(&red[2], LumenFacade::OFF),
  LumenFacade(&red[3], LumenFacade::OFF),
  LumenFacade(&red[0], LumenFacade::ON),
  LumenFacade(&red[1], LumenFacade::ON),
  LumenFacade(&red[2], LumenFacade::ON),
  LumenFacade(&red[3], LumenFacade::ON)
};

LumenFacade map_green[] = {
  LumenFacade(&green[0], LumenFacade::OFF),
  LumenFacade(&green[1], LumenFacade::OFF),
  LumenFacade(&green[2], LumenFacade::OFF),
  LumenFacade(&green[3], LumenFacade::OFF),
  LumenFacade(&green[0], LumenFacade::ON),
  LumenFacade(&green[1], LumenFacade::ON),
  LumenFacade(&green[2], LumenFacade::ON),
  LumenFacade(&green[3], LumenFacade::ON)
};


String parse(const String name, const String json);


String cmd = COMMAND_INIT;
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
void callback() {
  if (i > 7) i = 0;
  if (cmd == COMMAND_INIT) {
    map_init[i].exec();
  } else if (cmd == COMMAND_WAIT_CONNECT) {
    map_init[i].exec();
  } else if (cmd == COMMAND_CONNECT) {
    map_init[i].exec();
  } else if (cmd == COMMAND_SERVER_FAILED) {
    map_init[i].exec();
  } else if (cmd == COMMAND_UNDEFINED) {
    map_init[i].exec();
  } else if (cmd == COMMAND_TM_ANSWER_SUCCESS) {
    for (int k = 0; k < 4; k++) {
      red[k].off();
      green[k].on();
    }
  } else if (cmd == COMMAND_TM_ANSWER_SUCCESS_RUNNING) {
    blinkGreen(i);
  } else if (cmd == COMMAND_TM_ANSWER_FAILURE) {
    for (int k = 0; k < 4; k++) {
      red[k].on();
      green[k].off();
    }
  } else if (cmd == COMMAND_TM_ANSWER_FAILURE_RUNNING) {
    blinkRed(i);
  } else if (cmd == COMMAND_TM_ANSWER_NONE) {
    map_red[i].exec();
  } else {
    map_red[i].exec();
    map_green[i].exec();
  }

  i++;
}

void loop() {
    if (esp.available() > 0) {
      String json = esp.readString();
      json.trim();
      cmd = parse("status", json);
      Serial.println(cmd);
    }

  delay(300);
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
