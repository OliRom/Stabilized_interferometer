// #include "Pid.h"
// #include "CircularList.h"
// #include "PosCounter.h"
#include "Ims400cc.h"

#define motor_p_pin 5
#define motor_n_pin 6
#define I_sense_pin 4
#define phase_A_pin 18
#define phase_B_pin 19
#define index_pulse_pin 7
#define eor_p_pin 2
#define eor_n_pin 3
#define meca_0_pin 0
#define moving_led_pin 8
#define pid_delay 10

Ims400cc table(motor_p_pin, motor_n_pin, I_sense_pin, phase_A_pin, phase_B_pin, index_pulse_pin, eor_p_pin, eor_n_pin, meca_0_pin, moving_led_pin, pid_delay, false);

byte data = 0x00;

void index_pulse_interrupt(){table.phase_change();}
void eor_interrupt(){table.stop_interruption();}
// void mecanical_zero_interrupt(){table.zero_interrupt();}  // Le signal du zéro mécanique ne fonctionne pas.

#define START_TRANSMIT 0x01

void send_data(String commande){Serial.print(char(START_TRANSMIT) + commande + '\n');}
void serial_search_for(byte code){while ((Serial.read() != code) and (Serial.available() > 0)){}}

void setup() {
  Serial.begin(9600);
  delay(5000);

  attachInterrupt(digitalPinToInterrupt(table.get_I_pin()), index_pulse_interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(table.get_interrupt_pin('p')), eor_interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(table.get_interrupt_pin('n')), eor_interrupt, FALLING);
  //attachInterrupt(digitalPinToInterrupt(table.get_interrupt_pin('0')), mecanical_zero_interrupt, CHANGE);

  Serial.println("Commencement");

  table.init_pid(1e-4, 5e-7, 0.0025, 100);
  table.set_pwm_prescaler('m');
  table.set_step_conversion(1.25);  // 4000 pts / 5 mm
}

void loop() {
  if (Serial.available() > 0){
    serial_search_for(START_TRANSMIT);
    data = Serial.read();

    switch (data){
      case '@':  // go_to(pos, precision)
        {float pos = Serial.parseFloat();
        serial_search_for(',');
        float incertitude = Serial.parseFloat();

        table.go_to(pos, incertitude);
        send_data(String(' ') + String(table.get_position(2)));
        break;}

      case '?':  // get_position()
        send_data(String('!') + String(table.get_position(2)));
        break;

      case '[':  // get_max_pos()
        send_data(String(']') + String(table.get_max_pos()));
        break;

      case '(':  // calibrate()
        table.calibrate();
        send_data(String(')'));
        break;
    }
    serial_search_for('\n');
  }

  /*Serial.print(digitalRead(table.get_interrupt_pin('p')));
  Serial.print(" ");
  Serial.print(digitalRead(table.get_interrupt_pin('n')));
  Serial.print(" ");
  Serial.print(digitalRead(table.get_interrupt_pin('0')));
  Serial.print(" ");
  Serial.println(digitalRead(table.get_interrupt_pin('i')));
  delay(1000);*/
}
