#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"
#define outA 18 // Saída do PWM

int reAlimentacao = 2000;  //Initial value for soft start
int integrador = 0;
int erro_1 = 0;
unsigned long tempo = 0;

void setup() {
  Serial.begin(115200);

  // Inicializa o GPIO para MCPWM
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, outA);

  // Configuração do MCPWM
  mcpwm_config_t pwm_config;
  pwm_config.frequency = 500; // Frequência de 0.5 kHz
  pwm_config.cmpr_a = 50.0;    // Ciclo de trabalho A em porcentagem
  pwm_config.cmpr_b = pwm_config.cmpr_a;    // Ciclo de trabalho B em porcentagem
  pwm_config.counter_mode = MCPWM_UP_COUNTER; // Modo de contador ascendente
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;   // Modo de duty cycle

  // Inicializa o MCPWM com a configuração fornecida
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);

  // Inicia o PWM
  mcpwm_start(MCPWM_UNIT_0, MCPWM_TIMER_0);
}

void loop() {
  if (micros() - tempo >= 200) { //Fixed time loop
    tempo = micros();
    reAlimentacao -= ((int)(reAlimentacao - analogReadMilliVolts(34))) >>4; // IIR filter of measure values
    int erro = 2500 - reAlimentacao;
    integrador += erro;
    if (integrador > (100 << 11)) integrador = 100 << 11;
    if (integrador < 1) integrador = 1;
    int derivativo = erro - erro_1;
    erro_1 = erro;
    int acao = (erro >> 8) + (integrador >> 11) + (derivativo >> 8);  // PID gain with fast code
    if (acao > 100) acao = 100;
    if (acao < 1) acao = 1;

    // Atualiza o ciclo de trabalho do MCPWM
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, acao);

    Serial.println(String(erro) + "," + String(acao));  //Print results

  }
}
