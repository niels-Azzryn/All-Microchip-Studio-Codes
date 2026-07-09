/*	
 * Kueche.c
 *
 * Created: 18.06.2026 14:37:44
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"


int main(void)
{
	initBoard(0);
	//variabel
		//states
		uint8_t machine_state = 0;
		uint8_t fridge_state = 0;
		//player inventory 
		uint8_t player_inventory1 = 0;
		uint8_t player_inventory2 = 0;
		//Fridge
		uint8_t fridge_door = 0;
		uint8_t fridge_lamp = 0;
		uint8_t fridge_entry = 0;
		uint8_t fridge_blink_flag = 0;
		uint8_t fridge_temp = 5;
		uint8_t fridge_temp_einstellung = 5;
		uint8_t fridge_inventory_slot1 = 1;
		uint8_t fridge_inventory_slot2 = 0;
		uint8_t fridge_inventory_slot3 = 0;
		uint8_t fridge_inventory_slot4 = 0;
		uint8_t fridge_inventory_slot5 = 0;
		uint8_t fridge_door_winkel = 0;
		uint16_t fridge_door_poti_winkel = 0;
		//Joystick
		uint8_t joystick_button = 0;
		uint8_t joystick_alt = 0;
		uint8_t joystick_neu = 0;
		uint8_t joystick_flanke = 0;
		uint8_t joystick_pos_flanke = 0;
		uint16_t joystick_y = 0;
		uint8_t joystick_up_down = 0;
		uint8_t joystick_up_down_alt = 0;
		uint8_t joystick_up_down_neu = 0;
		uint8_t joystick_up_down_flanke = 0;
		uint8_t joystick_up_pos_flanke = 0;
		uint8_t joystick_down_pos_flanke = 0;
		uint16_t joystick_x = 0;
		uint8_t joystick_left_right = 0;
		uint8_t joystick_left_right_alt = 0;
		uint8_t joystick_left_right_neu = 0;
		uint8_t joystick_left_right_flanke = 0;
		uint8_t joystick_left_pos_flanke = 0;
		uint8_t	joystick_right_pos_flanke = 0;
		//buttons
		uint8_t buttons = 0;
		uint8_t buttons_alt = 0;
		uint8_t buttons_neu = 0;
		uint8_t buttons_flanke = 0;
		uint8_t back_button = 0;
		//schalter
		uint8_t schalter = 0;
		//choice
		uint8_t position = 0;
		//timer
		uint64_t inZeit_ms = 0;
		uint32_t timer = 0;
		uint32_t timer2 = 0;
		
	//Arrays
		char * machines[] = {"Kitchen             ","Fridge              ","Oven                ","Sink              ","Cofemachine         ","Kettle              ","Microwave           ","Nuclear Reactor     ",
										"Lamp                ","Robot               ","Grill               ","Smoker              ","Airfryer            ","Deep Fryer          ","Garbage Can         ","Inventory           ",
										"Fridge Inventory","Temperatur","Freezer"};
		char * machine_choices[] = {"Fridge              ","Oven                ","Sink              ","Cofemachine         ","Kettle              ","Microwave           ","Nuclear Reactor     ","Lamp                "
													,"Robot               ","Grill               ","Smoker              ","Airfryer            ","Deep Fryer          ","Garbage Can         ","Inventory           "};
		char * fridge[] = {"Fridge Inventory    ","Temperature         ","Freezer             "};
		char * fridge_inventory[] = {"Slot 1              ","Slot 2","Slot 3","Slot 4","Slot 5"};
		char * items[] = {"Empty","Bread","Orange Juice","Steak"};
    while (1) {
//potentiometer
	fridge_door_poti_winkel = adcRead(ADC_08_POTI_1);
	fridge_door_winkel = (uint16_t) (fridge_door_poti_winkel * 90 /1023);
//Joystick
	//taster Flanke
	joystick_button = (PINE & 0b00000100)>0;
	joystick_alt = joystick_neu;
	joystick_neu = joystick_button;
	joystick_flanke = joystick_alt ^ joystick_neu;
	joystick_pos_flanke = joystick_flanke & joystick_neu;
	//adc einlesung
	joystick_y = adcRead(ADC_11_JOYSTICK_Y);
	joystick_x = adcRead(ADC_10_JOYSTICK_X);
	//up_down_nothing
	if(joystick_y>800){
		joystick_up_down = 2;
	}
	else if(joystick_y<200){
		joystick_up_down = 1;
	}
	else{
		joystick_up_down = 0;
	}
	//left_right_nothing
	if(joystick_x>800){
		joystick_left_right = 2;
	}
	else if(joystick_x<200){
		joystick_left_right = 1;
	}
	else{
		joystick_left_right = 0;
	}
	//flanke up_down
	joystick_up_down_alt = joystick_up_down_neu;
	joystick_up_down_neu = joystick_up_down;
	joystick_up_down_flanke = joystick_up_down_alt ^ joystick_up_down_neu;
	joystick_up_pos_flanke = (joystick_up_down_flanke & joystick_up_down_neu) & 0b0010;
	joystick_down_pos_flanke = (joystick_up_down_flanke & joystick_up_down_neu) & 0b0001;
	//flanke left right
	joystick_left_right_alt = joystick_left_right_neu;
	joystick_left_right_neu = joystick_left_right;
	joystick_left_right_flanke = joystick_left_right_alt ^ joystick_left_right_neu;
	joystick_left_pos_flanke = (joystick_left_right_flanke & joystick_left_right_neu) & 0b0010;
	joystick_right_pos_flanke = (joystick_left_right_flanke & joystick_left_right_neu) & 0b0001;
//systemzeit
	inZeit_ms = getSystemTimeMs();
//buttons
	buttons = buttonReadAllPL();
	buttons_alt = buttons_neu;
	buttons_neu = buttons;
	buttons_flanke = buttons_alt ^ buttons_neu;
	back_button = (buttons_flanke & buttons_neu) & 0b0010;
//choice switch
	if(joystick_up_pos_flanke){
		position += 1;
	}
	if(joystick_down_pos_flanke){
		position -= 1;
	}
//States
	switch (machine_state){
		case 0: //Kitchen Standart zustand----------------------------------------------------------------------------------------------------------------
			//entry flag reset
				fridge_entry = 0;
			//anzeige
			lcdWriteText(1,0,machine_choices[position]);
			lcdWriteText(2,0,"                    ");
			lcdWriteText(3,0,"                    ");
			//auswahl treffen
			if(joystick_pos_flanke){
				machine_state = position +1;
				position = 0;
			}
			//overflow
			if(position > 15){
				position = 14;
			}
			else if(position == 15){
				position = 0;
			}
		break;
		
		case 1: //fridge---------------------------------------------------------------------------------------------------------------------------------------
			//entry
				if(!fridge_entry){
					timer = inZeit_ms;
					fridge_entry = 1;
				}
			//door open check
				if(fridge_door_winkel>0){
					fridge_door = 1;
				}
				else{
					fridge_door = 0;
				}
			//anzeige
					if(!fridge_state){
						lcdWriteText(3,0,"Temp:%u° ",fridge_temp);
						lcdWriteText(2,0,"      ");
						if(fridge_door){
							lcdWriteText(1,0,fridge[position]);
					}
				}
			//anpassung der temperatur nach der einstellung oder wenn türe offen erwärmung
				if(fridge_door){
					if(((inZeit_ms-timer2)>(72000/(fridge_door_winkel/10))) && (fridge_temp<22)){
						fridge_temp += 1;
						timer2 = inZeit_ms;
					}
				}
				else{
					if(((inZeit_ms-timer2)>1000)&&(fridge_temp>fridge_temp_einstellung)){
						fridge_temp -= 1;
						timer2 = inZeit_ms;
					}	
					else if (((inZeit_ms-timer2)>1000)&&(fridge_temp<fridge_temp_einstellung)){
						fridge_temp += 1;
						timer2 = inZeit_ms;
					}
				}
			//türe geöffnet
				if(fridge_door){
					//2 minuten cooldown bis der alar losgeht ( die türe ist zu lange offen)
					if((inZeit_ms-timer)>120000){
						fridge_blink_flag = 1;
					}
					//blink mechanik
					if(fridge_blink_flag){
						if((inZeit_ms-timer)>500){
							fridge_lamp = fridge_lamp ^1;
							timer = inZeit_ms;
						}
					}
					else{//leuchten wenn es nicht blinkt
						fridge_lamp = 1;
					}
				}
				else{// türe geschlossen nichts leuchtet und timer wird resetet
					timer = inZeit_ms;
					fridge_blink_flag = 0;
					fridge_lamp = 0;
					lcdWriteText(1,0,"Fridge Closed       ");
					lcdWriteText(2,0,"      ");
					position = 0;
					fridge_state = 0;
				}
			//back
				if(back_button && !fridge_state){
					machine_state = 0;
					position = 0;
				}
			//overflow
				if(!fridge_state){
					if(position == 255){
						position = 2;
					}
					else if(position == 3){
						position = 0;
					}
				}
				else if(fridge_state == 1){
					if(position == 255){
						position = 4;
					}
					else if(position == 5){
						position = 0;
					}
				}
			//Auswahl treffen
				if(joystick_pos_flanke&&fridge_door&&!fridge_state){
					fridge_state = position+1;
				}
			//fridge states
				switch (fridge_state){
					case 1://Fridge Inventory
						//slot anziege
							lcdWriteText(1,0,fridge_inventory[position]);
							lcdWriteText(3,0,"                    ");
						//item anzeige des entsprchenden slots
							if(!position){
								lcdWriteText(2,0,items[fridge_inventory_slot1]);
							}
							if(position == 1){
								lcdWriteText(2,0,items[fridge_inventory_slot2]);
							}
							if(position == 2){
								lcdWriteText(2,0,items[fridge_inventory_slot3]);
							}
							if(position == 3){
								lcdWriteText(2,0,items[fridge_inventory_slot4]);
							}
							if(position == 4){
								lcdWriteText(2,0,items[fridge_inventory_slot5]);
							}
						
						//back
							if(back_button){
								fridge_state = 0;
								position = 0;
							}
					break;
					case 2://Temperatur
					//anzeige
							lcdWriteText(0,0,machines[fridge_state+16]);
							lcdWriteText(1,0,"                    ");
							lcdWriteText(2,0,"  <%u°>                ",fridge_temp_einstellung);
							lcdWriteText(3,0,"       ");
						//temperatur einstellung
							if(joystick_left_pos_flanke&&(fridge_temp>2)){
								fridge_temp_einstellung -= 1;
							}
							else if (joystick_right_pos_flanke && (fridge_temp<8)){
								fridge_temp_einstellung += 1;
							}
						//back
							if(back_button){
								fridge_state = 0;
								position = 0;
							}
					break;
					case 3://Freezer
				
					break;
			}	
		break;
	}//------------------------------------------------------------------------------------------------------------------------------------------------------
		
//Output
if(!fridge_state){
lcdWriteText(0,0,machines[machine_state]);
}
ledWriteAll(fridge_lamp<<0);
			}
}

