/*
 * C String.c
 *
 * Created: 01.06.2026 11:22:26
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"


int main(void)
{
	initBoard(0);
	char names [] = {"Niels"};
	char names1 [] = {"Niels"};
	char * binames [] = {"Neo hat einen", "kleinen schwanni"};
uint8_t flag = 0;
uint8_t identisch = 0;
int8_t resultat = 0;
if(flag){
	for(uint8_t i = 0; names[i]; i += 1){
		if((names[i]>91) && (names[i]<123)){
			names[i] = names[i] - 0x20;
		}
	}
}
resultat = strcmp(names,names1);
	if(!(resultat == 0)){
		identisch = 1;
	}
// for(uint8_t v = 0; names[v]; v += 1){
// 	if(!(names[v] == names1[v])){
// 		identisch = 1;
// 		break;
// 	}
// }
	lcdWriteZahl(1,0,identisch,1,0);
	lcdWriteText(0,0,names);
    while (1) {
		lcdLight(255);
    }
}

