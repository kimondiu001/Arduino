//........... @SOLOMON KIMONDIU ARTS..............//
//............DC MOTOR SPEED CONTROL..............//



#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>

LiquidCrystal_PCF8574 lcd(0x27); // set the LCD address to 0x27 for a 16 chars and 2 line display
int show = -1;


int in1 = 11;
int in2 = 10;
int ena1 = 9;
int in3 = 6;
int in4 = 7;
int ena2 = 8;

const int clockwise = 3;
const int anticlockwise = 4;
int MotorSpeed = 0;
int SpeedControl = A0;
int clockwisestate = 0;
int anticlockwisestate = 0;

int voltagepin = A2;
float Load_Voltage = 0;
int readval;
int offset = 25;

double Vout = 0;
double Current = 0;
const double scale_factor = 0.185; // 5A
//const double scale_factor = 0.1; // 20A
//const double scale_factor = 0.066; // 30A
const double vRef = 4.50;
const double resConvert = 1023;
double resADC = vRef/resConvert;
double zeroPoint = vRef/2;

float value = 0;
float rev= 0;
int rpm;
int oldtime=0;
int time;

void isr()
{
  rev++;
  }

void setup() {
  // put your setup code here, to run once:
pinMode(in1,OUTPUT);
pinMode(in2,OUTPUT);
pinMode(ena1,OUTPUT);
pinMode(ena2,OUTPUT);
Serial.begin(9600);
pinMode(clockwise,INPUT_PULLUP);
pinMode(anticlockwise,INPUT_PULLUP);
pinMode(voltagepin,INPUT);
attachInterrupt(digitalPinToInterrupt(2),isr,RISING);
Serial.begin(9600);
Wire.begin();
Wire.beginTransmission(0x27);
show = 0;
lcd.begin(20, 4);
}


void(* resetFunc)(void)=0;

void loop(){
  // put your main code here, to run repeatedly:

MotorSpeed = analogRead(SpeedControl);
Serial.println(MotorSpeed);
delay(100);
MotorSpeed = map(MotorSpeed, 0, 1023, 0, 255);
analogWrite(ena1,MotorSpeed);
analogWrite(ena2,MotorSpeed);
digitalWrite(in1,HIGH);
digitalWrite(in2,LOW);
digitalWrite(in3,LOW);
digitalWrite(in4,HIGH);
{
int volt = analogRead(voltagepin);
Serial.println(volt);
double Load_Voltage = map(volt, 0, 1023, 0, 1220) + offset;
Load_Voltage/=100;
delay(100);
Serial.print("Voltage: ");
Serial.print(Load_Voltage);
Serial.println("V");
lcd.setBacklight(1);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("VOLTAGE:");
lcd.setCursor(9, 0);
lcd.print(Load_Voltage);
}

{
// code for the tachometer to measure the speed of the motor.
delay(1000);
detachInterrupt(0);
//rev= 0;
time=millis()-oldtime;
//int wings=1;
//int RPMnew = rev/wings;
rpm = (rev/time)*60000*1;
oldtime=millis();

rev= 0;
lcd.setCursor(0,2);
lcd.print("SPEED: ");
lcd.print(rpm);
lcd.print(" RPM");
delay(100);
attachInterrupt(digitalPinToInterrupt(2),isr,RISING);
}

  // Vout is read 1000 Times for precision
  Vout = 0;
  for(int i = 0; i < 1000; i++) {
    Vout = (Vout + (resADC * analogRead(A3)));   
//    delay(1);
  }
  
  Vout = Vout /1000;
  // Convert Vout into Current using Scale Factor
  Current = (Vout - zeroPoint)/ (-scale_factor) - .35;                                    
  lcd.setCursor(0, 1);
  lcd.print("CURRENT: ");
  lcd.print(Current);
  lcd.print(" Amps");
  delay(100);
  Serial.print("Vout = ");           
  Serial.print(Vout,2); 
  Serial.print(" Volts");                            
  Serial.print("\t Current = ");                  
  Serial.print(Current,2);
  Serial.println(" Amps");                             
//  delay(100); 
 
}
