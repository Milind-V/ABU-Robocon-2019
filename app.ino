#include <PS2X_lib.h>

#define relay1 36
#define relay2 38

#define pwm4a 5
#define pwm4b 4

#define pwm2a 9
#define pwm2b 8

#define pwm1a 11
#define pwm1b 10

#define pwm3a 6
#define pwm3b 7

#define PS2_DAT 30
#define PS2_CMD 32
#define PS2_SEL 33
#define PS2_CLK 31

#define maxSpeed 100
#define halfSpeed 50

#define pressures false
#define rumble false

int error = 0;
byte type = 0;
byte vibrate = 0;

PS2X ps2x;

void motor(int p1a, int p1b, int p2a, int p2b, int p3a, int p3b, int p4a, int p4b)
{

    analogWrite(pwm1a, p1a);
    analogWrite(pwm1b, p1b);
    analogWrite(pwm2a, p2a);
    analogWrite(pwm2b, p2b);
    analogWrite(pwm3a, p3a);
    analogWrite(pwm3b, p3b);
    analogWrite(pwm4a, p4a);
    analogWrite(pwm4b, p4b);

    if (p2b > 0 && p4a > 0)
    {
        Serial.print("Maju\n");
    }
    else if (p2a > 0 && p4b > 0)
    {
        Serial.print("Mundur\n");
    }
    else if (p1a > 0 && p3a > 0)
    {
        Serial.print("Kanan\n");
    }
    else if (p1b > 0 && p3b > 0)
    {
        Serial.print("Kiri\n");
    }
    else if (p1b > 0 && p2b > 0 && p3a > 0 && p4b > 0)
    {
        Serial.print("Putar Kanan\n");
    }
    else if (p1a > 0 && p2a > 0 && p3b > 0 && p4a > 0)
    {
        Serial.print("Putar Kiri\n");
    }
}

void setup()
{
    //Relay Pnumatik
    pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);

    //Pin Motor
    pinMode(pwm1a, OUTPUT);
    pinMode(pwm1b, OUTPUT);
    pinMode(pwm2a, OUTPUT);
    pinMode(pwm2b, OUTPUT);
    pinMode(pwm3a, OUTPUT);
    pinMode(pwm3b, OUTPUT);
    pinMode(pwm4a, OUTPUT);
    pinMode(pwm4b, OUTPUT);
    Serial.begin(57600);
    delay(300);
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
}

void loop()
{

    ps2x.read_gamepad();
    motor(0, 0, 0, 0, 0, 0, 0, 0);

    if (ps2x.ButtonPressed(PSB_TRIANGLE))
    {
        digitalWrite(relay1, LOW);
    }
    if (ps2x.ButtonPressed(PSB_L2))
    {
        digitalWrite(relay1, HIGH);
    }
    if (ps2x.ButtonPressed(PSB_CIRCLE))
    {
        digitalWrite(relay2, LOW);
    }
    if (ps2x.ButtonPressed(PSB_R2))
    {
        digitalWrite(relay2, HIGH);
    }

        if (ps2x.Button(PSB_CROSS) || ps2x.Button(PSB_PAD_UP))
    {
        motor(0, 0, 0, maxSpeed, 0, 0, maxSpeed, 0); //MAJU
    }
    if (ps2x.Button(PSB_SQUARE) || ps2x.Button(PSB_PAD_DOWN))
    {
        motor(0, 0, halfSpeed, 0, 0, 0, 0, halfSpeed); //MUNDUR
    }
    if (ps2x.Button(PSB_PAD_RIGHT))
    {
        motor(maxSpeed, 0, 0, 0, maxSpeed, 0, 0, 0); ///KANAN
    }
    if (ps2x.Button(PSB_PAD_LEFT))
    {
        motor(0, maxSpeed, 0, 0, 0, maxSpeed, 0, 0); //KIRI
    }
    if (ps2x.Button(PSB_L1))
    {
        motor(0, halfSpeed, 0, halfSpeed, halfSpeed, 0, 0, halfSpeed); //PUTAR KANAN
    }
    if (ps2x.Button(PSB_R1))
    {
        motor(halfSpeed, 0, halfSpeed, 0, 0, halfSpeed, halfSpeed, 0); //PUTAR KIRI
    }
}