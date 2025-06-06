# Dispositivo de Alerta Local AquaGuard (Edge Computing & IoT)

## 1. Introdução

O **Dispositivo de Alerta Local AquaGuard** é um protótipo desenvolvido como parte da Global Solution da FIAP para a disciplina de Edge Computing & IoT. Este projeto visa fornecer uma solução acessível e eficaz para o monitoramento de níveis de água e emissão de alertas locais imediatos em áreas de risco de enchentes. Utilizando processamento na ponta (edge computing), o dispositivo opera de forma autônoma para alertar os residentes sobre perigos iminentes, permitindo uma resposta mais rápida e potencialmente salvando vidas e patrimônio.

## 2. Problema Abordado

As enchentes representam um desafio crítico e recorrente no Brasil, afetando milhões de pessoas anualmente, especialmente em comunidades vulneráveis com infraestrutura limitada. A ausência ou falha de sistemas de alerta que cheguem de forma clara, rápida e localizada agrava a situação, dificultando a evacuação e a tomada de medidas preventivas. Este projeto busca endereçar essa lacuna, oferecendo um sistema de alerta de baixo custo e fácil compreensão que atua diretamente no local de risco.

## 3. Funcionalidades Principais

* **Monitoramento Contínuo:** Medição em tempo real da distância até a superfície da água utilizando um sensor ultrassônico.
* **Processamento Local (Edge):** Todos os dados são processados e as decisões de alerta são tomadas localmente pelo microcontrolador Arduino, garantindo autonomia e rapidez.
* **Display LCD Informativo:** Um display LCD 16x2 exibe informações cruciais, alternando automaticamente entre duas telas:
    * **Tela 1:** Mostra a distância atual até a água (ex: "Distancia: 85.20 cm").
    * **Tela 2:** Exibe o nível de alerta textual correspondente (ex: "Nivel de alerta:", seguido por "Normal", "Cuidado", "Perigo" ou "PERIGO MAX!").
* **Alertas Visuais Progressivos:** LEDs de cores diferentes (Verde, Amarelo e Vermelho) indicam o status atual do nível da água:
    * Verde: Nível normal/seguro.
    * Amarelo: Nível de cuidado/atenção.
    * Vermelho: Nível de perigo.
* **Alerta Sonoro Crítico:** Um buzzer é ativado quando o nível da água atinge o estado de "PERIGO MAX!", fornecendo um aviso audível e urgente.
* **Operação Não Bloqueante:** A lógica de temporização utiliza a função `millis()` para garantir que o dispositivo permaneça responsivo e atualize as leituras e alertas em intervalos regulares.

## 4. Componentes Utilizados

### Hardware:
* **Microcontrolador:** Arduino Uno (ou similar)
* **Sensor de Distância:** Sensor Ultrassônico HC-SR04
* **Display:** LCD Alfanumérico 16x2 com módulo I2C (endereço padrão: 0x27)
* **Alertas Visuais:**
    * 1x LED Verde (com resistor apropriado, ex: 220Ω)
    * 1x LED Amarelo (com resistor apropriado, ex: 220Ω)
    * 1x LED Vermelho (com resistor apropriado, ex: 220Ω)
* **Alerta Sonoro:** 1x Buzzer (ativo ou passivo)
* **Outros:** Protoboard (Placa de Ensaio), Jumpers (Fios de Conexão)

### Software e Bibliotecas:
* Arduino IDE
* Biblioteca `Wire.h` (nativa do Arduino, para comunicação I2C)
* Biblioteca `LiquidCrystal_I2C.h` (para controle do display LCD via I2C)

## 5. Montagem do Circuito

O circuito pode ser montado conforme a imagem de referência. As principais conexões são:

![image](https://github.com/user-attachments/assets/0c107a15-508c-4451-9bc4-476fb5678712)

* **Sensor HC-SR04:**
    * `VCC` -> `5V` (Arduino)
    * `Trig` -> Pino Digital `5` (Arduino)
    * `Echo` -> Pino Digital `4` (Arduino)
    * `GND` -> `GND` (Arduino)
* **Display LCD I2C:**
    * `VCC` -> `5V` (Arduino)
    * `GND` -> `GND` (Arduino)
    * `SDA` -> Pino Analógico `A4` (ou pino SDA dedicado do Arduino)
    * `SCL` -> Pino Analógico `A5` (ou pino SCL dedicado do Arduino)
* **LEDs:**
    * LED Verde (Anodo) -> Resistor (220Ω) -> Pino Digital `6` (Arduino)
    * LED Amarelo (Anodo) -> Resistor (220Ω) -> Pino Digital `7` (Arduino)
    * LED Vermelho (Anodo) -> Resistor (220Ω) -> Pino Digital `8` (Arduino)
    * Todos os Catodos dos LEDs -> `GND` (Arduino)
* **Buzzer:**
    * Pino Positivo (+) -> Pino Digital `2` (Arduino)
    * Pino Negativo (-) -> `GND` (Arduino)

*(Certifique-se de que o endereço I2C do seu LCD no código (`0x27`) corresponde ao do seu módulo.)*

## 6. Funcionamento e Lógica do Software

O código no Arduino opera da seguinte forma:

1.  **Inicialização (`setup()`):** Configura os pinos, inicializa o display LCD e a comunicação serial (opcional para debug).
2.  **Loop Principal (`loop()`):**
    * **Leitura do Sensor:** A cada `intervaloLeitura` (definido em 500ms no código mais recente), o sensor ultrassônico é acionado para medir a distância até a água. A distância é convertida para centímetros.
    * **Lógica de Alerta:** Com base na `distanciaAtual` medida:
        * **Normal:** Se `distanciaAtual >= 100 cm` -> LED Verde acende.
        * **Cuidado:** Se `90 cm <= distanciaAtual < 100 cm` -> LED Amarelo acende.
        * **Perigo:** Se `80 cm <= distanciaAtual < 90 cm` -> LED Vermelho acende.
        * **PERIGO MAX!:** Se `distanciaAtual < 80 cm` -> LED Vermelho acende e o Buzzer é ativado.
        * Nos demais casos, o buzzer é explicitamente desligado.
    * **Atualização do Display LCD:** A cada `intervaloDisplay` (definido em 2500ms), o LCD é limpo e alterna entre duas telas:
        * **Tela 0:** Exibe `Distancia: [valor] cm`.
        * **Tela 1:** Exibe `Nivel de alerta:` na primeira linha e a mensagem textual do alerta (`Normal`, `Cuidado`, `Perigo` ou `PERIGO MAX!`) na segunda linha.
    * Toda a temporização é gerenciada por `millis()` para evitar o uso de `delay()` bloqueante.

## 7. Como Usar e Testar

1.  **Montagem:** Monte o circuito conforme a seção 5.
2.  **Software:**
    * Abra o arquivo `.ino` na Arduino IDE.
    * Certifique-se de que a biblioteca `LiquidCrystal_I2C` está instalada (Vá em Ferramentas > Gerenciar Bibliotecas... e procure por "LiquidCrystal I2C" e instale uma compatível, como a de Frank de Brabander).
3.  **Upload:** Selecione a placa "Arduino Uno" (ou a que estiver usando) e a porta COM correta em Ferramentas, e então clique em "Carregar".
4.  **Teste:**
    * Após o upload, o LCD deve mostrar a mensagem de inicialização e depois começar a alternar as telas.
    * Aproxime e afaste um objeto (simulando a superfície da água) do sensor HC-SR04.
    * Observe as mudanças na distância exibida no LCD e como os LEDs e o buzzer respondem aos diferentes níveis de alerta definidos no código.
    * Você pode abrir o Monitor Serial (Ctrl+Shift+M) na Arduino IDE (com baud rate 9600) para ver os valores de distância e alerta sendo impressos para debug.

## 8. Autores

* Enzo Dourado - RM565585
* Matheus Victorio - RM566447
* Vinícius Lugli - RM564694

## 9. Links

* Wokwi: https://wokwi.com/projects/432680905520811009

* YouTube: https://youtu.be/FHBjba8tQSI

*(Global Solution FIAP - Edge Computing & IoT - [Ano/Semestre])*

---
