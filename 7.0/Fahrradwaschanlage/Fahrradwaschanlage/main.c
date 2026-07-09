/*
 * Fahrradwaschanlage.c
 *
 * Created: 06.03.2026 10:26:32
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"

#define MASKE_NOTAUS (1<<7)
#define MASKE_EINAUS (1<<0)
#define MASKE_FAHRRADSENSOR (1<<1)
#define MASKE_WASSER (1<<2)

#define MASKE_JOYSTICK (1<<2)

int main(void)
{

	typedef enum {NOTAUS,AUS,EIN,Programmwahl,Wartung,Premium,Intensiv,Schnell,Vorwaesche,Hauptwaesche,Spuelen,Trocknen,Fehler,Pause,Ventil_Test,Trockner_Test,Buersten_Test,Wasserpumpne_Test} zustand_t;
	zustand_t zustand = AUS;
	
	initBoard(0);
	char * states [] = {"Notaus      ","Aus         ","Ein         ","Programmwahl","Wartung     ","Premium     ","Intensiv    ","Schnell     ","Vorwaesche  ","Hauptwaesche","Spuelen     ","Trocknen    ","Fehler!","Pause","Ventil Test","Trockner Test","Buersten Test","Wasserpumpen Test"};
	char *choices [] = {"Premium :","Intensiv:","Schnell :","Back    :"};
	char *wartungen [] = {"Ventil Test      :","Trocknet Test    :","Buersten Test    :","Wasserpumpen Test:","Back             :"};
	
	uint8_t schalter = 0;
	uint8_t inNotAus = 0;
	uint8_t inEinAus = 0;
	uint8_t inFahrradsensor = 0;
	uint8_t inWasser = 0;
	
	uint64_t inZeit_ms = 0;
	uint64_t timer = 0;
	uint64_t vorwaesche = 0;
	uint64_t hauptwaesche = 0;
	uint64_t spuelen = 0;
	uint64_t trocknen = 0;
	
	uint16_t y = 0;
	uint16_t x = 0;
	uint8_t hoch_flag = 0;
	uint8_t runter_flag = 0;
	uint8_t position = 1;
	uint8_t position_old = 0;
	
	uint8_t joystick_alt = 0;
	uint8_t joystick_neu = 0;
	uint8_t joystick_flank = 0;
	uint8_t joystick_posflank = 0;
	uint8_t button = 0;
	uint8_t joystik_button = 0;
	
	uint8_t programmwahl_position = 0;
	
	uint64_t zwischenspeicher_vor = 0;
	uint64_t zwischenspeicher_haupt = 0;
	uint64_t zwischenspeicher_spu = 0;
	uint64_t zwischenspeicher_tro = 0;
	
    while (1){
//Eingabe
	schalter = PINC;
	joystik_button = PINE;
	inNotAus = (schalter & MASKE_NOTAUS)>0;
	inEinAus = (schalter & MASKE_EINAUS)>0;
	inFahrradsensor = (schalter & MASKE_FAHRRADSENSOR)>0;
	inWasser = (schalter & MASKE_WASSER)>0;
	
	inZeit_ms = getSystemTimeMs();
	
	joystick_alt = joystick_neu;
	joystick_neu = joystik_button;
	joystick_flank = joystick_alt ^ joystick_neu;
	joystick_posflank = joystick_flank & joystick_neu;
	button = (joystick_posflank & MASKE_JOYSTICK)>0;
	x = adcRead(ADC_10_JOYSTICK_X);
	y = adcRead(ADC_11_JOYSTICK_Y);
	if((y<300)&&(!runter_flag)&&(position<3)){
		position_old = position;
		position += 1;
		runter_flag = 1;
	}
	if((y>700)&&(!hoch_flag)&&(position>1)){
		position_old = position;
		position -= 1;
		hoch_flag = 1;
	}
	if((y>400)&&(y<500)){
		hoch_flag = 0;
		runter_flag = 0;
	}
	
//Verarbeitung
	switch(zustand){
		case NOTAUS://NOTAUS-------------------------------------------------------------------
			if(inNotAus){
				zustand = AUS;
			}
			lcdWriteText(1,0,"                    ");
			lcdWriteText(2,0,"                    ");
			lcdWriteText(3,0,"                    ");
		break;
		
		case AUS://AUS----------------------------------------------------------------------------
			if(!inNotAus){
				zustand = NOTAUS;
			}
			if(inEinAus){
				zustand = EIN;
			}
			lcdWriteText(1,0,"                    ");
			lcdWriteText(2,0,"                    ");
			lcdWriteText(3,0,"                    ");
		break;
		
		case EIN: //EIN-----------------------------------------------------------------------------
			if(!inEinAus){
				zustand = AUS;
			}
			if(!inNotAus){
				zustand = NOTAUS;
			}
			lcdWriteText(position,13,"x");
			lcdWriteText(position_old,13," ");
			if(inFahrradsensor){
				lcdWriteText(2,0,"Programmwahl:");
			}
			else{
				lcdWriteText(2,0,"             ");
			}
			lcdWriteText(1,0,"Wartung     :");
			lcdWriteText(3,0,"                    ");
			if(button&&(position==2)){
				zustand = Programmwahl;
				lcdWriteText(1,0,"                              ");
				lcdWriteText(2,0,"                              ");
				lcdWriteText(3,0,"                              ");
			}
			programmwahl_position = 0;
						if(button&&(position==1)){
							lcdWriteText(1,0,"                    ");
							lcdWriteText(2,0,"                              ");
							zustand = Wartung;
						}
		break;
		
		case Programmwahl://Programmwahl-----------------------------------------------------------------------
			if(!inFahrradsensor){
				zustand = EIN;
			}
			if(!inEinAus){
				zustand = AUS;
			}
			if(!inNotAus){
				zustand = NOTAUS;
			}
			lcdWriteText(1,0,choices[programmwahl_position]);
			lcdWriteText(2,0,choices[programmwahl_position+1]);
			lcdWriteText(3,0,choices[programmwahl_position+2]);
			
			lcdWriteText(position,9,"x");
			lcdWriteText(position_old,9," ");
			if((position==3)&&(y<300)&&(!programmwahl_position)&&(!runter_flag)){
				programmwahl_position += 1;
			}
			if((position==1)&&(y>700)&&(programmwahl_position)&&(!hoch_flag)){
				programmwahl_position -= 1;
			}
			
			if((position==3)&&button&&programmwahl_position){
				lcdWriteText(1,0,"                              ");
				lcdWriteText(2,0,"                              ");
				lcdWriteText(3,0,"                              ");
				zustand = EIN;
			}
			if((position==1)&&button&&(!programmwahl_position)){
				lcdWriteText(1,0,"                              ");
				lcdWriteText(2,0,"                              ");
				lcdWriteText(3,0,"                              ");
				zustand = Premium;
			}
			if((position==2)&&button&&(!programmwahl_position)){
				lcdWriteText(1,0,"                              ");
				lcdWriteText(2,0,"                              ");
				lcdWriteText(3,0,"                              ");
				zustand = Intensiv;
			}
			if((position==1)&&button&&(programmwahl_position)){
				lcdWriteText(1,0,"                              ");
				lcdWriteText(2,0,"                              ");
				lcdWriteText(3,0,"                              ");
				zustand = Intensiv;
			}
			if((position==3)&&button&&(!programmwahl_position)){
				lcdWriteText(1,0,"                              ");
				lcdWriteText(2,0,"                              ");
				lcdWriteText(3,0,"                              ");
				zustand = Schnell;
			}
			if((position==2)&&button&&(programmwahl_position)){
				lcdWriteText(1,0,"                              ");
				lcdWriteText(2,0,"                              ");
				lcdWriteText(3,0,"                              ");
				zustand = Schnell;
			}
		break;
		
		case Premium://-----------------------------------------------------------------------------------
			vorwaesche = 5000;
			hauptwaesche = 20000;
			spuelen = 10000;
			trocknen = 10000;
			timer = inZeit_ms;
			zustand = Vorwaesche;
		break;
		
		case Intensiv://-------------------------------------------------------------------------------------
			vorwaesche = 10000;
			hauptwaesche = 30000;
			spuelen = 15000;
			trocknen = 12000;
			timer = inZeit_ms;
			zustand = Vorwaesche;
		break;
		
		case Schnell://---------------------------------------------------------------------------------------
			hauptwaesche = 10000;
			spuelen = 5000;
			trocknen = 5000;
			timer = inZeit_ms;
			zustand = Hauptwaesche;
		break;
		
		case Vorwaesche://----------------------------------------------------------------------------------
		lcdWriteZahl(1,0,(vorwaesche-(inZeit_ms-timer))/1000+1,2,0);
		lcdWriteText(1,2,"s");
			if(((vorwaesche-(inZeit_ms-timer))>0)&&((vorwaesche-(inZeit_ms-timer))<10)){
				timer = inZeit_ms;
				zustand = Hauptwaesche;
			}
			if(!inNotAus){
				zustand = NOTAUS;
			}
			if((!inFahrradsensor)|(!inWasser)){
				zustand = Fehler;
				lcdWriteText(0,0,"                              ");
				lcdWriteText(1,0,"                              ");
			}
		lcdWriteText(3,0,"Abbrechen:");
		if(button&&(position==3)){
			zustand = EIN;
		}
			lcdWriteText(position,10,"x");
			lcdWriteText(position_old,10," ");
			lcdWriteText(2,0,"Pause    :");
		if((position==2)&&button){
			zustand = Pause;
		}
		break;
		
		case Hauptwaesche://-----------------------------------------------------------------------------
		lcdWriteZahl(1,0,(hauptwaesche-(inZeit_ms-timer))/1000+1,2,0);
		lcdWriteText(1,2,"s");
			if(((hauptwaesche-(inZeit_ms-timer))>0)&&((hauptwaesche-(inZeit_ms-timer))<10)){
				timer = inZeit_ms;
				zustand = Spuelen;
			}
			if(!inNotAus){
				zustand = NOTAUS;
			}
			if((!inFahrradsensor)|(!inWasser)){
				zustand = Fehler;
				lcdWriteText(0,0,"                              ");
				lcdWriteText(1,0,"                              ");
			}
			lcdWriteText(3,0,"Abbrechen:x");
		if(button){
			zustand = EIN;
		}
		break;
		
		case Spuelen://--------------------------------------------------------------------------------------
		lcdWriteZahl(1,0,(spuelen-(inZeit_ms-timer))/1000+1,2,0);
		lcdWriteText(1,2,"s");
			if(((spuelen-(inZeit_ms-timer))>0)&&((spuelen-(inZeit_ms-timer))<10)){
				timer = inZeit_ms;
				zustand = Trocknen;
			}
			if(!inNotAus){
				zustand = NOTAUS;
			}
			if((!inFahrradsensor)|(!inWasser)){
				zustand = Fehler;
				lcdWriteText(0,0,"                              ");
				lcdWriteText(1,0,"                              ");
			}
			lcdWriteText(3,0,"Abbrechen:x");
		if(button){
			zustand = EIN;
		}
		break;
		
		case Trocknen: //-----------------------------------------------------------------------------------
		lcdWriteZahl(1,0,(trocknen-(inZeit_ms-timer))/1000+1,2,0);
		lcdWriteText(1,2,"s");
			if(((trocknen-(inZeit_ms-timer))>0)&&((trocknen-(inZeit_ms-timer))<10)){
				zustand = EIN;
				lcdWriteText(1,0,"                              ");
				lcdWriteText(2,0,"                              ");
				lcdWriteText(3,0,"                              ");
				lcdWriteText(0,0,"                              ");
				zustand = EIN;
			}
			if(!inNotAus){
				zustand = NOTAUS;
			}
			if((!inFahrradsensor)|(!inWasser)){
				zustand = Fehler;
				lcdWriteText(0,0,"                              ");
				lcdWriteText(1,0,"                              ");
			}
			lcdWriteText(3,0,"Abbrechen:x");
		if(button){
			zustand = EIN;
		}
		break;
		
		case Fehler://Fehler-------------------------------------------------------------------------------
		lcdWriteText(3,0,"Back:x                   ");
			if(!inFahrradsensor){
				lcdWriteText(1,0,"Bycicle missing!!!!");
			}
			if(!inWasser){
				lcdWriteText(2,0,"No WATER!!!!");
			}
			if(button){
				zustand = EIN;
				lcdWriteText(1,0,"                              ");
				lcdWriteText(2,0,"                              ");
			}
		break;
		
		case Pause://----------------------------------------------------------------------------------------
			
		break;
		
		case Wartung://Wartung------------------------------------------------------------------------
			lcdWriteText(1,0,wartungen[programmwahl_position]);
			lcdWriteText(2,0,wartungen[programmwahl_position+1]);
			lcdWriteText(3,0,wartungen[programmwahl_position+2]);
			lcdWriteText(position,18,"x");
			lcdWriteText(position_old,18," ");
			if((position==3)&&(y<300)&&(!programmwahl_position)&&(!runter_flag)){
				programmwahl_position += 2;
				lcdWriteText(3,18," ");
				position = 0;
			}
			if((position==1)&&(y>700)&&(programmwahl_position)&&(!hoch_flag)){
				programmwahl_position -= 2;
				lcdWriteText(1,18," ");
				position = 2;
			}
			if((position==1)&&button&&(!programmwahl_position)){
				zustand = Ventil_Test;
			}
			if((position==2)&&button&&(!programmwahl_position)){
				zustand = Trockner_Test;
			}
			if((position==3)&&button&&(!programmwahl_position)){
				zustand = Buersten_Test;
			}
			if((position==1)&&button&&(programmwahl_position)){
				zustand = Buersten_Test;
			}
			if((position==2)&&button&&(programmwahl_position)){
				zustand = Wasserpumpne_Test;
			}
			if((position==3)&&button&&(programmwahl_position)){
				lcdWriteText(1,0,"                  ");
				lcdWriteText(2,0,"                  ");
				zustand = EIN;
			}
		break;
	}
//Ausgabe
	lcdWriteText(0,0,states[zustand]);
    }
}

