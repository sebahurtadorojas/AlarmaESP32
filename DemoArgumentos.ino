#include <dummy.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <EEPROM.h>  
WiFiMulti wifiMulti;
WebServer server(80);



#include "data.h"
const uint32_t TiempoEsperaWifi = 5000;

#define EEPROM_SIZE 1
int alarmaState=0;





void mensajeArgumento() {
  String mensaje = "Valores Recividos\n\n";
  mensaje += "URI: ";
  mensaje += server.uri();
  mensaje += "\nMetodo: ";
  mensaje += (server.method() == HTTP_GET) ? "GET" : "POST";
  mensaje += "\nArgumentos: ";
  mensaje += server.args();
  mensaje += "\n";
  for (int i = 0; i < server.args() ; i++) {
    mensaje += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  mensaje += "\n";
  if (server.hasArg("pollo")) {
    mensaje += "El Pollo dice " + server.arg("pollo");
  } else {
    mensaje += "No hay pollo";
  }
  server.send(200, "text/plain", mensaje);
}

void funcionActivo(){
  EEPROM.write(0,1);
  EEPROM.commit();
  Serial.println("Activo");
  server.send(200,"text/plain","Estamos onFire mi gente");
}
void funcionApagado(){
  EEPROM.write(0,0);
  EEPROM.commit();
  Serial.println("Apagado");
  server.send(200,"text/plain","Que sucede mi gente");
}

void mensajeError() {
  String mensaje = "<h1>404</h1>";
  mensaje += "Pagina No encontrada</br>";
  mensaje += "Intenta otra pagina</br>";
  server.send(404, "text/html", mensaje);
}

void nextWebPage(){
  Serial.print("Cambiar Pagina");
  server.send(200,"text/html",pagina2);
}


void readings(){
  String json="{\"Estado\":0,\"Prueba\":\"Hola\"}";
server.send(200,"application/json",json);}



String p1=R"====(const elemEstado=document.getElementById("check"); elemEstado.checked=)====";
  
void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  Serial.println("\nIniciando Server Web");
  wifiMulti.addAP(ssid_1, password_1);
  WiFi.mode(WIFI_STA);
  Serial.print("Conectando a Wifi ..");
  while (wifiMulti.run(TiempoEsperaWifi) != WL_CONNECTED) {
    Serial.print(".");
  }
  Serial.println(".. Conectado");
  Serial.print("SSID: ");
  Serial.print(WiFi.SSID());
  Serial.print(" ID: ");
  Serial.println(WiFi.localIP());
  if (!MDNS.begin("elesp")) {
    Serial.println("Error configurando mDNS!");
    while (1) {
      delay(1000);
    }
  }
  MDNS.addService("http", "tcp", 80);
  alarmaState=EEPROM.read(0);


  p1+=alarmaState;
  p1+="; ";
  p1+=java;
  p1+=fin;
  Serial.println(p1);
  server.on("/", []() {
    if (!server.authenticate(admin_server, password_server)) {
      return server.requestAuthentication();
    }
    server.send(200, "text/html", pag+p1);
  });
  

  server.on("/readings",readings);

  server.on("/test.svg", drawGraph);
  server.on("/Activo",funcionActivo);
  server.on("/Apagado",funcionApagado);
  server.on("/config",nextWebPage);
  server.onNotFound(mensajeError);
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
}

void drawGraph() {
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x += 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";

  server.send(200, "image/svg+xml", out);
}
