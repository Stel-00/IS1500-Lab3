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


/* Your code goes into main as well as any needed functions. */
int main() {
  // Call labinit()
  labinit();

  int ledmask = 0;
  set_leds(ledmask);

  // Enter a forever loop
  while (ledmask < 0xf) {
    time2string( textstring, mytime ); // Converts mytime to string
    display_string( textstring ); //Print out the string 'textstring'
    delay( 1000 );          // Delays 1 sec (adjust this value)
    tick( &mytime );     // Ticks the clock once
    ledmask++;
    set_leds(ledmask);
  }
}


