#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "ws2812.h"

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

// Definições dos pinos
#define LED_PIN_R 13
#define LED_PIN_G 11
#define LED_PIN_B 12
#define BTN_A_PIN 5
#define BTN_B_PIN 6
#define WS2812_PIN 7

// Variáveis globais para o display
uint8_t ssd[1024]; // Tamanho máximo do buffer para display 128x64
struct render_area frame_area;

// Variáveis globais para controle dos LEDs
volatile bool led_g_state = false;
volatile bool led_b_state = false;

// Função para tratamento de debounce
static uint32_t last_button_time = 0;
#define DEBOUNCE_DELAY 200

// Função de callback única para ambos os botões
void button_callback(uint gpio, uint32_t events)
{
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - last_button_time < DEBOUNCE_DELAY)
        return;

    last_button_time = current_time;

    if (gpio == BTN_A_PIN)
    {
        // Controle do LED Verde
        led_g_state = !led_g_state;
        gpio_put(LED_PIN_G, led_g_state);

        // Mensagem para o display
        memset(ssd, 0, ssd1306_buffer_length);
        char msg[20];
        sprintf(msg, "LED Verde: %s", led_g_state ? "ON" : "OFF");
        ssd1306_draw_string(ssd, 0, 0, msg);
        render_on_display(ssd, &frame_area);

        printf("LED Verde alterado para: %s\n", led_g_state ? "ON" : "OFF");
    }
    else if (gpio == BTN_B_PIN)
    {
        // Controle do LED Azul
        led_b_state = !led_b_state;
        gpio_put(LED_PIN_B, led_b_state);

        // Mensagem para o display
        memset(ssd, 0, ssd1306_buffer_length);
        char msg[20];
        sprintf(msg, "LED Azul: %s", led_b_state ? "ON" : "OFF");
        ssd1306_draw_string(ssd, 0, 0, msg);
        render_on_display(ssd, &frame_area);

        printf("LED Azul alterado para: %s\n", led_b_state ? "ON" : "OFF");
    }
}

int main()
{
    stdio_init_all(); // Inicializa os tipos stdio padrão presentes ligados ao binário

    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    // Configurar a área de renderização
    frame_area.start_column = 0;
    frame_area.end_column = ssd1306_width - 1;
    frame_area.start_page = 0;
    frame_area.end_page = ssd1306_n_pages - 1;
    calculate_render_area_buffer_length(&frame_area);

    // zera o display inteiro
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

restart:

    // Parte do código para exibir a mensagem no display (opcional: mudar ssd1306_height para 32 em ssd1306_i2c.h)
    // /**
    char *text[] = {
        "  Ola!   ",
        "  Bem Vindos   "};

    int y = 0;
    for (uint i = 0; i < count_of(text); i++)
    {
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
    //

    // Configuração dos LEDs RGB - Certifique-se que os LEDs estão desligados inicialmente
    gpio_init(LED_PIN_R);
    gpio_init(LED_PIN_G);
    gpio_init(LED_PIN_B);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);
    gpio_set_dir(LED_PIN_B, GPIO_OUT);
    gpio_put(LED_PIN_G, false); // Inicializa LED verde desligado
    gpio_put(LED_PIN_B, false); // Inicializa LED azul desligado

    // Configuração dos botões com pull-up
    gpio_init(BTN_A_PIN);
    gpio_init(BTN_B_PIN);
    gpio_set_dir(BTN_A_PIN, GPIO_IN);
    gpio_set_dir(BTN_B_PIN, GPIO_IN);
    gpio_pull_up(BTN_A_PIN);
    gpio_pull_up(BTN_B_PIN);

    // Configuração das interrupções com uma única função de callback
    gpio_set_irq_enabled_with_callback(BTN_A_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled(BTN_B_PIN, GPIO_IRQ_EDGE_FALL, true); // Note a diferença aqui

    // Inicialização do WS2812
    ws2812_init(WS2812_PIN);

    while (true)
    {
        // Leitura do Serial Monitor
        if (stdio_usb_connected())
        {
            int c = getchar_timeout_us(0);
            if (c != PICO_ERROR_TIMEOUT)
            {
                // Limpa o display
                memset(ssd, 0, ssd1306_buffer_length);

                // Exibe o caractere no display
                char str[2] = {c, '\0'};
                ssd1306_draw_string(ssd, 0, 0, str);
                render_on_display(ssd, &frame_area);

                // Se for um número, atualiza a matriz WS2812
                if (c >= '0' && c <= '9')
                {
                    int num = c - '0';
                    // Aqui você deve implementar a lógica para exibir o número na matriz WS2812
                    ws2812_show_number(num);
                }
            }
        }

        sleep_ms(10);
    }

    return 0;
}
