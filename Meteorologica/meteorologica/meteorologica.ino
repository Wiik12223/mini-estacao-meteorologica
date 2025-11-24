#include <WiFi.h>
#include <WebServer.h>
#include <DHTesp.h>

DHTesp dht;
WebServer server(80);

struct Registro {
  unsigned long timestamp;
  float temp;
  float umid;
};

const int MAX_REGISTROS = 3600;  // 1 hora com 1 leitura/s
Registro dados[MAX_REGISTROS];
int indice = 0;
int total_registros = 0;

void salvarLeitura(float t, float h) {
  dados[indice] = { millis(), t, h };
  indice = (indice + 1) % MAX_REGISTROS;
  if (total_registros < MAX_REGISTROS) total_registros++;
}

float mediaTemp(unsigned long intervalo_ms) {
  unsigned long agora = millis();
  float soma = 0;
  int count = 0;

  for (int i = 0; i < total_registros; i++) {
    if (agora - dados[i].timestamp <= intervalo_ms) {
      soma += dados[i].temp;
      count++;
    }
  }
  return count > 0 ? soma / count : 0;
}

float mediaUmid(unsigned long intervalo_ms) {
  unsigned long agora = millis();
  float soma = 0;
  int count = 0;

  for (int i = 0; i < total_registros; i++) {
    if (agora - dados[i].timestamp <= intervalo_ms) {
      soma += dados[i].umid;
      count++;
    }
  }
  return count > 0 ? soma / count : 0;
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\nIniciando sensor DHT11...");
  dht.setup(26, DHTesp::DHT11);

  Serial.println("\nConectando ao WiFi...");
  Serial.print("Rede: ");
  Serial.println("ICM");

  WiFi.begin("ICM", "territoriodofazer");

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tentativas++;

    if (tentativas > 20) {  // 20 tentativas = 10 segundos
      Serial.println("\n❌ Falha ao conectar ao WiFi!");
      Serial.println("Reinicie o ESP32 ou verifique a rede.");
      return;  // impede continuar sem WiFi
    }
  }

  Serial.println("\n\n✅ Conectado com sucesso!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  server.on("/dados", []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    TempAndHumidity d = dht.getTempAndHumidity();
    salvarLeitura(d.temperature, d.humidity);

    String json = "{";
    json += "\"temp_atual\":" + String(d.temperature) + ",";
    json += "\"umid_atual\":" + String(d.humidity) + ",";
    json += "\"media10_temp\":" + String(mediaTemp(600000)) + ",";
    json += "\"media30_temp\":" + String(mediaTemp(1800000)) + ",";
    json += "\"media60_temp\":" + String(mediaTemp(3600000)) + ",";
    json += "\"media10_umid\":" + String(mediaUmid(600000)) + ",";
    json += "\"media30_umid\":" + String(mediaUmid(1800000)) + ",";
    json += "\"media60_umid\":" + String(mediaUmid(3600000));
    json += "}";

    server.send(200, "application/json", json);
  });

  server.begin();
  Serial.println("Servidor web iniciado.");
}

void loop() {
  server.handleClient();
}
