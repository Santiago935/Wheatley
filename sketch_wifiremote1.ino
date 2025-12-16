#include <vehicle.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "";        // Completar con el nombre de tu WiFi
const char* password = ""; // Completar con la contraseña de tu WiFi

vehicle Acebott;
int Speeds = 150; //velocidad del auto
WebServer server(80);

void handleRoot() {
  String message = "<html>";
  message += "<style> body {-webkit-user-select: none; -khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;}</style><body>";
  message += "<center><h1>moveCar control</h1></center>";
  message +="<center>";
  message += "<button onmousedown=\"moveCar('tl')\" onmouseup=\"moveCar('s')\" ontouchstart=\"moveCar('tl')\" ontouchend=\"moveCar('s')\" style=\"width:200px;height:250px\">Trun left</button>";
  message += "<button onmousedown=\"moveCar('f')\" onmouseup=\"moveCar('s')\" ontouchstart=\"moveCar('f')\" ontouchend=\"moveCar('s')\" style=\"width:200px;height:250px\">Forward</button>";
  message += "<button onmousedown=\"moveCar('tr')\" onmouseup=\"moveCar('s')\" ontouchstart=\"moveCar('tr')\" ontouchend=\"moveCar('s')\" style=\"width:200px;height:250px\">Trun Right</button>";
  message +="</center><center>";
  message += "<button onmousedown=\"moveCar('l')\" onmouseup=\"moveCar('s')\" ontouchstart=\"moveCar('l')\" ontouchend=\"moveCar('s')\" style=\"width:200px;height:250px\">Left</button>";
  message += "<button onmousedown=\"moveCar('b')\" onmouseup=\"moveCar('s')\" ontouchstart=\"moveCar('b')\" ontouchend=\"moveCar('s')\" style=\"width:200px;height:250px\">Backward</button>";
  message += "<button onmousedown=\"moveCar('r')\" onmouseup=\"moveCar('s')\" ontouchstart=\"moveCar('r')\" ontouchend=\"moveCar('s')\" style=\"width:200px;height:250px\">Right</button>";
  message +="</center>";
  message += "<script>";
  message += "function moveCar(move) {";
  message += "  fetch('/Car?move=' + move);";
  message += "}";
  message += "</script>";
  message += "</body></html>";
  server.send(200, "text/html", message);
}

void handlemoveCar() {
  if (server.hasArg("move")) {
    String move = server.arg("move");
    Serial.println(move);
    if (move == "f") {
      Acebott.Move(Forward, Speeds); 
    } else if (move == "b") {
      Acebott.Move(Backward, Speeds); 
    } else if (move == "l") {
      Acebott.Move(Contrarotate, Speeds);
    } else if (move == "r") {
      Acebott.Move(Clockwise, Speeds); 
    } else if (move == "s") {
      Acebott.Move(Stop, 0);
    } 
    else if (move == "tl") {
      Acebott.Move(Move_Left, Speeds);
    } 
    else if (move == "tr") {
      Acebott.Move(Move_Right, Speeds);
    } 
  } 
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);
  

  WiFi.mode(WIFI_STA);
  WiFi.setTxPower(WIFI_POWER_19_5dBm);
  
  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  
  // Esperar la conexión
  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos < 20) {
    delay(500);
    Serial.print(".");
    intentos++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado a WiFi!");
    Serial.print("IP del Acebott: ");
    Serial.println(WiFi.localIP());  // IP para abrir en la pagina 
    Serial.print("Usa 'http://");
    Serial.print(WiFi.localIP());
    Serial.println("' para conectarte");
  } else {
    Serial.println("\nError: No se pudo conectar a WiFi");
    Serial.println("Verifica el nombre y contraseña de tu red");
  }
  
  server.on("/", handleRoot);
  server.on("/Car", handlemoveCar);
  server.begin();
  Serial.println("Servidor iniciado!");
  
  Acebott.Init();
  Acebott.Move(Stop, 0);
}

void loop() {
  server.handleClient();
}