#include <PS2X_lib.h>

#define PS2_DAT 13
#define PS2_CMD 11
#define PS2_SEL 10
#define PS2_CLK 12

#define pwm1a 4
#define pwm1b 5
#define pwm2a 9
#define pwm2b 8
#define pwm3a 6
#define pwm3b 7
#define pwm4a 2
#define pwm4b 3

#define pressures false
#define rumble false

int forward;
int reverse;
int right;
int left;
int spinright;
int spinleft;
int error = 0;
byte type = 0;
byte vibrate = 0;

int maxSpeed = 90;

PS2X ps2x;

void setup()
{
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
    if (error == 0)
    {
        Serial.print("Found Controller, configured successful ");
        Serial.print("pressures = ");
        if (pressures)
            Serial.println("true ");
        else
            Serial.println("false");
        Serial.print("rumble = ");
        if (rumble)
            Serial.println("true)");
        else
            Serial.println("false");
        Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
        Serial.println("holding L1 or R1 will print out the analog stick values.");
        Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
    }
    else if (error == 1)
        Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

    else if (error == 2)
        Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

    else if (error == 3)
        Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

    type = ps2x.readType();
    switch (type)
    {
    case 0:
        Serial.print("Unknown Controller type found ");
        break;
    case 1:
        Serial.print("DualShock Controller found ");
        break;
    case 2:
        Serial.print("GuitarHero Controller found ");
        break;
    case 3:
        Serial.print("Wireless Sony DualShock Controller found ");
        break;
    }
}

void loop()
{
    forward = 0;
    reverse = 0;
    right = 0;
    left = 0;
    spinright = 0;
    spinleft = 0;

    if (error == 1) //skip loop
        return;

    if (type == 2)
    {
        ps2x.read_gamepad();
    }
    else
    {
        ps2x.read_gamepad(false, vibrate);

        //Move forward and reverse
        if (ps2x.Button(PSB_CROSS))
        {
            forward = 90;
        }
        if (ps2x.ButtonReleased(PSB_SQUARE))
        {
            reverse = 15;
        }

        if (ps2x.Button(PSB_SQUARE))
        {
            reverse = 90;
        }
        if (ps2x.ButtonReleased(PSB_SQUARE))
        {
            forward = 15;
        }

        //Move right and left
        if (ps2x.Analog(PSS_LX) > 128)
        {
            right = (ps2x.Analog(PSS_LX) - 128) * 2;
            if (right > maxSpeed)
                right = maxSpeed;
        }
        else if (ps2x.Analog(PSS_LX) < 128)
        {
            left = abs(ps2x.Analog(PSS_LX) - 128) * 2;
            if (left > maxSpeed)
                left = maxSpeed;
        }
        else
        {
            left = 0;
        }
    }

    //spinRight
    if (ps2x.Button(PSB_R1))
    {
        spinright = 50;
    }
    //spinLeft
    if (ps2x.Button(PSB_L1))
    {
        spinleft = 50;
    }

    Serial.print("Forward =  ");
    Serial.print(forward);
    Serial.print("\t");

    Serial.print("Reverse =  ");
    Serial.print(reverse);
    Serial.print("\t");

    Serial.print("Right =  ");
    Serial.print(right);
    Serial.print("\t");

    Serial.print("Left =  ");
    Serial.print(left);
    Serial.print("\t");

    Serial.print("Spin Right =  ");
    Serial.print(spinright);
    Serial.print("\t");

    Serial.print("Spin Left =  ");
    Serial.print(spinleft);
    Serial.print("\t");

    Serial.println("");

    if ((forward > 0) && reverse == 0 && right == 0 && left == 0 && spinright == 0 && spinleft == 0)
    {
        moveForward();
    }
    else if ((reverse > 0) && forward == 0 && right == 0 && left == 0 && spinright == 0 && spinleft == 0)
    {
        moveReverse();
    }
    else if ((right > 0) && forward == 0 && reverse == 0 && left == 0 && spinright == 0 && spinleft == 0)
    {
        moveRight();
    }
    else if ((left > 0) && forward == 0 && reverse == 0 && right == 0 && spinright == 0 && spinleft == 0)
    {
        moveLeft();
    }
    else if ((spinright > 0) && forward == 0 && reverse == 0 && right == 0 && left == 0 && spinleft == 0)
    {
        spinRight();
    }
    else if ((spinleft > 0) && forward == 0 && reverse == 0 && right == 0 && left == 0 && spinright == 0)
    {
        spinLeft();
    }
    else
    {
        stopMovement();
    }

} //end of loop

void moveForward()
{
    analogWrite(pwm2a, 0);
    analogWrite(pwm2b, forward);
    analogWrite(pwm4a, forward);
    analogWrite(pwm4b, 0);
}

void moveReverse()
{
    analogWrite(pwm2a, reverse);
    analogWrite(pwm2b, 0);
    analogWrite(pwm4a, 0);
    analogWrite(pwm4b, reverse);
}

void moveLeft()
{
    analogWrite(pwm1a, left);
    analogWrite(pwm1b, 0);
    analogWrite(pwm3a, left);
    analogWrite(pwm3b, 0);
}

void moveRight()
{
    analogWrite(pwm1a, 0);
    analogWrite(pwm1b, right);
    analogWrite(pwm3a, 0);
    analogWrite(pwm3b, right);
}

void spinLeft()
{
    analogWrite(pwm1a, spinleft);
    analogWrite(pwm1b, 0);
    analogWrite(pwm2a, spinleft);
    analogWrite(pwm3a, 0);
    analogWrite(pwm3b, spinleft);
    analogWrite(pwm2b, 0);
    analogWrite(pwm4a, spinleft);
    analogWrite(pwm4b, 0);
}

void spinRight()
{
    analogWrite(pwm1a, 0);
    analogWrite(pwm1b, spinright);
    analogWrite(pwm2a, 0);
    analogWrite(pwm3a, spinright);
    analogWrite(pwm3b, 0);
    analogWrite(pwm2b, spinright);
    analogWrite(pwm4a, 0);
    analogWrite(pwm4b, spinright);
}

void stopMovement()
{
    analogWrite(pwm1a, 0);
    analogWrite(pwm1b, 0);
    analogWrite(pwm2a, 0);
    analogWrite(pwm3a, 0);
    analogWrite(pwm3b, 0);
    analogWrite(pwm2b, 0);
    analogWrite(pwm4a, 0);
    analogWrite(pwm4b, 0);
}
