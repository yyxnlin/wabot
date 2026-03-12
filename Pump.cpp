class Pump {
  private:
    int relayPin;

  public:
    Pump(int pin);
    void begin();
    void on();
    void off();
};