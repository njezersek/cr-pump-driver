#define DEBOUNCE_DELAY 10

class DebounceInput{
private:
	bool last_reading = 0;
	unsigned long last_time = 0;
	uint8_t pin;
public:
	bool last_state = 0;
	bool state = 0;
	DebounceInput(uint8_t pin) : pin(pin) {
		
	}

	void setup(){
		pinMode(pin, INPUT_PULLUP);
	}

	void update(){
		last_state = state;
		bool state_tmp = !digitalRead(pin);
		unsigned long now = millis();
		if(last_reading != state_tmp){
			last_time = now;
		}
		if(now - last_reading > DEBOUNCE_DELAY){
			state = state_tmp;
		}
		last_reading = state_tmp;
	}
};