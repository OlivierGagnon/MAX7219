#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>

//Set the port defines
#define MAX7219_DDR		DDRD
#define MAX7219_PORT	PORTD
#define MAX7219_PIN_DIN	PIND0	//data pin
#define MAX7219_PIN_CS	PIND1	//load pin
#define MAX7219_PIN_CLK	PIND2	//clock pin

//Addresses in HEX mode
#define COL0	0x01
#define COL1	0x02
#define COL2	0x03
#define COL3	0x04
#define COL4	0x05
#define COL5	0x06
#define COL6	0x07
#define COL7	0x08
#define DECODE_MODE	0x09
#define INTENSITY	0x0a
#define SCAN_LIMIT	0x0b
#define SHUTDOWN	0x0c
#define TEST_DISPLAY	0x0f

//Rows in HEX mode
#define ROW0	0x01
#define ROW1	0x02
#define ROW2	0x04
#define ROW3	0x08
#define ROW4	0x10
#define ROW5	0x20
#define ROW6	0x40
#define ROW7	0x80

//Function prototypes
void MAX7219_CMD(int, int);
void MAX7219_CLEAR(void);
//void MAX7219_SET_COLROW(int, int);
void MAX7219_SET_ROWS(int *);

int main(void)
{
	//Set 3 ports as output
	MAX7219_DDR |= (1<<MAX7219_PIN_DIN)|(1<<MAX7219_PIN_CS)|(1<<MAX7219_PIN_CLK);
	MAX7219_CMD(SHUTDOWN,0x01); //Enable the display
	MAX7219_CMD(SCAN_LIMIT, 0x07); //Display digits 0 1 2 3 4 5 6 7
	//MAX7219_CMD(DECODE_MODE, 0x00); //Decode mode, keep default
	//MAX7219_CMD(INTENSITY, 0x00); //Intensity, keep default for now
	//MAX7219_CMD(TEST_DISPLAY, 0x00); //Test display OFF (1 is on)
	MAX7219_CLEAR();
	while(1)
	{
		
		int pattern1[8]={0x01,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x01};
		int pattern2[8]={0x01,0x0F,0x0F,0x3F,0x3F,0x0F,0x0F,0x01};
		int pattern3[8]={0x01,0x0F,0x0F,0xFF,0xFF,0x0F,0x0F,0x01};

		MAX7219_SET_ROWS(pattern1);
		_delay_ms(300);
		MAX7219_SET_ROWS(pattern2);
		_delay_ms(150);
		MAX7219_SET_ROWS(pattern3);
		_delay_ms(500);
		
	}
}

/*
void MAX7219_SET_COLROW(int col, int row)
{
	int i;
	for (i=1;i<9;i++)
	{
		if (col & 0x80)
		{
			MAX7219_CMD(i, row);
		}
		col <<= 1;
	}
}
*/

void MAX7219_SET_ROWS(int rows[8])
{
	MAX7219_CMD(COL0, rows[7]);
	MAX7219_CMD(COL1, rows[6]);
	MAX7219_CMD(COL2, rows[5]);
	MAX7219_CMD(COL3, rows[4]);
	MAX7219_CMD(COL4, rows[3]);
	MAX7219_CMD(COL5, rows[2]);
	MAX7219_CMD(COL6, rows[1]);
	MAX7219_CMD(COL7, rows[0]);
}

void MAX7219_CLEAR(void)
{
	MAX7219_CMD(COL0, 0x00);
	MAX7219_CMD(COL1, 0x00);
	MAX7219_CMD(COL2, 0x00);
	MAX7219_CMD(COL3, 0x00);
	MAX7219_CMD(COL4, 0x00);
	MAX7219_CMD(COL5, 0x00);
	MAX7219_CMD(COL6, 0x00);
	MAX7219_CMD(COL7, 0x00);
}

//Send a command to the max7219
void MAX7219_CMD(int address, int data)
{
	//address = 0000 + address;
	int i;
	MAX7219_PORT &= ~(1<<MAX7219_PIN_CS);	//CS low
	//send address bit. 4 bits don't care + 4 bits for address
	for (i=0;i<8;i++)
	{
		if (address & 0x80)
			MAX7219_PORT |= (1<<MAX7219_PIN_DIN);	//data pin high
		else
			MAX7219_PORT &= ~(1<<MAX7219_PIN_DIN);	//data pin low
		MAX7219_PORT |= (1<<MAX7219_PIN_CLK);	//clock high
		MAX7219_PORT &= ~(1<<MAX7219_PIN_CLK);	//clock low
		//shift byte right
		address <<= 1;
	}
	//byte1 sent
	//send data bit. 8 bits for data
	for (i=0;i<8;i++)
	{
		if (data & 0x80)
			MAX7219_PORT |= (1<<MAX7219_PIN_DIN);	//data pin high
		else
			MAX7219_PORT &= ~(1<<MAX7219_PIN_DIN);	//data pin low
		MAX7219_PORT |= (1<<MAX7219_PIN_CLK);	//clock high
		MAX7219_PORT &= ~(1<<MAX7219_PIN_CLK);	//clock low
		//shift byte right
		data <<= 1;
	}
	//byte2 sent
	//pull CS high
	MAX7219_PORT |= (1<<MAX7219_PIN_CS);	//CS high
}