#include <12f675.h>
//Compilado con ccs y MPLAB
#FUSES NOWDT, INTRC_IO, NOCPD, NOPROTECT, NOMCLR, NOPUT, NOBROWNOUT, BANDGAP_HIGH
#use delay(clock=4000000)

#define maximo	6						//---CANTIDAD DE MODOS-----

void bl_normal(void);					//Modo rormal
void bl_ahorro(void);					//Modo ahorro
void bl_medio(void);					//Sin implementar
void bl_burst_solo(void);				//10 Segundos a Full pero para evitar recursion
void sos(void);							//Modo SOS
void rojo(void);						//Rojo continuo
void estrobo_bl(void);					//Blanco	estroboscopico
void estrobo_rojo(void);				//Rojo 		estroboscopico
void modo_sel(void);					//Funcion para seleccionar modo + 1segundo pulsado resetea +2segundos pulsado Burst
void modo_reset(void);					//Resetea el modo a apagado
void punto(void);						//Para funcion SOS	
void raya(void);						//Para funcion SOS
void apago(void);						//Apago los led por las dudas


int j,cont,flag,modo=0,i=0;

void main()
{	
	#byte OSCCAL = 0x80;
	set_tris_a(0b11000001);
	setup_comparator( NC_NC_NC_NC );
	setup_adc_ports( NO_ANALOGS );
	setup_adc( ADC_OFF );
	
		
	#define GP0 PIN_A0
	#define GP1 PIN_A1
	#define GP2 PIN_A2
	#define GP3 PIN_A3
	#define GP4 PIN_A4
	#define GP5 PIN_A5
					
	output_low(GP1);
	output_low(GP3);
	output_low(GP4);
	output_low(GP5);
	
	for(;;){				//Bucle infinito
		
		modo_sel();
		
		switch (modo)
		{
			case 0:
			apago();
			break;	

			case 1:
			apago();
			bl_normal();
			break;
				
			case 2:
			apago();
			bl_ahorro();
			break;
	
			case 3:
			apago();
			rojo();
			break;

			case 4:
			apago();
			estrobo_bl();
			break;
			
			case 5:
			apago();
			estrobo_rojo();
			break;

			case 6:
			apago();			
			sos();
			break;

			default:
			apago();
			delay_ms(500);
			output_high(GP1);
			delay_ms(500);
			output_low(GP1);
			break;


		}





	}
}


//_________FUNCIONES_______
void bl_normal()					//NORMAL
{	output_high(GP1);
	delay_ms(1);
	output_low(GP1);
	delay_ms(1);
	
return;
}

void bl_ahorro(void)				//AHORRO
{	output_high(GP1);
	delay_ms(3);
	output_low(GP1);
	delay_ms(12);
return;
}

void bl_medio(void)				//MEDIO
{
}

void bl_burst_solo(void)		
{	output_high(GP1);		
	for(j=0;j<=16;j++)
	{
			delay_ms(250);
			if(input(GP0)==0)		//controlo el pulsador de modo para resetear solamente
				modo_reset();
					
	}
								//terminan los 10 seg y se reinicia el modo
return;
}


void rojo(void)						//ROJO
{	output_high(GP2);
	delay_ms(1);
	output_low(GP2);
	delay_ms(1);
}

void estrobo_bl(void)					//ESTROBOSCÓPICO BLANCO
{	do{	output_high(GP1);
		if(input(GP0)==0)			//controlo el pulsador de modo
				{flag=1;
				modo_sel();
				}
			else
			flag=0;
	if(flag==1)
	break;	

	delay_ms(333);
	output_low(GP1);

	if(input(GP0)==0)			//controlo el pulsador de modo
				{flag=1;
				modo_sel();
				}
			else
			flag=0;
	if(flag==1)
	break;	
	delay_ms(333);
	}while(!(flag==1));	

}

void estrobo_rojo(void)					//ESTROBOSCÓPICO ROJO
{	do{	output_high(GP2);
		if(input(GP0)==0)			//controlo el pulsador de modo
				{flag=1;
				modo_sel();
				}
			else
			flag=0;
	if(flag==1)
	break;	

	delay_ms(333);
	output_low(GP2);

	if(input(GP0)==0)			//controlo el pulsador de modo
				{flag=1;
				modo_sel();
				}
			else
			flag=0;
	if(flag==1)
	break;	
	delay_ms(333);
	}while(!(flag==1));	

}

void sos(void)
{	
	do{
		punto();
		punto();
		punto();
		raya();
		raya();
		raya();
		punto();
		punto();
		punto();
		delay_ms(250);
	if(input(GP0)==0)			//controlo el pulsador de modo
				{flag=1;
				modo_reset();
				}
			else
			flag=0;
	delay_ms(250);
	if(flag==1)
	break;	
	}while(!(flag==1));
	modo_reset();
return;

}

void punto(void){
output_high(GP2);
		if(input(GP0)==0)			//controlo el pulsador de modo
				{flag=1;
				modo_reset();
				}
			else
			flag=0;
	if(flag==1)
	break;	

	delay_ms(100);
	output_low(GP2);
	delay_ms(50);
return;

}
void raya(void){
output_high(GP2);
		if(input(GP0)==0)			//controlo el pulsador de modo
				{flag=1;
				modo_reset();
				}
			else
			flag=0;
	if(flag==1)
	break;	

	delay_ms(200);
	output_low(GP2);
	delay_ms(50);
return;
}




void modo_sel()						//SELECCION DE MODO
{
if(input(GP0)==0)
		{if(modo<maximo)
		modo++;
		else
		modo=0;
		
		i=0;
		do{
		i++;
		delay_ms(100);
		}while(input(GP0)==0);
		}
		if(i>10)					//si aprieto mas de 1 segundo se resetea el modo
		modo_reset();
	
		if(modo==0&&i>=20)			//si aprieto mas de 2 segundo se pone el modo burst 
		{	bl_burst_solo();			//pero tiene que estar reseteado previamente el modo
			modo=1;
			i=0;
		}			
return;
}

void modo_reset()					//Pongo el modo en cero oséa apagado
{
		modo=0;
		apago();
return;
}

void apago(void){
		output_low(GP2);
		output_low(GP1);
		return;
}
