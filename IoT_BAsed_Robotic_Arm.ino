#include <WiFi.h>
#include <Servo.h>

Servo myservo, myservo1, myservo2, myservo3;  // create servo object to control a servo
// twelve servo objects can be created on most boards

// GPIO the servo is attached to
static const int servoPin = 27;
static const int servoPin1 = 14;
static const int servoPin2 = 13;
static const int servoPin3 = 12; 

// Replace with your network credentials
const char* ssid     = "Ali khan";
const char* password = "khanaliraza";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Decode HTTP GET value
String valueString = String(5);
String valueString1 = String(5);
String valueString2 = String(5);
String valueString3 = String(5);

int pos1 = 0;
int pos2 = 0;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {

  myservo.attach(servoPin);  // attaches the servo on the servoPin to the servo object
  myservo1.attach(servoPin1);  // attaches the servo on the servoPin to the servo object
  myservo2.attach(servoPin2);  // attaches the servo on the servoPin to the servo object
  myservo3.attach(servoPin3);  // attaches the servo on the servoPin to the servo object  
  
  Serial.begin(115200);

myservo.write(0);
myservo1.write(0);
myservo2.write(0);
myservo3.write(0);  

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  // put your main code here,to run respectively.
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            client.println(".slider { width: 300px; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
                     
            // Web Page
            client.println("</head><body><h1>ESP32 with Servo</h1>");
           
            client.println("<p>Base: <span id=\"servoPos\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");

            client.println("<p>Shoulder: <span id=\"servoPos1\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider1\" onchange=\"servo1(this.value)\" value1=\""+valueString1+"\"/>");

            client.println("<p>Arm: <span id=\"servoPos2\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider2\" onchange=\"servo2(this.value)\" value2=\""+valueString2+"\"/>");

            client.println("<p>Grip: <span id=\"servoPos3\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider3\" onchange=\"servo3(this.value)\" value3=\""+valueString3+"\"/>");
            
            client.println("<script>var slider = document.getElementById(\"servoSlider\");");
            client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
            client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
            client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}");

            client.println("var slider1 = document.getElementById(\"servoSlider1\");");
            client.println("var servoP1 = document.getElementById(\"servoPos1\"); servoP1.innerHTML = slider1.value;");
            client.println("slider1.oninput = function() { slider1.value = this.value; servoP1.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo1(pos1) { ");
            client.println("$.get(\"/?value=\" + pos1 + \"&\"); {Connection: close};}");

            client.println("var slider2 = document.getElementById(\"servoSlider2\");");
            client.println("var servoP2 = document.getElementById(\"servoPos2\"); servo2.innerHTML = slider2.value;");
            client.println("slider2.oninput = function() { slider2.value = this.value; servoP2.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo2(pos2) { ");
            client.println("$.get(\"/?value=\" + pos2 + \"&\"); {Connection: close};}");
            
            client.println("var slider3 = document.getElementById(\"servoSlider3\");");
            client.println("var servoP3 = document.getElementById(\"servoPos3\"); servoP3.innerHTML = slider3.value;");
            client.println("slider3.oninput = function() { slider3.value = this.value; servoP3.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo3(pos3) { ");
            client.println("$.get(\"/?value=\" + pos3 + \"&\"); {Connection: close};}</script>");
            client.println("</body></html>");     
            
            //GET /?value=180& HTTP/1.1
             if(header.indexOf("GET /?value=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1+1, pos2);
              
              //Rotate the servo
              myservo.write(valueString.toInt());
              Serial.println(valueString); 
            }         

              if(header.indexOf("GET /?value1=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString1 = header.substring(pos1+1, pos2);
              
              //Rotate the servo
              myservo1.write(valueString1.toInt());
              Serial.println(valueString1); 
            }   

             if(header.indexOf("GET /?value2=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString2 = header.substring(pos1+1, pos2);
              
              //Rotate the servo
              myservo2.write(valueString2.toInt());
              Serial.println(valueString2); 
            }   

             if(header.indexOf("GET /?value3=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString3 = header.substring(pos1+1, pos2);
              
              //Rotate the servo
              myservo3.write(valueString3.toInt());
              Serial.println(valueString3); 
            }  

            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
