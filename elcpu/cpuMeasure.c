
#include <tm1/tmProcessor.h>			/* for procCap */
#include <stdio.h>
#include <tm1/tmTypes.h>
#include <tm1/tmAvFormats.h>			/* for tmTimeStamp_t */
#include <ops/custom_defs.h>			/* for CYCLES */	


int cpuMeasure() {
	Float					DiffTime;
	pprocCapabilities_t 	procCap;
	tmTimeStamp_t			TimeValue;
	

	TimeValue.ticks   = CYCLES();
	TimeValue.hiticks = HICYCLES();
		
		
	// Processing function to measure....

		
		
	// Difference 
	TimeValue.ticks   = CYCLES()-TimeValue.ticks;
	TimeValue.hiticks = HICYCLES()-TimeValue.hiticks;
	printf(" TimeValue.hiticks = %d\tTimeValue.ticks=%d\n",TimeValue.hiticks,TimeValue.ticks);

	// Convert into milliseconds
	procGetCapabilities(&procCap);
	printf("cpuClockFrequency = %d\n",procCap->cpuClockFrequency);
	DiffTime = TimeValue.hiticks/((Float)procCap->cpuClockFrequency/1000.0);
	printf(" TimeValue.hiticks = %3.2f\t",TimeValue.hiticks/((Float)procCap->cpuClockFrequency/1000.0));
	
	DiffTime = TimeValue.ticks/((Float)procCap->cpuClockFrequency/1000.0);
	DP(("DECIMATION::MsbTime = %d\tLsbTime = %3.2fms\n",(MsbTime2-MsbTime1),DiffTime));


	return TMLIBAPPS_OK;
}


int main() {
}