#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>

// ====== PINOS ======
const int DHT_PIN = 15;      // DHT22 DATA (SDA no Wokwi)
const int LDR_PIN = 34;      // AO do módulo LDR

const int BTN_BEM = 12;        // Botão verde
const int BTN_NEUTRO = 13;     // Botão amarelo
const int BTN_ESTRESSADO = 14; // Botão vermelho

// ====== OBJETO DHT ======
DHTesp dht;

// ====== REDE WI-FI (Wokwi) ======
const char* ssid = "Wokwi-GUEST"; // padrão do Wokwi
const char* password = "";        // sem senha no Wokwi

// ====== MQTT ======
const char* mqttServer = "broker.hivemq.com";
const int   mqttPort   = 1883;
const char* mqttTopic  = "zenflow/estacoes/EST01";

// Cliente WiFi e MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// ====== VARIÁVEIS GERAIS ======
String estacaoId = "EST01";
String humorAtual = "neutro";  // valor inicial
unsigned long ultimoEnvio = 0;
const unsigned long intervaloEnvio = 5000; // 5 segundos

// ====== CONECTAR WI-FI ======
void conectarWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.print("Conectando ao WiFi ");
  Serial.print(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// ====== CONECTAR MQTT ======
void conectarMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Conectando ao broker MQTT...");
    // Gera um clientId aleatório
    String clientId = "ZenFlowESP32-";
    clientId += String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str())) {
      Serial.println(" conectado!");
      // se quiser assinar algo: mqttClient.subscribe("...");
    } else {
      Serial.print(" falhou, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" – tentando de novo em 5s");
      delay(5000);
    }
  }
}

// ====== LER HUMOR PELOS BOTÕES ======
void lerHumor() {
  bool bemPressionado        = digitalRead(BTN_BEM) == LOW;
  bool neutroPressionado     = digitalRead(BTN_NEUTRO) == LOW;
  bool estressadoPressionado = digitalRead(BTN_ESTRESSADO) == LOW;

  if (bemPressionado) {
    humorAtual = "bem";
  } else if (neutroPressionado) {
    humorAtual = "neutro";
  } else if (estressadoPressionado) {
    humorAtual = "estressado";
  }
  // Se nenhum botão estiver pressionado, mantém o último humor
}

// ====== PUBLICAR LEITURA NO MQTT ======
void publicarLeituraMQTT() {
  // Lê DHT22
  TempAndHumidity data = dht.getTempAndHumidity();
  float temperatura = data.temperature;
  float umidade     = data.humidity;

  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Falha ao ler DHT22");
    return;
  }

  // Lê LDR (0 a 4095)
  int ldrValor = analogRead(LDR_PIN);

  unsigned long agoraMs = millis();

  // Monta JSON
  String payload = "{";
  payload += "\"estacaoId\":\"" + estacaoId + "\",";
  payload += "\"humor\":\"" + humorAtual + "\",";
  payload += "\"temperatura\":" + String(temperatura, 2) + ",";
  payload += "\"umidade\":" + String(umidade, 2) + ",";
  payload += "\"luminosidade\":" + String(ldrValor) + ",";
  payload += "\"timestampMs\":" + String(agoraMs);
  payload += "}";

  Serial.print("Publicando no MQTT: ");
  Serial.println(payload);

  mqttClient.publish(mqttTopic, payload.c_str());
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // DHT
  dht.setup(DHT_PIN, DHTesp::DHT22);

  // Botões com pull-up interno
  pinMode(BTN_BEM,        INPUT_PULLUP);
  pinMode(BTN_NEUTRO,     INPUT_PULLUP);
  pinMode(BTN_ESTRESSADO, INPUT_PULLUP);

  // LDR
  pinMode(LDR_PIN, INPUT);

  // WiFi + MQTT
  conectarWiFi();
  mqttClient.setServer(mqttServer, mqttPort);

  randomSeed(micros()); // pra gerar clientId aleatório

  conectarMQTT();

  Serial.println("ZenFlow Sense com MQTT iniciado!");
}

void loop() {
  // Garante WiFi e MQTT conectados
  if (WiFi.status() != WL_CONNECTED) {
    conectarWiFi();
  }
  if (!mqttClient.connected()) {
    conectarMQTT();
  }
  mqttClient.loop(); // mantém conexão viva

  // Atualiza humor conforme botões
  lerHumor();

  // Envia dados periodicamente
  unsigned long agora = millis();
  if (agora - ultimoEnvio >= intervaloEnvio) {
    ultimoEnvio = agora;
    publicarLeituraMQTT();
  }

  delay(50);
}
