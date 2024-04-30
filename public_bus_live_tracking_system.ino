#include <SoftwareSerial.h>

//Install the following Libraries
#include <TinyGPS++.h> 
#include <ESP8266WiFi.h>

//GPS Module RX pin to NodeMCU D1
//GPS Module TX pin to NodeMCU D2
const int RXPin = 4, TXPin = 5;
SoftwareSerial neo7m(RXPin, TXPin);

TinyGPSPlus gps;
             
const char *ssid = "wifiname";
const char *password = "wifipassword";

String GMAP_API_KEY = "yourgooglemapsapi";

WiFiServer server(80);

String html;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  neo7m.begin(9600);

    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");    // this is the address to use for viewing the map
    Serial.println(WiFi.localIP());
    server.begin();
      
}

void loop()
{
  smartdelay_gps(1000);
  
  if (gps.location.isValid()) 
  {
    //Storing the Latitude. and Longitude
    String latitude = String(gps.location.lat(), 6);
    String longitude = String(gps.location.lng(), 6); 
    
    //Send to Serial Monitor for Debugging
    //Serial.print("LAT:  ");
    //Serial.println(latitude);  // float to x decimal places
    //Serial.print("LONG: ");
    //Serial.println(longitude);

    // listen for incoming clients
    WiFiClient client = server.available();
    if(client) {                             
    Serial.println("new client");          
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            
      if (client.available()) {             // if there's client data
        char c = client.read();          // read a byte
          if (c == '\n') {                      // check for newline character,
          if (currentLine.length() == 0) {  // if line is blank it means its the end of the client HTTP request
      
    //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
    String html = "<!DOCTYPE html>";
    html += "<html lang='en'>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<meta http-equiv='X-UA-Compatible' content='ie=edge'>";
    html += "<title>Public Bus Live Tracking System</title>";
    html += "<link href='https://fonts.googleapis.com/css2?family=Poppins:wght@400;600&display=swap' rel='stylesheet'>";
    html += "<style>";
    html += "body { background-color: #f0f0f0; color: #333; font-family: 'Poppins', sans-serif; }"; 
    html += "h1 { text-align: center; background-color: #333; color: #fff; padding: 20px; border-radius: 5px; }"; 
    html += ".footer { text-align: center; color: #333; background-color: #fff; padding: 20px; font-size: 16px; border-top: 1px solid #ddd; }";
    html += ".footer:hover { color: #555; }"; // Hover effect
    html += "#map { height: 70vh; width: 100%; border: 2px solid #ddd; border-radius: 5px; }"; 
    html += "</style>";
    html += "</head>";
    html += "<body>";
    html += "<h1>";
    html += "Public Bus Live Tracking System"; 
    html += " <img src='https://cdn-icons-png.flaticon.com/512/9249/9249336.png' width='32' height='32' alt='Vehicle' />";
    html += "</h1>";
    html += "<div id='map'></div>"; 
    html += "<div class='footer'>"; 
    html += "&#169; Developed by Team Spotlight"; 
    html += "</div>"; 
    html += "</body>";
    html += "</html>";
    html+="<script>";
    
    //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
    ///////////////////////////////////////////////////////////////////////////////////

    
    html+="var map;";
    html+="var marker;";
    
    //5000ms means 5000/1000 = 5 Seconds
    //20000ms means 20000/1000 = 20 Seconds
    html+="var INTERVAL = 5000;";
    
    //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
    html+="function initMap(){";
      //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
      html+="var options = {";
          html+="zoom:16,";
          html+="center:{lat:"+latitude+",lng:"+longitude+"},";
          html+="mapTypeId: google.maps.MapTypeId.ROADMAP,";
      html+="};";
      //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
      html+="map = new google.maps.Map(document.getElementById('map'), options);";
    html+="}";
    //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

    //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
    html+="function getMarkers() {";
      //html+="console.log("+latitude+");";
      //html+="console.log("+longitude+");";
      
      html+="var newLatLng = new google.maps.LatLng("+latitude+", "+longitude+");";
      //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
      html+="marker = new google.maps.Marker({";
        html+="position: newLatLng,";
        html+="map: map";
      html+="});";
      //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    html+="}";
    //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

    //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
    html+="window.setInterval(getMarkers,INTERVAL);";
    
    html+="</script>";
    html+="<script async defer src='https://maps.googleapis.com/maps/api/js?key="+GMAP_API_KEY+"&callback=initMap'>";
    html+="</script>";
    html+="</body></html>";
    //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
 
 client.print(html);

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {   currentLine = ""; }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
         // here you can check for any keypresses if your web server page has any
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    }    
  }
}

static void smartdelay_gps(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (neo7m.available())
      gps.encode(neo7m.read());
  } while (millis() - start < ms);
}
