#include<LiquidCrystal_I2C.h>
#include <AltSoftSerial.h>
#include <TinyGPS++.h>

#include <SoftwareSerial.h>
#include <math.h>

#include<Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
const String EMERGENCY_PHONE = "+918076518325";
#define rxPin 2
#define txPin 3
SoftwareSerial sim800(rxPin,txPin);
AltSoftSerial neogps;
TinyGPSPlus gps;
String sms_status,sender_number,received_date,msg;
String latitude, longitude;
#define BUZZER 12
#define BUTTON 11
#define xPin A1
#define yPin A2
#define zPin A3
byte updateflag;
int Xaxis = 0, Yaxis = 0, Zaxis = 0;
int deltx = 0, delty = 0, deltz = 0;
int vibration = 2, devibrate = 75;
int magnitude = 0;
int sensitivity = 150;
double angle;
boolean Impact_Detected = false;
unsigned long time1;
unsigned long impact_time;
unsigned long alert_delay = 10000; //30 seconds
void setup()
{
  Serial.begin(9600);
  sim800.begin(9600);
  neogps.begin(9600);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  sms_status = "";
  sender_number="";
  received_date="";
  msg="";
  sim800.println("AT"); //Check GSM Module
  delay(1000);
  sim800.println("ATE1"); //Echo ON
  delay(1000);
  sim800.println("AT+CPIN?"); //Check SIM ready
  delay(1000);
  sim800.println("AT+CMGF=1"); //SMS text mode
  delay(1000);
  sim800.println("AT+CNMI=1,1,0,0,0"); /// Decides how newly arrived SMS should be handled
  delay(1000);
  time1 = micros(); 
  Xaxis = analogRead(xPin);
  Yaxis = analogRead(yPin);
  Zaxis = analogRead(zPin);
}
void loop()
{
  if (micros() - time1 > 1999) Impact();
  if(updateflag > 0) 
  {
    updateflag=0;  
    Serial.println("Impact detected!!");
    Serial.print("Magnitude:"); Serial.println(magnitude);

    getGps();
    digitalWrite(BUZZER, HIGH);
    Impact_Detected = true;
    impact_time = millis();
    
    lcd.clear();
    lcd.setCursor(0,0); //col=0 row=0
    lcd.print("Crash Detected");
    lcd.setCursor(0,1); //col=0 row=1
    lcd.print("Magnitude:"+String(magnitude));
  }
  if(Impact_Detected == true)
  {
    if(millis() - impact_time >= alert_delay) {
      digitalWrite(BUZZER, LOW);
      makeCall();
      
      delay(100);
      
      sendAlert();
      Impact_Detected = false;
      impact_time = 0;
    }
  }
  
  if(digitalRead(BUTTON) == LOW){
    delay(200);
    digitalWrite(BUZZER, LOW);
    Impact_Detected = false;
    impact_time = 0;
  }
  while(sim800.available()){
    parseData(sim800.readString());
  }
  while(Serial.available())  {
    sim800.println(Serial.readString());
  }


}

void Impact()
{
  time1 = micros(); // resets time value
  int oldx = Xaxis; //store previous axis readings for comparison
  int oldy = Yaxis;
  int oldz = Zaxis;

  Xaxis = analogRead(xPin);
  Yaxis = analogRead(yPin);
  Zaxis = analogRead(zPin);
  vibration--; 
  if(vibration < 0) vibration = 0;                                  
  if(vibration > 0) return;
  
  deltx = Xaxis - oldx;                                           
  delty = Yaxis - oldy;
  deltz = Zaxis - oldz;
   magnitude = sqrt(sq(deltx) + sq(delty) + sq(deltz));
  
  if (magnitude >= sensitivity) //impact detected
  {
    updateflag=1;
    vibration = devibrate;
  }
  
  else
  {
    magnitude=0;
  }
}
void parseData(String buff){
  Serial.println(buff);

  unsigned int len, index;
  index = buff.indexOf("\r");
  buff.remove(0, index+2);
  buff.trim();
  if(buff != "OK"){
    index = buff.indexOf(":");
    String cmd = buff.substring(0, index);
    cmd.trim();
    
    buff.remove(0, index+2);
    if(cmd == "+CMTI"){
      index = buff.indexOf(",");
      String temp = buff.substring(index+1, buff.length()); 
      temp = "AT+CMGR=" + temp + "\r"; 
      sim800.println(temp); 
    }
    else if(cmd == "+CMGR"){
      if(buff.indexOf(EMERGENCY_PHONE) > 1){
        buff.toLowerCase();
        if(buff.indexOf("get gps") > 1){
          getGps();
          String sms_data;
          sms_data = "GPS Location Data\r";
          sms_data += "http://maps.google.com/maps?q=loc:";
          sms_data += latitude + "," + longitude;
        
          sendSms(sms_data);
        }
      }
    }
  }
  else{
  }
}

void getGps()
{
  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 2000;){
    while (neogps.available()){
      if (gps.encode(neogps.read())){
        newData = true;
        break;
      }
    }
  }
  
  if (newData) //If newData is true
  {
    latitude = String(gps.location.lat(), 6);
    longitude = String(gps.location.lng(), 6);
    newData = false;
  }
  else {
    Serial.println("No GPS data is available");
    latitude = "";
    longitude = "";
  }

  Serial.print("Latitude= "); Serial.println(latitude);
  Serial.print("Lngitude= "); Serial.println(longitude);
}

void sendAlert()
{
  String sms_data;
  sms_data = "Accident Alert!!\r";
  sms_data += "http://maps.google.com/maps?q=loc:";
  sms_data += latitude + "," + longitude;

  sendSms(sms_data);
}

void makeCall()
{
  Serial.println("calling....");
  sim800.println("ATD"+EMERGENCY_PHONE+";");
  delay(20000); //20 sec delay
  sim800.println("ATH");
  delay(1000); //1 sec delay
}

 void sendSms(String text)
{
  sim800.print("AT+CMGF=1\r");
  delay(1000);
  sim800.print("AT+CMGS=\""+EMERGENCY_PHONE+"\"\r");
  delay(1000);
  sim800.print(text);
  delay(100);
  sim800.write(0x1A); //ascii code for ctrl-26 //sim800.println((char)26); //ascii code for ctrl-26
  delay(1000);
  Serial.println("SMS Sent Successfully.");
}

boolean SendAT(String at_command, String expected_answer, unsigned int timeout){

    uint8_t x=0;
    boolean answer=0;
    String response;
    unsigned long previous;
    
    while( sim800.available() > 0) sim800.read();

    sim800.println(at_command);
    
    x = 0;
    previous = millis();

    do{
        if(sim800.available() != 0){
            response += sim800.read();
            x++;
            if(response.indexOf(expected_answer) > 0){
                answer = 1;
                break;
            }
        }
    }while((answer == 0) && ((millis() - previous) < timeout));

  Serial.println(response);
  return answer;
}
