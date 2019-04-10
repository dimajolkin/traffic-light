
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

    void off() {
      if (status == 0) {
        return;
      }

      status = 0;
      digitalWrite(pin, LOW);    // выключаем светодиод
    }

};

class Traffic {
  protected:
    Lumen *_red;
    Lumen *_green;
    boolean status = false;
  public:
    Traffic(Lumen *red, Lumen *green): _red(red), _green(green) {}

    void setup() {
      _red->attach();
      _green->attach();
    }

    void red() {
      status = false;
      _red->on();
      _green->off();
    }

    void green() {
      status = true;
      _red->off();
      _green->on();
    }

    void blink() {
      if (status == false) {
        green();
      } else {
        red();
      }
    }

    void check() {
      delay(5000);
      Serial.println("Check RED.");
      _red->on();
      delay(2000);
      _red->off();
      delay(2000);

      Serial.println("Check Green.");
      _green->on();
      delay(2000);
      _green->off();
      delay(2000);

      Serial.println("Completion.");
      _red->on();
      _green->on();
      delay(2000);
      _red->off();
      _green->off();
      delay(2000);
    }
};
