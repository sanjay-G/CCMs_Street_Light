#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid = "San's Mobile";
const char* password = "791384265";

String openWeatherMapApiKey = "9a63a2454dfb132b32a696ece8d00198";
String city = "COIMBATORE";
String countryCode = "IN";

String responseData;
const long IST_OFFSET = 5 * 3600 + 30 * 60;

struct Sunrise {
  String year;
  String month;
  String day;
  String hour;
  String minute;
  String second;
};

struct Sunset {
  String year;
  String month;
  String day;
  String hour;
  String minute;
  String second;
};

Sunrise SunriseData = {"0", "0", "0", "0", "0", "0"};
Sunset SunsetData =  {"0", "0", "0", "0", "0", "0"};


void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected");
  getWeatherData();
  

}

void loop() {
  
  // Serial.println("SunriseTime : "+SunriseData.hour+":"+SunriseData.minute);
  // Serial.println("SunsetTime : "+SunsetData.hour+":"+SunsetData.minute);
  // delay(2000);

  //input of lux sensor adjust the brightness
  // After sunrise and sunset time - comparision of lux for brightness adjustment

}








String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  http.begin(client, serverName);
  
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}



void getWeatherData(){
  if(WiFi.status()== WL_CONNECTED){
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
      String responseData=httpGETRequest(serverPath.c_str());
      Serial.println(responseData);
      JSONVar myObject = JSON.parse(responseData);
      unsigned long SunriseEpoch = myObject["sys"]["sunrise"];
      unsigned long SunsetEpoch=myObject["sys"]["sunset"];  
      //------------------------------------------------------------//
    String sunrise = convertEpochToIST(SunriseEpoch);
     SunriseData.year = sunrise.substring(0,4);
     SunriseData.month = sunrise.substring(5,7);
     SunriseData.day = sunrise.substring(8,10);
     SunriseData.hour = sunrise.substring(11,13);
     SunriseData.minute = sunrise.substring(14,16);
     //------------------------------------------------------------//
     String Sunset = convertEpochToIST(SunsetEpoch);
     SunsetData.year = Sunset.substring(0,4);
     SunsetData.month = Sunset.substring(5,7);
     SunsetData.day = Sunset.substring(8,10);
     SunsetData.hour = Sunset.substring(11,13);
     SunsetData.minute = Sunset.substring(14,16);

     delay(1000);
     
  }
}

String convertEpochToIST(unsigned long epoch) {
  // Apply IST offset
  epoch += IST_OFFSET;

  // Calculate date and time
  int seconds = epoch % 60;
  epoch /= 60;
  int minutes = epoch % 60;
  epoch /= 60;
  int hours = epoch % 24;
  epoch /= 24;

  int days = epoch; // Days since 1/1/1970

  // Calculate year (approximate for simplicity)
  int year = 1970;
  while (days >= 365) {
    if (isLeapYear(year)) {
      if (days >= 366) days -= 366;
      else break;
    } else {
      days -= 365;
    }
    year++;
  }

  // Calculate month and day
  int month = 1;
  while (true) {
    int daysInMonth = getDaysInMonth(month, year);
    if (days < daysInMonth) break;
    days -= daysInMonth;
    month++;
  }
  int day = days + 1;

  // Format timestamp as "YYYY-MM-DD HH:MM:SS"
  char buffer[20];
  snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d-%02d:%02d:%02d", year, month, day, hours, minutes, seconds);

  return String(buffer);
}

// Function to check if a year is a leap year
bool isLeapYear(int year) {
  if (year % 4 != 0) return false;
  if (year % 100 != 0) return true;
  return year % 400 == 0;
}

// Function to get the number of days in a month
int getDaysInMonth(int month, int year) {
  switch (month) {
    case 4: case 6: case 9: case 11: return 30;
    case 2: return isLeapYear(year) ? 29 : 28;
    default: return 31;
  }
}
