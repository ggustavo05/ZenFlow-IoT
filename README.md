# 🌿 ZenFlow Sense – Estação de Bem-Estar no Trabalho

> Monitoramento em tempo real do bem-estar no ambiente de trabalho usando IoT, MQTT e Node-RED.

<!--
Se tiver um banner, troque o caminho abaixo, por exemplo:
![ZenFlow Sense Banner](docs/assets/zenflow-banner.png)
-->
![Logo ZenFlow](https://drive.google.com/uc?export=view&id=1QZ5OrgmxkSRhUeOyufPQ0KDM32LUoNCb)

---

## 📌 Sobre o projeto

O **ZenFlow Sense** é uma estação IoT focada em **bem-estar no ambiente de trabalho**, capaz de coletar dados ambientais (temperatura, umidade, luminosidade) e o **humor auto-relatado** dos colaboradores por meio de botões físicos.

A solução calcula um **Índice de Bem-Estar (0–100)** em tempo real e apresenta tudo em um **dashboard no Node-RED**, permitindo acompanhar rapidamente se o ambiente está favorável, neutro ou em alerta de estresse.

É um dos componentes do ecossistema **ZenFlow**, voltado para saúde mental e qualidade de vida no trabalho.

---

## 🎯 Objetivo

Fornecer uma visão **clara, visual e em tempo real** do bem-estar no ambiente de trabalho, permitindo que líderes, RH e times de People Analytics:

- identifiquem **momentos de maior estresse**;
- acompanhem **como o ambiente físico influencia** no humor (calor, abafado, frio, etc.);
- tomem decisões mais assertivas sobre **clima organizacional, ergonomia e conforto**.

---

## 🤖 Alinhamento com o tema “O Futuro do Trabalho”

O projeto responde diretamente ao tema **“Disruptive Architectures: IoT, IoB & Generative IA – Futuro do Trabalho”**, pois integra:

- **Digitalização do clima emocional**: uso de IoT para transformar percepções subjetivas (humor) em dados objetivos.
- **Ambientes centrados no colaborador**: monitoramento contínuo de conforto térmico e luminosidade.
- **Cultura data-driven**: fornece métricas de bem-estar que podem ser cruzadas com produtividade, absenteísmo e engajamento.
- **Saúde mental como prioridade**: permite identificar padrões de estresse e agir preventivamente.

---

## 🧩 Problemática

No “futuro do trabalho”, espera-se que as pessoas:

- sejam mais **produtivas**, criativas e resilientes;
- se adaptem a **mudanças constantes**;
- lidem com **modelos híbridos, metas agressivas e hiperconectividade**.

Na prática, isso gera:

- aumento de **estresse e sobrecarga emocional**;
- ambientes físicos muitas vezes **desconfortáveis** (calor, ar seco, pouco ventilado, pouca luz);
- baixa visibilidade para RH e liderança sobre o **estado real de bem-estar** dos times.

Sem dados confiáveis, a empresa:

- reage **tarde demais** a situações de adoecimento mental;
- não consegue mensurar o impacto de ações de bem-estar;
- perde oportunidades de melhorar o clima antes de virar um problema sério.

---

## ✅ Solução proposta

O **ZenFlow Sense** oferece:

- 🛰️ **Estação IoT** com ESP32, sensores ambientais e botões de humor;
- 📡 **Comunicação via MQTT** com mensagens em JSON;
- 🧮 Cálculo de **Índice de Bem-Estar (0–100)** que combina:
  - humor auto-relatado,
  - temperatura,
  - umidade;
- 📊 **Dashboard em Node-RED** com:
  - gráfico de sensores (Temperatura x Umidade),
  - indicador do humor atual,
  - gauge de Índice de Bem-Estar,
  - card de Status do Ambiente (Positivo / Neutro / Alerta de Estresse).

Com isso, a empresa passa a ter uma **visão contínua** do bem-estar e pode agir rapidamente.

---

## 🏗️ Arquitetura do sistema

### Visão geral

```text
┌───────────────────────────────────────────────────────────────┐
│                        ZENFLOW SENSE                          │
└───────────────────────────────────────────────────────────────┘

        PARTE 1 – Coleta e Publicação (ESP32 + Wokwi)
        ────────────────────────────────────────────
        DHT22 (Temp/Umid) ─┐
                           ├─→ ESP32 → JSON → MQTT → broker.hivemq.com
        LDR (Luz) ─────────┤
        Botões de Humor ───┘

        PARTE 2 – Gateway e Processamento (Node-RED)
        ────────────────────────────────────────────
        MQTT In → JSON → Processa Leitura
                           │
                           ├→ Cálculo Índice de Bem-Estar
                           ├→ Gráfico Sensores Ambientais
                           ├→ Humor Atual
                           └→ Status do Ambiente





        PARTE 3 – Dashboard (Node-RED Dashboard)
        ────────────────────────────────────────────
        • Gráfico de Sensores Ambientais (Temp x Umid)
        • Card de Status do Ambiente
        • Gauge do Índice de Bem-Estar
        • Header com logo e título ZenFlow Sense

```

## 🔄 Fluxo de comunicação

O fluxo de comunicação do **ZenFlow Sense** é dividido em três camadas principais: dispositivo IoT, gateway (Node-RED) e visualização (dashboard).

1. O **ESP32** (simulado no Wokwi) lê:
   - Temperatura e umidade pelo **DHT22**;
   - Luminosidade pelo **módulo LDR (AO)**;
   - Humor do colaborador por meio de **3 botões físicos** (`bem`, `neutro`, `estressado`).

2. O dispositivo monta um **JSON** com todas as leituras e o estado atual do humor e publica via **MQTT** no broker público:

   - **Broker:** `broker.hivemq.com`  
   - **Porta:** `1883`  
   - **Tópico:** `zenflow/estacoes/EST01`

3. O **Node-RED** atua como gateway:
   - O nó `mqtt in` assina o tópico `zenflow/estacoes/EST01`;
   - O nó `json` converte a string em objeto JavaScript;
   - O nó `function Processa leitura / índice de bem-estar` calcula o campo `indiceBemEstar` (0 a 100), combinando humor, temperatura e umidade;
   - A mensagem processada é distribuída para os demais nós (gráficos, gauge, status, etc.).

4. O **dashboard do Node-RED** exibe os dados em tempo real:
   - **Gráfico de Sensores Ambientais** com duas linhas (Temperatura x Umidade);
   - **Humor atual** do colaborador;
   - **Índice de Bem-Estar** em um gauge;
   - **Status do Ambiente** (Ambiente Positivo, Neutro ou Alerta de Estresse), com cor e texto descritivo.

Diagrama textual do fluxo:

```text
ESP32 (Wokwi)
  ├─ DHT22 (Temperatura / Umidade)
  ├─ LDR (Luminosidade)
  └─ Botões de Humor (Bem / Neutro / Estressado)
        │
        ▼
Publicação MQTT (JSON)
  Broker: broker.hivemq.com
  Tópico: zenflow/estacoes/EST01
        │
        ▼
Node-RED (Gateway)
  mqtt in → json → function (Processa leitura / índice de bem-estar)
        │
        ├─ ui_chart (Gráfico Sensores Ambientais)
        ├─ ui_gauge (Índice de Bem-Estar)
        ├─ ui_text (Humor atual)
        └─ ui_template (Status do Ambiente + Header)
        │
        ▼
Node-RED Dashboard (Visualização em tempo real)
```

## 📨 Formato da mensagem MQTT

---
- Broker: broker.hivemq.com
- Porta: 1883
- Tópico (exemplo):

```text
  zenflow/estacoes/EST01
```
- Payload (JSON):

```json
  {
  "estacaoId": "EST01",
  "humor": "neutro",
  "temperatura": 26.7,
  "umidade": 40,
  "luminosidade": 1001,
  "timestampMs": 39153
}

```
- Após o processamento no Node-RED é acrescentado:

```json
  "indiceBemEstar": 60
```

## 🌡️ Cálculo do Índice de Bem-Estar

No Node-RED, o node `function Processa leitura / índice de bem-estar:`

1. Recebe o objeto JSON em msg.payload.

2. Define um índice base pelo humor:

- bem → 90

- neutro → 70

- estressado → 40

3. Ajusta o índice com base em faixas ideais:

- Temperatura ideal: 20°C a 26°C

  - Se estiver fora desse intervalo → –10 pontos

- Umidade ideal: 30% a 70%

  - Se estiver fora desse intervalo → –5 pontos

4. Garante limites:

- mínimo: 0

- máximo: 100

5. Atribui o valor final em:

```javascript

data.indiceBemEstar = indice;
msg.payload = data;
return msg;

```
## 🧱 Componentes de hardware (simulação Wokwi)

Na simulação Wokwi, o hardware do **ZenFlow Sense** é composto por:

| Componente              | Modelo               | Função                                           | Pino ESP32      |
|------------------------|----------------------|--------------------------------------------------|-----------------|
| Microcontrolador       | ESP32 DevKit V1      | Processamento, WiFi e publicação MQTT            | –               |
| Sensor Temp/Umidade    | DHT22                | Mede temperatura (°C) e umidade relativa (%)     | GPIO 15 (DATA)  |
| Sensor de Luminosidade | Módulo LDR (AO)      | Mede intensidade de luz (0–4095)                 | GPIO 34 (AO)    |
| Botão “Bem”            | Push button          | Sinaliza humor positivo                          | GPIO 12         |
| Botão “Neutro”         | Push button          | Sinaliza humor neutro                            | GPIO 13         |
| Botão “Estressado”     | Push button          | Sinaliza humor de estresse                       | GPIO 14         |
| Alimentação sensores   | 3V3 / GND do ESP32   | Alimentação do DHT22 e do módulo LDR             | 3V3 / GND       |

**Observações:**

- O **DHT22** é ligado em:
  - `VCC` → `3V3`
  - `GND` → `GND`
  - `DATA` (`SDA` no Wokwi) → `GPIO 15`
- O **módulo LDR** é ligado em:
  - `VCC` → `3V3`
  - `GND` → `GND`
  - `AO` → `GPIO 34`
  - `DO` não é utilizado.
- Os **botões** utilizam `INPUT_PULLUP`:
  - Um lado do botão → `GND`
  - Outro lado → GPIO (`12`, `13`, `14`)

![Montagem do hardware no Wokwi](https://drive.google.com/uc?export=view&id=1jkfXLGqpHiCBjlERcXvp6dTtzjSb5pY7)


## 📋 Requisitos técnicos da disciplina

A solução foi desenvolvida para atender aos requisitos da disciplina  
**“Disruptive Architectures: IoT, IoB & Generative IA – Futuro do Trabalho”**:

| Requisito                                     | Atendido? | Como foi implementado                                                  |
|-----------------------------------------------|:---------:|------------------------------------------------------------------------|
| Sistema IoT                                   | ✅        | ESP32 com DHT22, módulo LDR e botões físicos de humor                  |
| Hardware (Arduino/ESP32, físico ou Wokwi)     | ✅        | Simulação completa no Wokwi com ESP32 DevKit V1                        |
| Dashboard em tempo real                       | ✅        | Node-RED Dashboard com gráfico, gauge, status e humor atual            |
| Gateway (Node-RED, ThingSpeak ou equivalente) | ✅        | Node-RED como gateway de processamento e integração                    |
| Protocolo MQTT/HTTP com JSON                  | ✅        | MQTT + JSON via `broker.hivemq.com` no tópico `zenflow/estacoes/EST01`|
| Aplicação alinhada ao tema Futuro do Trabalho | ✅        | Monitoramento contínuo do bem-estar no ambiente de trabalho            |

Além disso, a solução está integrada à ideia central do **ZenFlow** (bem-estar, saúde mental e performance sustentável no trabalho).


## 🛠️ Tecnologias utilizadas

### Dispositivo IoT (ESP32 + Wokwi)

- **Linguagem:** C++ (plataforma Arduino)
- **Simulador:** [Wokwi](https://wokwi.com/)
- **Principais bibliotecas:**
  - `WiFi.h` – conexão do ESP32 à rede WiFi
  - `PubSubClient.h` – cliente MQTT para publicação no broker
  - `DHTesp.h` – leitura do sensor DHT22
- **Protocolo de comunicação:** MQTT (publicação em tópico exclusivo por estação)
- **Formato de dados:** JSON

### Gateway & Dashboard (Node-RED)

- **Plataforma:** Node-RED
- **Funcionalidade como gateway:**
  - Recebe mensagens MQTT do ESP32
  - Converte para objeto JSON
  - Calcula o Índice de Bem-Estar
  - Distribui para os componentes visuais e lógicos
- **Nodes principais:**
  - `mqtt in` – assina o tópico `zenflow/estacoes/EST01`
  - `json` – parse do payload em JSON
  - `function` – regras de negócio (cálculo do índice, preparação de dados)
  - `ui_chart` – gráfico de sensores (Temperatura x Umidade)
  - `ui_gauge` – gauge do Índice de Bem-Estar
  - `ui_text` – exibição do humor atual
  - `ui_template` – header com logo e card de Status do Ambiente
- **Dashboard:**
  - Organizado em três colunas:
    - Gráfico de Sensores Ambientais
    - Header + Humor Atual
    - Índice de Bem-Estar + Status do Ambiente

### Outras ferramentas

- **Git & GitHub** – versionamento e hospedagem do código e documentação.
- **Google Drive / Imagens** – armazenamento da logo e screenshots para o README.

![Fluxo Node-RED do ZenFlow Sense](https://drive.google.com/uc?export=view&id=1PHlt9S-yrgM4ZIkRlMavVgJGR-asS4PI)

## 🚀 Como executar o projeto

### 1. Clonar o repositório

```bash
git clone https://github.com/SEU-USUARIO/SEU-REPOSITORIO.git
cd SEU-REPOSITORIO
```

### 2. Executar a simulação no Wokwi (ESP32)
1. Acesse o Wokwi
2. Crie um novo projeto com ESP32.
3. Importe os arquivos da pasta `wokwi/` do repositório:
  - `diagram.json` → circuito (hardware)
  - `sketch.ino` → código do ESP32
4. Verifique se o código está configurado com:
  
  -  SSID: `Wokwi-GUEST`

 - Senha: vazia ` ("")`

 - Broker MQTT: `broker.hivemq.com`

 - Porta: `1883`

 - Tópico: `zenflow/estacoes/EST01`
5. Clique em Start Simulation.
6. Abra o Serial Monitor para conferir:
- conexão ao WiFi,

- conexão ao broker MQTT,

- publicação dos JSONs com as leituras.

### 3. Subir o Node-RED (Gateway + Dashboard)
3.1. Instalar Node-RED (se ainda não tiver)

Caso ainda não tenha o Node-RED instalado:

``` bash
npm install -g --unsafe-perm node-red
```
3.2. Iniciar o Node-RED
``` bash
node-red
```
### 4. Importar os fluxos do ZenFlow Sense

1. Com o Node-RED aberto, acesse o editor em:
http://localhost:1880

2. No menu superior direito, clique em:
☰ → Import → Clipboard.

3. Abra o arquivo node-red/flows.json do repositório, copie o conteúdo e cole na janela de import.

4. Clique em Import.

5. Verifique os nós MQTT:

 - Server: broker.hivemq.com

 - Port: 1883

 - Topic: zenflow/estacoes/EST01

6. Clique em Deploy (canto superior direito) para aplicar as configurações.

### 5. Acessar o dashboard

1. No Node-RED, vá em:
☰ → Dashboard.

2. Na aba Site, verifique o caminho do dashboard (por padrão é /ui).

3. Acesse no navegador:
   ``` text
   http://localhost:1880/ui
   ```
---

![Dashboard ZenFlow Sense](https://drive.google.com/uc?export=view&id=1nCJm2WDT90d-pfDikJPmTZEfcKjzYr3U)
---

# Link  do Vídeo:
https://youtu.be/G0nL41UIujo


   ---
   ## 👥 Equipe
| Nome               | RM     |
| ------------------ | ------ |
| Amanda Galdino     | 560066 |
| Bruno Cantacini    | 560242 |
| Gustavo Gonçalves | 556823 |
