#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>

char auth[] = "70b318d71fcc441e912a30b97d4a4cc0";
WiFiClient client;
HTTPClient http;
char sid[] = "AAKASH";
char pas[] = "#######";

int i=1,tri=4,vti=-1,value;
String take;
float lat,lon;
int brk=1;
BlynkTimer timer;
WidgetTerminal terminal(V1);
WidgetMap myMap(V3);
void mail()
{
  if(vti==1)
  {
    value=random(1,111);
    if (client.connect("api.thingspeak.com",80)) {
    take="http://api.thingspeak.com/update?key=YOUR_KEY&field1=";
    take +=(value);
    http.begin(take);
    http.GET();
    http.end();
    terminal.print("\nData Sent !! ");
    terminal.print(value);
    terminal.flush();}
  }
  else if(vti==0)
  {
    terminal.print("\n\nLat : ");
    terminal.print(lat);
    terminal.print("      Lon : ");
    terminal.println(lon);
    terminal.flush();
  }
}
void questions(int t)
{
  switch(t)
  {
   case 1 : terminal.println("> What is Your Name ?");
            terminal.flush();
   break;

   case 2 : terminal.println("> Your Password ?");
            if(tri != 0)
            terminal.flush();
   break;

   default : terminal.clear();
             terminal.println("Username/Password is Incorrect !!!");
   break;
  }
}

BLYNK_WRITE(V2)
{
  if(brk=param.asInt())
  {
     timer.setInterval(19000L,mail);
  }
  else
  {
    vti=0;
    mail();
  }
}
BLYNK_WRITE(V1)
{
  if(i==1)
  {
      terminal.clear();
      if(String("Aakash Verma") == param.asStr())
      {
        terminal.print("Welcome Mr.");
        terminal.write(param.getBuffer(),param.getLength());
        terminal.println();
        i=2;
      }
      else
      {
        questions(i);
      }
      terminal.flush();
  }
  if(i==2)
  {   
      if(String("12345") == param.asStr())
      {
        terminal.clear();
        myMap.location(0, lat, lon, "Current Location");  
        terminal.println("\n--------- Access Granted ---------");
        vti=1;   
      }
      else
      {
        if(tri!=4)
        {
          terminal.clear();
          terminal.println("\n--------- Access Denied ---------");
          terminal.flush();
          terminal.print("    ### Attempts lefts : ");
          terminal.print(tri);
          terminal.println(" ###\n");
        }
        questions(i);
        tri--;
      }
        terminal.flush();
        if(tri == -1)
        {
          i=0;
          terminal.clear();
          terminal.flush();
        }
  }
  if(String("teser") == param.asStr())
    {       tri=4;i=0;
            vti=-1;
            terminal.clear();
            questions(++i);
            terminal.flush();}
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, sid, pas);
  terminal.clear();
  questions(1);
}

void loop()
{
  Blynk.run();
  timer.run();
}
