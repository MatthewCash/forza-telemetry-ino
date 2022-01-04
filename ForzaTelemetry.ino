#define TOTAL_LED_PINS 5
#define LED_PIN_1 2
#define LED_PIN_2 3
#define LED_PIN_3 4
#define LED_PIN_4 5
#define LED_PIN_5 6

#define SEG_PIN_TL 12
#define SEG_PIN_T 13
#define SEG_PIN_TR 7
#define SEG_PIN_M 11
#define SEG_PIN_BL 10
#define SEG_PIN_B 9
#define SEG_PIN_BR 8

const char SEG_PINS[] = {SEG_PIN_TL, SEG_PIN_T, SEG_PIN_TR, SEG_PIN_M, SEG_PIN_BL, SEG_PIN_B, SEG_PIN_BR};

const char SEG_CHAR_R[] = {0, 0, 0, 1, 1, 0, 0};
const char SEG_CHAR_N[] = {0, 0, 0, 1, 1, 0, 1};
const char SEG_CHAR_0[] = {1, 1, 1, 0, 1, 1, 1};
const char SEG_CHAR_1[] = {0, 0, 1, 0, 0, 0, 1};
const char SEG_CHAR_2[] = {0, 1, 1, 1, 1, 1, 0};
const char SEG_CHAR_3[] = {0, 1, 1, 1, 0, 1, 1};
const char SEG_CHAR_4[] = {1, 0, 1, 1, 0, 0, 1};
const char SEG_CHAR_5[] = {1, 1, 0, 1, 0, 1, 1};
const char SEG_CHAR_6[] = {1, 1, 0, 1, 1, 1, 1};
const char SEG_CHAR_7[] = {0, 1, 1, 0, 0, 0, 1};
const char SEG_CHAR_8[] = {1, 1, 1, 1, 1, 1, 1};
const char SEG_CHAR_9[] = {1, 1, 1, 1, 0, 1, 1};

const char *SEG_NUMS[] = {SEG_CHAR_0, SEG_CHAR_1, SEG_CHAR_2, SEG_CHAR_3, SEG_CHAR_4, SEG_CHAR_5, SEG_CHAR_6, SEG_CHAR_7, SEG_CHAR_8, SEG_CHAR_9};

unsigned int engine_max_rpm = 0;
unsigned int engine_current_rpm = 0;
unsigned char gear = 255;

inline char should_all_leds_flash()
{
    return engine_current_rpm > engine_max_rpm;
}

inline char should_led_light_up(const unsigned int led_pin)
{
    return engine_current_rpm >= (engine_max_rpm - (TOTAL_LED_PINS - led_pin + 1) * (engine_max_rpm / 20));
}

unsigned int flash_counter = 0;
#define FLASH_COUNTER_MAX 30000

void update_gear()
{
    char *seg_char;

    if (gear < 10)
        seg_char = SEG_NUMS[gear];

    if (gear == 10)
        seg_char = SEG_NUMS[0];

    if (gear == 0 && engine_current_rpm != 0)
        seg_char = SEG_CHAR_R;

    // Default value (no data)
    if (gear == 255)
        seg_char = SEG_CHAR_N;

    // Player is not driving
    if (gear == 0 && engine_current_rpm == 0)
        seg_char = SEG_CHAR_N;

    // Game reports neutral gear
    if (gear == 11)
        seg_char = SEG_CHAR_N;

    for (int i = 0; i < 7; i++)
    {
        digitalWrite(SEG_PINS[i], seg_char[i]);
    }
}

void update_rpms()
{
    if (!should_all_leds_flash())
    {
        digitalWrite(LED_PIN_1, should_led_light_up(1));
        digitalWrite(LED_PIN_2, should_led_light_up(2));
        digitalWrite(LED_PIN_3, should_led_light_up(3));
        digitalWrite(LED_PIN_4, should_led_light_up(4));
        digitalWrite(LED_PIN_5, should_led_light_up(5));
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.println("Serial Connected");

    pinMode(LED_PIN_1, OUTPUT);
    pinMode(LED_PIN_2, OUTPUT);
    pinMode(LED_PIN_3, OUTPUT);
    pinMode(LED_PIN_4, OUTPUT);
    pinMode(LED_PIN_5, OUTPUT);

    pinMode(SEG_PIN_TL, OUTPUT);
    pinMode(SEG_PIN_T, OUTPUT);
    pinMode(SEG_PIN_TR, OUTPUT);
    pinMode(SEG_PIN_M, OUTPUT);
    pinMode(SEG_PIN_BL, OUTPUT);
    pinMode(SEG_PIN_B, OUTPUT);
    pinMode(SEG_PIN_BR, OUTPUT);

    update_gear();
    update_rpms();
}

void loop()
{
    if (Serial.available() >= 7)
    {
        // Game data starts with two 0xff bytes
        if (Serial.read() == 0xff && Serial.read() == 0xff)
        {
            const unsigned int new_engine_max_rpm = (Serial.read() << 8) + Serial.read();
            if (new_engine_max_rpm != engine_max_rpm)
            {
                engine_max_rpm = new_engine_max_rpm;
                update_rpms();
            }

            const unsigned int new_engine_current_rpm = (Serial.read() << 8) + Serial.read();
            if (new_engine_current_rpm != engine_current_rpm)
            {
                engine_current_rpm = new_engine_current_rpm;
                update_rpms();
            }

            const unsigned char new_gear = Serial.read();
            if (new_gear != gear)
            {
                gear = Serial.read();
                update_gear();
            }
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
}