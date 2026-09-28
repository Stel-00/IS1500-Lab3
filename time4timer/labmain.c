/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */


/* Below functions are external and found in other files. */
extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);
extern int nextprime( int );

int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";

volatile int* timer_pointer = (volatile int*) 0x04000020;
int timeout_counter = 0;

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{}

/* Add your code here for initializing interrupts. */
void labinit(void) {
  //100 ms interval and 30MHz gives us 3000000 cycles
  //3000000 in hex: 0x2DC6C0

  //periodl register
  *(timer_pointer + 2) = 0xC6C0;

  //periodh register
  *(timer_pointer + 3) = 0x2D;

  //control register, set to start and cont
  *(timer_pointer + 1) = 0x6;
}


void set_leds(int led_mask) {
  //Create pointer to the memory segment of the led
  volatile int *led_pointer = (volatile int*) 0x04000000;

  //Dereference and set that value to the led mask
  *led_pointer = led_mask;
}

void set_displays(int display_number, int value) {

  if(display_number < 0 || display_number > 5) {
    return;
  }

  int display_adress = (display_number * 0x10) + 0x04000050;

  volatile int *display_pointer = (volatile int*) display_adress;

  switch (value){
    case (0):
      *display_pointer = 0x40;
      break;
    case (1):
      *display_pointer = 0x79;
      break;
    case (2):
      *display_pointer = 0x24;
      break;
    case (3):
      *display_pointer = 0x30;
      break;
    case (4):
      *display_pointer = 0x19;
      break;
    case (5):
      *display_pointer = 0x12;
      break;
    case (6):
      *display_pointer = 0x02;
      break;
    case (7):
      *display_pointer = 0x78;
      break;
    case (8):
      *display_pointer = 0x00;
      break;
    case (9):
      *display_pointer = 0x10;
      break;
    default:
      *display_pointer = 0xff;
      break;
  }

}

int get_sw(void) {
  volatile int* switch_pointer = (volatile int*) 0x04000010;

  return *switch_pointer & 0x3ff;
}

 int get_btn(void) {
  volatile int* button_pointer = (volatile int*) 0x040000d0;

  return *button_pointer & 0x1;
 }

 void time2display(int mytime){

  //Seconds
  set_displays(0, mytime & 0xf);
  set_displays(1, (mytime & 0xf0) >> 4);

  //Minutes
  set_displays(2, (mytime & 0xf00) >> 8);
  set_displays(3, (mytime & 0xf000) >> 12);

  //Hours
  set_displays(4, (mytime & 0xf0000) >> 16);
  set_displays(5, (mytime & 0xf00000) >> 20);

 }

 void check_button() {
  if(!get_btn()) {
    return;
  }

  int sw = get_sw();
  //Masks out the 8 least significant switches to give us the setting for either
  //seconds minutes or hours.
  int display = sw & 0x300;

  //Masks out the 4 most significant switches
  //so we get the value of the switches that are turned on
  int switch_value = sw & 0x3f;

  //We change the value to clock bcd format
  int switch_value_bcd_format = ((switch_value / 10) << 4) | (switch_value % 10);
  int mask = 0;

  switch(display) {
    //Update seconds
    case 0x100:
      mask = mytime & 0xffffff00;
      mytime = mask | switch_value_bcd_format;
      break;
    //Update minutes
    case 0x200:
      mask = mytime & 0xffff00ff;
      mytime = mask | (switch_value_bcd_format << 8);
      break;
    //Update hours
    case 0x300:
      mask = mytime & 0xff00ffff;
      mytime = mask | (switch_value_bcd_format << 16);
      break;

  }

 }

 int check_timeout() {
  //If no timeout, return
  if((*timer_pointer & 0x1) == 0) {
    return 0;
  }

  if(timeout_counter > 9) {
    timeout_counter = 0;
    *timer_pointer = *timer_pointer & 0xfffffff0;
    return 1;
  }
  
  //Reset TO bit
  timeout_counter++;
  *timer_pointer = *timer_pointer & 0xfffffff0;
  return 0;


 }

/* Your code goes into main as well as any needed functions. */
int main() {
  // Call labinit()
  labinit();

  //End-loop switch is 7 (to the right of the two most significant switches)
  //Switch needs to be flipped up for the program to run.
  while((get_sw() & 0x80) != 0) {
    if(check_timeout()) {
      time2string( textstring, mytime ); // Converts mytime to string
      time2display(mytime);
      display_string( textstring ); //Print out the string 'textstring'
      tick( &mytime );     // Ticks the clock once
    }
    check_button();


  } 
}


