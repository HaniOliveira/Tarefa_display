# Projeto Display OLED com Raspberry Pi Pico

Este projeto implementa um sistema interativo usando Raspberry Pi Pico com display OLED, LEDs RGB, matriz WS2812 e botões.

## Funcionalidades

- Display OLED SSD1306 para exibição de mensagens
- Controle de LEDs RGB através de botões
- Matriz WS2812 5x5 para exibição de números (0-9)
- Interface serial para entrada de caracteres e números

## Hardware Necessário

- Raspberry Pi Pico
- Display OLED SSD1306 (I2C)
- 3 LEDs RGB
- 2 Botões
- Matriz WS2812 5x5
- Resistores e jumpers necessários

## Conexões

### Display OLED

- SDA: GPIO 14
- SCL: GPIO 15

### LEDs RGB

- LED Vermelho: GPIO 13
- LED Verde: GPIO 11
- LED Azul: GPIO 12

### Botões

- Botão A: GPIO 5
- Botão B: GPIO 6

### Matriz WS2812

- Data In: GPIO 7

## Como Usar

1. **Controle dos LEDs**

   - Pressione o Botão A para alternar o LED Verde
   - Pressione o Botão B para alternar o LED Azul
   - O estado atual dos LEDs é mostrado no display OLED

2. **Entrada Serial**
   - Conecte via USB
   - Digite caracteres para exibir no display OLED
   - Digite números (0-9) para exibir na matriz WS2812

## Compilação e Instalação

1. Configure o ambiente SDK do Raspberry Pi Pico
2. Clone este repositório
3. Execute:
