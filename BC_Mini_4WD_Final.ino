// BC_Mini_4WD_Final
// created by mariofarrel

#include <Ps3Controller.h>
#include <Servo.h>

// setting
int maxAngle = 110;     // left turn
int centerAngle = 85;   // netral position
int minAngle = 78;      // right turn
int maxSpeed = 200;

// motor pin
int motor1Pin1 = 27; 
int motor1Pin2 = 26; 
int enable1Pin = 14;
int dir = 0;
int motorStop = 0;
// motor pwm properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 0;

// servo init
Servo myservo;
int pos = centerAngle;

void notify()
{
    //---------------- Analog stick value events ---------------
   if( abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) > 2 ){
       /*
       Serial.print("Moved the left stick:");
       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
       Serial.println();
       */
       
       pos = map(int(Ps3.data.analog.stick.lx), -118, 137, maxAngle, minAngle); 
       Serial.print("servo pos = "); Serial.print(pos);
       Serial.println();
   }
   if( abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) > 2 ){
       /*
       Serial.print("Moved the right stick:");
       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.rx, DEC);
       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ry, DEC);
       Serial.println();
       */

       if (int(Ps3.data.analog.stick.ry) < -2)
       {
          // motor forward
          Serial.print("motor forward: ");
          digitalWrite(motor1Pin1, LOW);
          digitalWrite(motor1Pin2, HIGH);
          dutyCycle = map(int(Ps3.data.analog.stick.ry), 0, -128, 0, maxSpeed);
          Serial.println(dutyCycle);
          ledcWrite(pwmChannel, dutyCycle);
          dir = 1;
          motorStop = 0;
       }

       if (int(Ps3.data.analog.stick.ry) > 2)
       {
          // motor backward
          Serial.print("motor backward: ");
          digitalWrite(motor1Pin1, HIGH);
          digitalWrite(motor1Pin2, LOW);
          dutyCycle = map(int(Ps3.data.analog.stick.ry), 0, 127, 0, maxSpeed);
          Serial.println(dutyCycle);
          ledcWrite(pwmChannel, dutyCycle);
          dir = 0;
          motorStop = 0;
       }
    }

    //---------------- Analog stick value events zero ---------------
   if( abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) < 2 ){
       // servo netral
       myservo.write(centerAngle);
   }
   if( abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) < 2 ){
       if ((abs(int(Ps3.data.analog.stick.ry)) < 2) && (motorStop == 0))
       {
          //motor stop
          Serial.println("motor stop");
          digitalWrite(motor1Pin1, LOW);
          digitalWrite(motor1Pin2, LOW);
          motorStop = 1;
       }
    }
}

void onConnect(){
    Serial.println("Connected.");
}

void setup()
{
    // ps3
    Serial.begin(115200);
    Ps3.attach(notify);
    Ps3.attachOnConnect(onConnect);
    Ps3.begin("02:00:00:00:00:00");
    Serial.println("Ready.");

    // motor
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(enable1Pin, OUTPUT);
    ledcSetup(pwmChannel, freq, resolution);
    ledcAttachPin(enable1Pin, pwmChannel);

    // servo
    myservo.attach(25, 2);  // pin 25, channel 1
}

void loop()
{
    // servo write
    myservo.write(pos);
    if(!Ps3.isConnected())
        return;
}
