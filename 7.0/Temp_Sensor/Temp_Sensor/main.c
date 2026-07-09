/*
 * test_speicher_Spielstand.c
 *
 * Created: 19.03.2026 10:43:16
 * Author : e4Menzi
 */ 

#include "ucBoardDriver.h"


int main(void)
{
	uint16_t inAdcWertTempSens = 0;
	uint16_t outAdcWertSpannung = 0;
	uint16_t outTemp = 0;
	
	initBoard(0);
	
	lcdWriteText(0, 0, "ADC:");
	lcdWriteText(1, 0, "Spannung:");
	lcdWriteText(2, 0, "Temp:");


	while (1)
	{
		
		inAdcWertTempSens = adcRead(ADC_13_TEMPERATUR);
		outAdcWertSpannung = (uint32_t)inAdcWertTempSens*1000*5/1023;
		outTemp = (outAdcWertSpannung-400)*51/1000;
		

		lcdWriteText(0, 8, "%4u/1023", inAdcWertTempSens);
		lcdWriteText(1, 8, "%4u mV", outAdcWertSpannung);
		lcdWriteText(2, 8, "%4u gC", outTemp);

	}
}