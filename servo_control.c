#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Definições
#define SERVO_GPIO 22  // GPIO para o sinal PWM do servo
#define LED_GPIO 12     // GPIO do LED RGB
#define PWM_FREQ 50     // Frequência do PWM (50Hz, período de 20ms)
#define MIN_PULSE 500   // Pulso mínimo para 0° (500µs)
#define MID_PULSE 1470  // Pulso médio para 90° (1470µs)
#define MAX_PULSE 2400  // Pulso máximo para 180° (2400µs)
#define CLOCK_DIV 64    // Divisor de clock do PWM

// Função para configurar o PWM na GPIO desejada
void setup_pwm(uint gpio, uint32_t freq) {
    gpio_set_function(gpio, GPIO_FUNC_PWM); // Configura GPIO para PWM
    uint slice = pwm_gpio_to_slice_num(gpio); // Obtém o número do slice PWM
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, CLOCK_DIV); // Configura divisor de clock
    pwm_init(slice, &config, true); // Inicializa PWM
}

// Função para definir o ângulo do servo (conversão de tempo para duty cycle)
void set_servo_angle(uint gpio, uint pulse_width_us) {
    uint slice = pwm_gpio_to_slice_num(gpio);
    uint wrap = clock_get_hz(clk_sys) / (CLOCK_DIV * PWM_FREQ);
    uint level = (pulse_width_us * wrap) / 20000; // 20000µs = 20ms
    pwm_set_wrap(slice, wrap);
    pwm_set_gpio_level(gpio, level);
}

int main() {
    stdio_init_all();
    
    // Configuração do PWM para o servo
    setup_pwm(SERVO_GPIO);
    
    // Teste inicial: Ajustar servo para 0°, 90° e 180° com 5s de intervalo
    set_servo_angle(SERVO_GPIO, MIN_PULSE);
    sleep_ms(5000);
    
    set_servo_angle(SERVO_GPIO, MID_PULSE);
    sleep_ms(5000);
    
    set_servo_angle(SERVO_GPIO, MAX_PULSE);
    sleep_ms(5000);
    
    // Rotina de movimentação suave entre 0° e 180°
    while (1) {
        for (int pulse = MIN_PULSE; pulse <= MAX_PULSE; pulse += 5) {
            set_servo_angle(SERVO_GPIO, pulse);
            sleep_ms(10);
        }
        for (int pulse = MAX_PULSE; pulse >= MIN_PULSE; pulse -= 5) {
            set_servo_angle(SERVO_GPIO, pulse);
            sleep_ms(10);
        }
    }

    return 0;
}
