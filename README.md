---

# Monitor de Temperatura & Umidade com ESP32 e DHT11

Um projeto simples e visual para monitorar temperatura e umidade em tempo real usando um **ESP32**, sensor **DHT11** e interface web com **HTML, CSS e Chart.js**.

---

## 💡 Funcionalidades

* **Leitura em tempo real** de temperatura (°C) e umidade (%).
* **Cálculo de médias móveis** para 10 minutos, 30 minutos e 1 hora.
* **Gráfico histórico** das últimas leituras usando Chart.js.
* **Interface web responsiva** acessível via navegador.
* **Servidor web embutido no ESP32** sem necessidade de PC intermediário.

---

## ⚙️ Componentes Necessários

* ESP32
* Sensor DHT11
* Cabo USB para programação do ESP32
* Rede Wi-Fi

---

## 🛠️ Instalação

### 1. Configurar o Arduino IDE

1. Instale a [Arduino IDE](https://www.arduino.cc/en/software).
2. Adicione suporte ao **ESP32** (Guia oficial: [ESP32 Board Manager](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)).
3. Instale as bibliotecas necessárias:

   * `DHTesp`
   * `WiFi` (já incluída no pacote ESP32)
   * `WebServer` (já incluída no pacote ESP32)

### 2. Configurar Wi-Fi

No arquivo `.ino`, altere:

```cpp
WiFi.begin("NOME_DA_REDE", "SENHA_DA_REDE");
```

Para os dados da sua rede local.

### 3. Conectar o DHT11

* DHT11 → pino **26** do ESP32 (conforme código, alterável se desejar).

### 4. Upload do código

1. Conecte o ESP32 ao computador.
2. Selecione a placa correta e porta no Arduino IDE.
3. Faça o **upload do sketch** (`.ino`) para o ESP32.

---

## 🌐 Interface Web

O HTML/CSS usa **Chart.js** para exibir gráficos dinâmicos.

* Abra o navegador e acesse:

```
http://<IP_DO_ESP32>
```

> O IP do ESP32 será exibido no Monitor Serial após conectar ao Wi-Fi.

**Recursos da interface:**

* Leitura atual de temperatura e umidade.
* Médias móveis para 10min, 30min e 1h.
* Gráfico histórico das últimas leituras (até 60 pontos).

---

## 🧮 Funcionamento do Código

### Arduino (.ino)

* Coleta dados do **DHT11** a cada segundo.
* Armazena leituras em um **buffer circular** de 1 hora.
* Calcula **médias móveis** em 10 min, 30 min e 60 min.
* Expondo um endpoint `/dados` que retorna JSON com:

  * `temp_atual`
  * `umid_atual`
  * `media10_temp`, `media30_temp`, `media60_temp`
  * `media10_umid`, `media30_umid`, `media60_umid`

### HTML/JS

* Faz **fetch do endpoint `/dados`** a cada 2 segundos.
* Atualiza os valores na página.
* Adiciona novas leituras no gráfico de temperatura.

---

## 📈 Tecnologias Utilizadas

* ESP32
* Sensor DHT11
* Arduino IDE
* HTML/CSS
* JavaScript
* Chart.js

---

## 📝 Observações

* Certifique-se de que o ESP32 esteja na mesma rede local que o dispositivo que acessará a página web.
* O buffer de dados mantém até 1 hora de leituras (configurável).
* Pinos e intervalos podem ser ajustados conforme necessidade.

---
