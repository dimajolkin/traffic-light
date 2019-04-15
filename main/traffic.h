
class Lumen {
  protected:
    byte pin;
    byte status = 0;
  public:
    Lumen(byte pin): pin(pin) {}
    void attach() {
      pinMode(pin, OUTPUT);
      off();
    }

    void on() {
      if (status == 1) {
        return;
      }
      status = 1;
      digitalWrite(pin, HIGH);   // зажигаем светодиод
    }

    boolean isOn() {
      return status == 1;
    }

    void tick() {
      if (isOn()) {
        off();
      }  else {
        on();
      }
    }

    void off() {
      if (status == 0) {
        return;
      }

      status = 0;
      digitalWrite(pin, LOW);    // выключаем светодиод
    }

};

#define COMMAND_DISABLE "disable"
#define COMMAND_RED "red"
#define COMMAND_RED_BLINK "red-blink"
#define COMMAND_GREEN "green"
#define COMMAND_GREEN_BLINK "green-blink"
#define COMMAND_WAIT "wait"

class Traffic {
  protected:
    Lumen *lumenRed;
    Lumen *lumenGreen;
    String currentCommand = COMMAND_WAIT;
  public:
    Traffic(Lumen *red, Lumen *green): lumenRed(red), lumenGreen(green) {}

    String parse(const String name, const String json) {
      byte start = json.indexOf("{\"" + name + "\": \"");
      if (start == -1 || start == 255) {
        return COMMAND_WAIT;
      }

      return json.substring(start + 6 + name.length(), json.indexOf("\"}", start));
    }

    void setup() {
      lumenRed->attach();
      lumenGreen->attach();
    }

    void disabled() {
      lumenGreen->off();
      lumenGreen->off();
    }

    void red() {
      lumenRed->on();
      lumenGreen->off();
    }

    void green() {
      lumenRed->off();
      lumenGreen->on();
    }

    void redBlink() {
      lumenGreen->off();
      lumenRed->tick();
    }

    void greenBlink() {
      lumenRed->off();
      lumenGreen->tick();
    }

    void blink() {
      if (!lumenGreen->isOn()) {
        green();
      } else {
        red();
      }
    }

    void check() {
      delay(5000);
      Serial.println("Check RED.");
      lumenRed->on();
      delay(2000);
      lumenRed->off();
      delay(2000);

      Serial.println("Check Green.");
      lumenGreen->on();
      delay(2000);
      lumenGreen->off();
      delay(2000);

      Serial.println("Completion.");
      lumenRed->on();
      lumenGreen->on();
      delay(2000);
      lumenRed->off();
      lumenGreen->off();
      delay(2000);
    }

    String getCommand() {
      return currentCommand;
    }

    void setCommand(String command) {
      currentCommand = command;
    }

    void handler() {
      if (currentCommand == COMMAND_RED) {
        red();
      }

      if (currentCommand == COMMAND_RED_BLINK) {
        redBlink();
      }

      if (currentCommand == COMMAND_GREEN_BLINK) {
        greenBlink();
      }

      if (currentCommand == COMMAND_GREEN) {
        green();
      }

      if (currentCommand == COMMAND_WAIT) {
        blink();
      }
    }
};
