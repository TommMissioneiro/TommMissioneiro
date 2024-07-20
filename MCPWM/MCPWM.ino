#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"
float i = 0;
#define outA 18 // Saída do PWM

void setup() {
  // Inicializa o GPIO para MCPWM
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, outA);

  // Configuração do MCPWM
  mcpwm_config_t pwm_config;
  pwm_config.frequency = 2000; // Frequência
  pwm_config.cmpr_a = 80.1;     // Ciclo de trabalho A em porcentagem
  pwm_config.cmpr_b = pwm_config.cmpr_a;     // Ciclo de trabalho do complementar em porcentagem
  pwm_config.counter_mode = MCPWM_UP_COUNTER; // Modo de contador ascendente
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;   // Modo de duty cycle

  // Inicializa o MCPWM com a configuração fornecida
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);

  // Configura o deadtime
  //mcpwm_deadtime_enable(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_ACTIVE_HIGH_COMPLIMENT_MODE, 1000, 1000);

  // Configura o MCPWM para sair de baixa (opcional, dependendo da necessidade)
  //mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);
  mcpwm_start(MCPWM_UNIT_0, MCPWM_TIMER_0);
}


void loop() {
  // Loop vazio, pois a configuração do PWM é feita no setup
  i = i + 0.1;
  if (i > 99) i = 0;
  mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, i);
  delay(100);
}
