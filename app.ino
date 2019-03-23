#include <PS2X_lib.h>

bool autoMode = false;

#define relay1 36
#define relay2 38

#define pwm4a 3
#define pwm4b 2

#define pwm2a 9
#define pwm2b 8

#define pwm1a 5
#define pwm1b 4

#define pwm3a = 6
#define pwm3b = 7

#define PS2_DAT 30
#define PS2_CMD 32
#define PS2_SEL 33
#define PS2_CLK 31

#define maxSpeed 90

#define pressures false
#define rumble false

int error = 0;
byte type = 0;
byte vibrate = 0;

PS2X ps2x;

void motor(int p1a, int p1b, int p2a, int p2b, int p3a, int p3b, int p4a, int p4b){

    analogWrite(pwm1a, p1a);
    analogWrite(pwm1b, p1b);
    analogWrite(pwm2a, p2a);
    analogWrite(pwm2b, p2b);
    analogWrite(pwm3a, p3a);
    analogWrite(pwm3b, p3b);
    analogWrite(pwm4a, p4a);
    analogWrite(pwm4b, p4b);
}

void setup()
{
    pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);

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
    // if (error == 0)
    // {
    //     Serial.print("Found Controller, configured successful ");
    //     Serial.print("pressures = ");
    //     if (pressures)
    //         Serial.println("true ");
    //     else
    //         Serial.println("false");
    //     Serial.print("rumble = ");
    //     if (rumble)
    //         Serial.println("true)");
    //     else
    //         Serial.println("false");
    //     Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    //     Serial.println("holding L1 or R1 will print out the analog stick values.");
    //     Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
    // }
    // else if (error == 1)
    //     Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

    // else if (error == 2)
    //     Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

    // else if (error == 3)
    //     Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

    // type = ps2x.readType();
    // switch (type)
    // {
    // case 0:
    //     Serial.print("Unknown Controller type found ");
    //     break;
    // case 1:
    //     Serial.print("DualShock Controller found ");
    //     break;
    // case 2:
    //     Serial.print("GuitarHero Controller found ");
    //     break;
    // case 3:
    //     Serial.print("Wireless Sony DualShock Controller found ");
    //     break;
    // }
}

void loop()
{

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

    if (error == 1) //skip loop
        return;

    if (type == 2)
    {
        ps2x.read_gamepad();
    }else
    {
        ps2x.read_gamepad(false, vibrate);

        //Tombol SELECT
        if (ps2x.ButtonPressed(PSB_SELECT))
        {
            autoMode = true;
        }

        //MAJU
        if (ps2x.Button(PSB_CROSS) || ps2x.Button(PSB_PAD_UP))
        {
            motor(0,0,0,maxSpeed,0,0,maxSpeed,0);
        }

        //MUNDUR
        if (ps2x.Button(PSB_SQUARE) || ps2x.Button(PSB_PAD_DOWN))
        {
            motor(0,0,50,0,0,0,0,50);
        }

        ///KANAN
        if (ps2x.Button(PSB_PAD_RIGHT))
        {
            motor(maxSpeed,0,0,0,maxSpeed,0,0,0);
        }

        //KIRI
        if (ps2x.Button(PSB_PAD_LEFT))
        {
            motor(0,maxSpeed,0,0,0,maxSpeed,0,0);
        }

        //PUTAR KANAN
        if (ps2x.Button(PSB_R1))
        {
            motor(0,50,0,50,50,0,0,50);
        }

        //PUTAR KIRI
        if (ps2x.Button(PSB_L1))
        {
            motor(50,0,50,0,0,50,50,0);
        }

    }     
} //end of loop