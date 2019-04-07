
class Lumen {
  protected:
    int pin;
    int status = 0;
  public:
    Lumen(int pin): pin(pin) {}
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
    Lumen _red;
    Lumen _green;
    int status = 0;
  public:
    Traffic(Lumen red, Lumen green): _red(red), _green(green) {}

    void setup() {
      _red.attach();
      _green.attach();
    }

    void red() {
      status = 0;
      _red.on();
      _green.off();
    }

    void green() {
      status = 1;
      _red.off();
      _green.on();
    }

    void blink() {
      if (status == 0) {
        green();
      } else {
        red();
      }
    }

    void check() {
      delay(5000);
      Serial.println("Check RED.");
      _red.on();
      delay(2000);
      _red.off();
      delay(2000);

      Serial.println("Check Green.");
      _green.on();
      delay(2000);
      _green.off();
      delay(2000);

      Serial.println("Completion.");
      _red.on();
      _green.on();
      delay(2000);
      _red.off();
      _green.off();
      delay(2000);
    }
};
