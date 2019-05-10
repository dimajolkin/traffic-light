
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

class LumenFacade {
  protected:
    Lumen *_lumen;
    byte _cmd;
  public:
    static byte ON; 
    static byte OFF;
    static byte TICK;
    
    LumenFacade(Lumen *lumen, byte cmd) {
      _lumen = lumen;
      _cmd = cmd;
    }

    void exec() {
      if (_cmd == ON) {
        _lumen->on();
      } else if (_cmd == OFF) {
        _lumen->off();
      } else if (_cmd == TICK) {
        _lumen->tick();
      }
    }
};

byte LumenFacade::ON = 1;
byte LumenFacade::OFF = 2;
byte LumenFacade::TICK = 3;
