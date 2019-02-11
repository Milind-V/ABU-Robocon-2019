#include <PS2X_lib.h>

const byte MOTOR_A = 20;
const byte MOTOR_B = 18;
const float stepcount = 95.00; // 95 Slots in disk
const float wheeldiameter = 100; // Wheel diameter in millimeters

volatile int counter_A = 0;
volatile int counter_B = 0;

bool autoMode = false;

int relay1 = 36;
int pwm4a = 2;
int pwm4b = 3;

int pwm2a = 9;
int pwm2b = 8;

int pwm1a = 4;
int pwm1b = 5;

int pwm3a = 6;
int pwm3b = 7;

#define PS2_DAT 13
#define PS2_CMD 11
#define PS2_SEL 10
#define PS2_CLK 12

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

void ISR_countA()
{
    counter_A++;
}

void ISR_countB()
{
    counter_B++;
}

int CMtoSteps(float cm)
{

    int result;
    float circumference = (wheeldiameter * 3.14) / 10;
    float cm_step = circumference / stepcount;

    float f_result = cm / cm_step;
    result = (int)f_result;

    return result;
}

void MoveForward(int steps, int mspeed)
{
    counter_A = 0;
    counter_B = 0;

    // Set Motor A forward
    digitalWrite(pwm2a, LOW);
    digitalWrite(pwm2b, HIGH);
    digitalWrite(pwm4a, HIGH);
    digitalWrite(pwm4b, LOW);

    while (steps > counter_A)
    {

        if (steps > counter_A)
        {
            analogWrite(pwm2a, 0);
            analogWrite(pwm2b, mspeed);
            analogWrite(pwm4a, mspeed);
            analogWrite(pwm4b, 0);
        }
        else
        {
            analogWrite(pwm4a, 0);
            analogWrite(pwm4b, 0);
            analogWrite(pwm2a, 0);
            analogWrite(pwm2b, 0);
        }
    }
    analogWrite(pwm4a, 0);
    analogWrite(pwm4b, 0);
    analogWrite(pwm2a, 0);
    analogWrite(pwm2b, 0);
    counter_A = 0;
    counter_B = 0;
}

void MoveReverse(int steps, int mspeed)
{
    counter_A = 0;
    counter_B = 0;

    digitalWrite(pwm2a, HIGH);
    digitalWrite(pwm2b, LOW);
    digitalWrite(pwm4a, LOW);
    digitalWrite(pwm4b, HIGH);

    while (steps > counter_A)
    {

        if (steps > counter_A)
        {
            analogWrite(pwm2a, mspeed);
            analogWrite(pwm2b, 0);
            analogWrite(pwm4a, 0);
            analogWrite(pwm4b, mspeed);
        }
        else
        {
            analogWrite(pwm4a, 0);
            analogWrite(pwm4b, 0);
            analogWrite(pwm2a, 0);
            analogWrite(pwm2b, 0);
        }
    }
    analogWrite(pwm4a, 0);
    analogWrite(pwm4b, 0);
    analogWrite(pwm2a, 0);
    analogWrite(pwm2b, 0);
    counter_A = 0;
    counter_B = 0;
}

void MoveLeft(int steps, int mspeed)
{
    counter_A = 0;
    counter_B = 0;

    analogWrite(pwm1a, HIGH);
    analogWrite(pwm1b, LOW);
    analogWrite(pwm3a, HIGH);
    analogWrite(pwm3b, LOW);

    while (steps > counter_B)
    {

        if (steps > counter_B)
        {
            analogWrite(pwm1a, mspeed);
            analogWrite(pwm1b, 0);
            analogWrite(pwm3a, mspeed);
            analogWrite(pwm3b, 0);
        }
        else
        {
            analogWrite(pwm1a, 0);
            analogWrite(pwm1b, 0);
            analogWrite(pwm3a, 0);
            analogWrite(pwm3b, 0);
        }
    }
    analogWrite(pwm1a, 0);
    analogWrite(pwm1b, 0);
    analogWrite(pwm3a, 0);
    analogWrite(pwm3b, 0);
    counter_A = 0;
    counter_B = 0;
}

void setup()
{pinMode(relay1, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(MOTOR_A), ISR_countA, RISING);
    attachInterrupt(digitalPinToInterrupt(MOTOR_B), ISR_countB, RISING);

    // Test Motor Movement

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
    if (ps2x.ButtonReleased(PSB_TRIANGLE)) {
        digitalWrite(relay1, LOW);
  
    }
    if (ps2x.ButtonReleased(PSB_CIRCLE)) {
        digitalWrite(relay1, HIGH);
     
    }
    
        
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

        if (ps2x.ButtonPressed(PSB_SELECT))
        {
            autoMode = true;
        }

        if (ps2x.Button(PSB_CROSS) || ps2x.Button(PSB_PAD_UP))
        {
            forward = 90;
        }
        if (ps2x.ButtonReleased(PSB_SQUARE) || ps2x.ButtonReleased(PSB_PAD_UP))
        {
            reverse = 15;
        }

        if (ps2x.Button(PSB_SQUARE) || ps2x.Button(PSB_PAD_DOWN))
        {
            reverse = 90;
        }
        if (ps2x.ButtonReleased(PSB_SQUARE) || ps2x.ButtonReleased(PSB_PAD_DOWN))
        {
            forward = 15;
        }
        ///hmm
        if (ps2x.Button(PSB_PAD_RIGHT))
        {
            right = 90;
        }
        if (ps2x.ButtonReleased(PSB_PAD_RIGHT))
        {
            left = 15;
        }
        if (ps2x.Button(PSB_PAD_LEFT))
        {
            left = 90;
        }
        if (ps2x.ButtonReleased(PSB_PAD_LEFT))
        {
            right = 15;
        }

        // //Move right and left
        // if (ps2x.Analog(PSS_LX) > 128)
        // {
        //   right = (ps2x.Analog(PSS_LX) - 128) * 2;
        //   if (right > maxSpeed)
        //     right = maxSpeed;
        // }
        // else if (ps2x.Analog(PSS_LX) < 128)
        // {
        //   left = abs(ps2x.Analog(PSS_LX) - 128) * 2;
        //   if (left > maxSpeed)
        //     left = maxSpeed;
        // }
        // else
        // {
        //   left = 0;
        // }

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

        if (autoMode == false)
        {
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
                analogWrite(pwm1a, 0);
                analogWrite(pwm1b, 0);
                analogWrite(pwm2a, 0);
                analogWrite(pwm3a, 0);
                analogWrite(pwm3b, 0);
                analogWrite(pwm2b, 0);
                analogWrite(pwm4a, 0);
                analogWrite(pwm4b, 0);
            }
        }
        else
        {
            // MoveLeft(CMtoSteps(40), 80);
            autoMode = false;
            MoveForward(CMtoSteps(90), 80);
            delay(1000);

            // analogWrite(pwm4a, 0);
            // analogWrite(pwm4b, 0);
            // analogWrite(pwm2a, 0);
            // analogWrite(pwm2b, 0);

            // analogWrite(pwm1a, 0);
            // analogWrite(pwm1b, 0);
            // analogWrite(pwm3a, 0);
            // analogWrite(pwm3b, 0);

            counter_A = 0;
            counter_B = 0;
            //delay(1000);
        }
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