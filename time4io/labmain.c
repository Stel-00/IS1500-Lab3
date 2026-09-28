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

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{}

/* Add your code here for initializing interrupts. */
void labinit(void)
{}


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


/* Your code goes into main as well as any needed functions. */
int main() {
  // Call labinit()
  labinit();

  int ledmask = 0;
  set_leds(ledmask);

  set_displays(0, 1);

  // Enter a forever loop
  while (ledmask < 0xf) {
    time2string( textstring, mytime ); // Converts mytime to string
    display_string( textstring ); //Print out the string 'textstring'
    delay( 1000 );          // Delays 1 sec (adjust this value)
    tick( &mytime );     // Ticks the clock once
    ledmask++;
    set_leds(ledmask);
    set_displays(0, ledmask);
    set_displays(2, ledmask);
    set_displays(4, ledmask);
    set_displays(5, ledmask);
  }
}


