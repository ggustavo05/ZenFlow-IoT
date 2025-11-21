# 🌿 ZenFlow Sense – Estação de Bem-Estar no Trabalho

> Monitoramento em tempo real do bem-estar no ambiente de trabalho usando IoT, MQTT e Node-RED.

<!--
Se tiver um banner, troque o caminho abaixo, por exemplo:
![ZenFlow Sense Banner](docs/assets/zenflow-banner.png)
-->
![ZenFlow Sense Banner](docs/assets/zenflow-banner.png)

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
