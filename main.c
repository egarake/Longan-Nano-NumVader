#include "lcd/lcd.h"

void Inp_init(void)
{
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
}

void Adc_init(void) 
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_0|GPIO_PIN_1);
    RCU_CFG0|=(0b10<<14)|(1<<28);
    rcu_periph_clock_enable(RCU_ADC0);
    ADC_CTL1(ADC0)|=ADC_CTL1_ADCON;
}

void IO_init(void)
{
	Inp_init();	// inport init
	Adc_init();	// A/D init
	Lcd_Init();	// LCD init
}

FlagStatus Get_BOOT0SW(void)
{
	return(gpio_input_bit_get(GPIOA, GPIO_PIN_8));
}

uint16_t Get_adc(int ch)
{
    ADC_RSQ2(ADC0)=0;
    ADC_RSQ2(ADC0)=ch;
    ADC_CTL1(ADC0)|=ADC_CTL1_ADCON;
    while(!(ADC_STAT(ADC0)&ADC_STAT_EOC));
    uint16_t ret=ADC_RDATA(ADC0)&0xFFFF;
    ADC_STAT(ADC0)&=~ADC_STAT_EOC;
    return(ret);
}

/*************************************************************************************
 0123456789012345678
0
1
2
3
4
**************************************************************************************/
int num;
#define num_max 10
#define buf_max 16
int num_buf[buf_max]={11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11};
u8 num_char[]="0123456789n ";
uint32_t wait;
int stime;
int ctime;
int hit1,hit2;
int score,miss;

void Disp_score(void){
		LCD_ShowString( 6*8, 3*16, (u8*)"Faile", RED);
		LCD_ShowChar(  12*8, 3*16, '-',       0, RED);
		LCD_ShowNum(   13*8, 3*16, miss,      5, RED);
		LCD_ShowString( 6*8, 1*16, (u8*)"Score", GREEN);
		LCD_ShowChar(  12*8, 1*16, ' ',       0, GREEN);
		LCD_ShowNum(   13*8, 1*16, score,     5, GREEN);
}

void Set__buf(void){
int i;
	for(i=0; i<buf_max-1; i++){num_buf[buf_max-1-i]=num_buf[buf_max-2-i];}
	if(hit1+hit2==10){	num_buf[0]=num_max;	hit1=hit2=0;}
	else{				num_buf[0]=rand()%num_max;}
}

void Disp_buf(void){
int i;
	for(i=0; i<buf_max; i++){LCD_ShowChar(17*8-i*8, 2*16, num_char[num_buf[i]], 0, WHITE );}
}

void Sele_num(void){
	num=(Get_adc(0)/(4096/32))%(num_max+1);
}

void Disp_num(void){
	LCD_ShowChar(1*8, 2*16, num_char[num], 0, WHITE );
	LCD_ShowChar(2*8, 2*16, ':',           1, WHITE );
}

void Chek_hit(void){
int i, c;
	for(c=buf_max; c>=0; --c){
		if(num_buf[c]==num){
			if(num==num_max){	hit1=hit2=0;			score+=10;}
			else{				hit2=hit1;	hit1=num;	score+= 1;}
			Disp_score();
			LCD_ShowChar(17*8-c*8, 2*16, 'X', 1, RED );
			for(i=c; i<buf_max-1; i++){num_buf[i]=num_buf[i+1];}
			if(stime>100){stime-=1;}
			delay_1ms(100);
			Disp_buf();
			return;
		}
	}
	miss+=1;
	Disp_score();
	LCD_ShowChar(1*8, 2*16, 'X', 1, RED );
	delay_1ms(100);
}

int Chek_end(void){
int i;
	if(num_buf[buf_max-1]!=11){
		for(i=0; i<buf_max; i++){
		LCD_ShowChar(17*8-i*8, 2*16, num_char[num_buf[i]], 0, RED );}
		LCD_ShowChar( 1*8,     2*16, num_char[num],        0, RED );
		LCD_ShowChar( 2*8,     2*16, 'X',                  1, RED );
		return 0;
	}
	return 1;
}

void Set__par(void){
int i;
	for(i=0; i<buf_max; i++){num_buf[i]=num_max+1;}
	wait=10;
	ctime=0;	stime=250;
	score=0;	miss=0;
}


/* Longan Nano NumVader */
int main( void ) 
{
int hit=1;
	IO_init();	// IO init

	while(1){
		BACK_COLOR=BLACK;
		LCD_Clear(BLACK);
		LCD_ShowString( 0*8, 0*16, (u8*)"Calculator Invader ", WHITE);
		LCD_ShowString( 0*8, 4*16, (u8*)"  www.egarake.work ", BLUE);
		Set__par();
		Set__buf();
		Disp_buf();
		Disp_score();
		do{
			Sele_num();
			Disp_num();
			if((++ctime%stime)==0){
				Set__buf();
				Disp_buf();
			}
			if(hit){if( Get_BOOT0SW()){hit=0; Chek_hit();}}
			else{	if(!Get_BOOT0SW()){hit=1;}}
			delay_1ms(wait);
		}while(Chek_end());
		while( Get_BOOT0SW());
		while(!Get_BOOT0SW());
	}
}
