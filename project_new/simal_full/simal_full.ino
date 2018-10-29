#include <LiquidCrystal.h>
#include <Servo.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
Servo myServo;
LiquidCrystal lcd(6, 7, 5, 4, 3, 2);
int getFingerprintIDez();
SoftwareSerial mySerial(8,9);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int lock_status=0;
int enrolled = 0;
int id=30;
int valid_position()
{
  if(analogRead(1)<100)
  return 1;
  else
  return 0;
}
void setup()  
{
  //pinMode(10,INPUT);
  //digitalWrite(10,LOW);
  lcd.begin(16,2);
  myServo.attach(10);
  Serial.begin(9600);
//  Serial.println("fingertest");
  // set the data rate for the sensor serial port
  finger.begin(57600);
  if (finger.verifyPassword()) {
    //Serial.println("Found fingerprint sensor!");
  } else {
    //Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
  //Serial.println("Waiting for valid finger...");
}
void lcd_show()
{
  lcd.clear();
  lcd.setCursor(0,1);
}
void open_lock()
{
  myServo.write(180);
  lock_status=1;
  //Serial.println("opened");
  lcd_show(); lcd.print("door opened");
}
void close_lock()
{
  myServo.write(0);
  lock_status=0;
 // Serial.println("closed");
 lcd_show(); lcd.print("door closed");
}
void loop()                     // run over and over again
{
 // if(digitalRead(10)!=HIGH)
//  {
  if(Serial.available()>0)
  {
    if(Serial.read()=='$')
    {
      //enroll
      while(!enrolled)
      {
        getFingerprintEnroll(id);
       // lcd.show(); lcd.print("Failed, Try again");
       // delay(2000);
      }
      enrolled=0;
      id++;
     // open_lock();
    }
    else if(lock_status==0)
    {
      open_lock();
    }
  }
  else
  getFingerprintIDez();
  delay(50);       //don't ned to run this at full speed.
}
//  else
//  {
//    Serial.println(200);
 //   open_lock();
//  }
//}
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  {if(lock_status==1) {Serial.println(199); close_lock();}return -1;}
    Serial.println((int)finger.fingerID);
    //move lock
    if(lock_status==0)
    open_lock();
    else if(valid_position()==1)
    close_lock();
    delay(5000);
    return finger.fingerID;
}
uint8_t getFingerprintEnroll(int id) {
  int p = -1;
  //Serial.println("Waiting for valid finger to enroll");
  lcd_show();
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
    //  Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
     // Serial.println(".");
     lcd.print("Place Finger");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
     // Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
     // Serial.println("Imaging error");
      break;
    default:
     // Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
     // Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
     // Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
     // Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
     // Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }
  
  //Serial.println("Remove finger");
  lcd_show(); lcd.print("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  p = -1;
  //Serial.println("Place same finger again");
  lcd_show();
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
    //  Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.print(".");
      lcd.print("Place Finger");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      break;
    default:
      //Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }
  
  
  // OK converted!
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    //Serial.println("Fingerprints did not match");
    return p;
  } else {
    //Serial.println("Unknown error");
    return p;
  }   
  
  //Serial.print("ID ");// Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    //Serial.println("Stored!");
    lcd_show(); lcd.print("Done");
    enrolled=1;
    delay(3000);
    return 1;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    //Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    //Serial.println("Error writing to flash");
    return p;
  } else {
    //Serial.println("Unknown error");
    return p;
  }   
}
