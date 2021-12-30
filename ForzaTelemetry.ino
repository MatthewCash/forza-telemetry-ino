#define TOTAL_LED_PINS 5
#define LED_PIN_1 2
#define LED_PIN_2 3
#define LED_PIN_3 4
#define LED_PIN_4 5
#define LED_PIN_5 6

void setup()
{
    Serial.begin(9600);
    Serial.println("Serial Connected");

    pinMode(LED_PIN_1, OUTPUT);
    pinMode(LED_PIN_2, OUTPUT);
    pinMode(LED_PIN_3, OUTPUT);
    pinMode(LED_PIN_4, OUTPUT);
    pinMode(LED_PIN_5, OUTPUT);
}

unsigned int engine_max_rpm = 0;
unsigned int engine_current_rpm = 0;
unsigned char gear = 0;

inline char should_all_leds_flash()
{
    return engine_current_rpm > engine_max_rpm;
}

inline char should_led_light_up(unsigned int led_pin)
{
    return engine_current_rpm >= (engine_max_rpm - (TOTAL_LED_PINS - led_pin + 1) * (engine_max_rpm / 20));
}

unsigned int flash_counter = 0;
#define FLASH_COUNTER_MAX 30000

void loop()
{
    if (Serial.available() >= 7)
    {
        // Game data starts with two 0xff bytes
        if (Serial.read() == 0xff && Serial.read() == 0xff)
        {
            engine_max_rpm = Serial.read() << 8 + Serial.read();
            engine_current_rpm = Serial.read() << 8 + Serial.read();
            gear = Serial.read();
        }
    }

    if (should_all_leds_flash())
    {
        flash_counter++;

        if (flash_counter > FLASH_COUNTER_MAX)
        {
            flash_counter = 0;

            digitalWrite(LED_PIN_1, !digitalRead(LED_PIN_1));
            digitalWrite(LED_PIN_2, !digitalRead(LED_PIN_2));
            digitalWrite(LED_PIN_3, !digitalRead(LED_PIN_3));
            digitalWrite(LED_PIN_4, !digitalRead(LED_PIN_4));
            digitalWrite(LED_PIN_5, !digitalRead(LED_PIN_5));
        }
    }
    else
    {
        digitalWrite(LED_PIN_1, should_led_light_up(1));
        digitalWrite(LED_PIN_2, should_led_light_up(2));
        digitalWrite(LED_PIN_3, should_led_light_up(3));
        digitalWrite(LED_PIN_4, should_led_light_up(4));
        digitalWrite(LED_PIN_5, should_led_light_up(5));
    }
}