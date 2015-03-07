#include "MAKERPad.h"

#define FREDADAM

#ifdef IAN
int Button[][4] =  {{0, 835, 842}, // button 1
{1, 730, 740}, // button 2
{2, 600, 610}, // button 3
{3, 310, 324}, // button 4
{0, 892, 1000}, // button 1 + button 2
{1, 892, 1000}, // button 1 + button 2
{0, 872, 880}, // button 1 + button 3
{2, 872, 880}, // button 1 + button 3
{0, 848, 855}, // button 1 + button 4
{3, 848, 855}, // button 1 + button 4
{1, 813, 823}, // button 2 + button 3
{2, 813, 823}, // button 2 + button 3
{1, 765, 773}, // button 2 + button 4
{3, 765, 773}, // button 2 + button 4
{2, 667, 675}, // button 3 + button 4
{3, 667, 675}}; // button 3 + button 4
#endif
#ifdef FREDADAM
int Button[][4] =  {{3, 835, 842}, // button 1
{2, 730, 740}, // button 2
{1, 607, 617}, // button 3
{0, 310, 324}, // button 4

{2, 892, 1001}, // button 2 + button 4
{3, 892, 1001}, // button 2 + button 4

{1, 872, 881}, // button 3 + button 4
{3, 872, 881}, // button 3 + button 4

{0, 848, 855}, // button 1 + button 4
{3, 848, 855}, // button 1 + button 4

{1, 813, 823}, // button 2 + button 3
{2, 813, 825}, // button 2 + button 3

{0, 765, 773}, // button 1 + button 2
{2, 765, 773}, // button 1 + button 2

{0, 667, 679}, // button 1 + button 3
{1, 667, 679}}; // button 1 + button 3
#endif

volatile charliePin leds[20] = {{ 4 , 0 },{ 3 , 0 },{ 2 , 0 },{ 1 , 0 },
{ 0 , 1 },{ 4 , 1 },{ 3 , 1 },{ 2 , 1 },
{ 1 , 2 },{ 0 , 2 },{ 4 , 2 },{ 3 , 2 },
{ 2 , 3 },{ 1 , 3 },{ 0 , 3 },{ 4 , 3 },
{ 2 , 4 },{ 1 , 4 },{ 0 , 4 },{ 3 , 4 }};

int buttons[5] = {0,0,0,0,0};

int down[5][2] = {{0,0},{0,0},{0,0},{0,0},{0,0}};
int oneshot[20];

volatile boolean display[20];

volatile boolean show = false;

volatile int di = 0;

volatile byte pins[5] = {5,4,3,2,1};


MAKERPad::MAKERPad(void) {

  for(int i = 0; i < 5*4; i++) {
    display[i] = false;
  }
  di = 2;

  pinMode( 13 , INPUT);
  pinMode( 12 , INPUT);
  pinMode( 11 , INPUT);
  pinMode( 10 , INPUT);
  pinMode( 9 , INPUT);


  cli();          // disable global interrupts
  TIMSK1 = (1<<TOIE1);
  // set timer0 counter initial value to 0
  TCNT1=35624;


  // start timer0 with /1024 prescaler
  TCCR1B |= (1 << CS10);
  sei();          // enable global interrupts



  sei();
  interrupts();             // enable all interrupts
}

void MAKERPad::Set(int num) {
  display[num] = true;
}

void MAKERPad::Clear(int num) {
  display[num] = false;
}

void MAKERPad::Clear() {
  for(int i = 0; i < 5*4; i++) {
    display[i] = false;
  }
}

void MAKERPad::Toggle(int num) {
  display[num] = !display[num];
}

void MAKERPad::ClearRow(int num) {
  for(int i  = 0; i < 4; i++)
  display[num*4+i] = false;
}
boolean MAKERPad::CheckButtonPressed(int num){
  if(oneshot[num]) {
    oneshot[num] = false;
    return true;

  } else {
    return oneshot[num];
  }
}

boolean MAKERPad::CheckButtonDown(int num){

  return oneshot[num];
}

void MAKERPad::UpdateButtons() {
  buttons[0] =  analogRead(0);
  buttons[1] =  analogRead(1);
  buttons[2] =  analogRead(2);
  buttons[3] =  analogRead(3);
  buttons[4] =  analogRead(4);

  for(int j = 0; j < 5; j++) {
    if(buttons[j] >= down[j][0] && buttons[j] <= down[j][1]) {

    } else {
      down[j][0] = 0;
      down[j][1] = 0;
      for(int k = 0; k < 4;k++) oneshot[j*4+k] = false;
      for(int i = 0; i <= 16; i++)
      {
        if(buttons[j] >= Button[i][1] && buttons[j] <= Button[i][2])
        {


          int label = Button[i][0];

          down[j][0] = Button[i][1];
          down[j][1] = Button[i][2];

          oneshot[j*4+label] = true;

        }
      }
    }
  }
}

ISR(TIMER1_OVF_vect)
{
  TCNT1 = 1000;
  TCCR1B = (1<<CS10);
  DDRB = DDRB & ~B00111110;
  PORTB = PORTB & 0xFF80;

  if(display[di]) {
    DDRB = DDRB | (1 << pins[leds[di].vcc]) | (1 << pins[leds[di].gnd]);
    PORTB = PORTB | (1 << pins[leds[di].vcc]);
  }

  di++;
  if(di >= 20) di = 0;

}
