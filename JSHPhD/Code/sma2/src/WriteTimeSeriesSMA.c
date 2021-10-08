/*---------------------------------------------------------------------
C-  Function:	WriteTimeSeriesSMA.c                                              
C-
C-	Purpose/    The module writes times series reports of SMA output
C-	Methods:    (discharge in m3/s or mm/hr) at each specified location
C-              (reporting stations) for the current time t.
C-
C-	Inputs:		qreport
C-
C-	Outputs:	
C-
C-	Controls:   ksim, nqreports, qconvert
C-
C-	Calls:		None
C-
C-	Called by:	WriteTimeSeries
C-
C-	Created:	Rosalia Rojas-Sanchez
C-				Department of Civil Engineering
C-              Colorado State University
C-				Fort Collins, CO 80523
C-
C-	Date:		19-JUN-2003 (last version)
C-
C-	Revised:	John F. England, Jr.
C-				Bureau of Reclamation
C-				Flood Hydrology Group, D-8530
C-				Bldg. 67, Denver Federal Center, Denver, CO 80225
C-
C-              Mark Velleux (CSU)
C-              Department of Civil Engineering
C-              Colorado State University
C-              Fort Collins, CO  80523
C-
C-	Date:		20-MAR-2004
C-
C-	Revisions:	Documentation, reorganization, complete rewrite
C-
C-	Date:		
C-
C-	Revisions:	
C-
C--------------------------------------------------------------------*/

//trex global variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for water transport
#include "trex_water_declarations.h"

//variable declarations and definitions specific to the 
//soil moisture accounting procedure.
#include "trex_SMA_declarations.h"

void WriteTimeSeriesSMA (void)
{
    //Set the SMA output file pointer to null
    SMAstatefile_fp = NULL;

    //open the water export file for writing in append mode
    SMAstatefile_fp = fopen (SMAstatefile, "a");

    //if the SMA output file pointer is null, abort execution
    if (SMAstatefile_fp == NULL)
    {
        //Write message to screen
        SMAstatefile_fp = stderr;
        printf ("Can't open SMA output File:  %s\n", SMAstatefile);
        printf ("printing to stderr\n");
        //exit (EXIT_FAILURE);    //abort
    }

	//Write current simulation time in human readable YYYY-MM-DD HH:MM:SS
	fprintf (SMAstatefile_fp, "%s", displaydate);
	//Write current simulation time (hours)
	fprintf (SMAstatefile_fp, ",%f", simtime);
  //Write storm #
	fprintf (SMAstatefile_fp, ",%d", smaReinitializationStormCount);
    //Write SMA states for all zones
    //and print out in comma-delimited format
    for (k = 1; k <= nuz; k++)
      //Upper zone storage ratios
    {
        fprintf (SMAstatefile_fp, ", %g", 
            uzSMA[k]->twc / uzSMA[k]->twm);
        for (h = 1; h <= uzSMA[k]->nparts; h++)
        {
            fprintf (SMAstatefile_fp, ", %g",
                     uzSMA[k]->part[h]->fwc / uzSMA[k]->part[h]->fwm);
        }
    }
    for (k = 1; k <= nlz; k++)
      //Lower zone storage ratios
    {
        fprintf (SMAstatefile_fp, ", %g", 
            lzSMA[k]->twc / lzSMA[k]->twm);
        for (h = 1; h <= lzSMA[k]->nparts; h++)
        {
            fprintf (SMAstatefile_fp, ", %g",
                     lzSMA[k]->part[h]->fwc / lzSMA[k]->part[h]->fwm);
        }
    }
    for (k = 1; k <= nuz; k++)
      //Individual upper zone flows
    {
        fprintf (SMAstatefile_fp, ", %g, %g, %g",
                uzSMA[k]->etdemand,
                uzSMA[k]->et,
                uzSMA[k]->et_deep);
        for (h = 1; h <= uzSMA[k]->nparts; h++)
        {
            fprintf (SMAstatefile_fp, ", %g", 
                uzSMA[k]->part[h]->intf);
        }
    }
    for (k = 1; k <= nlz; k++)
      //Individual lower zone flows
    {
            fprintf (SMAstatefile_fp, ", %g", 
                lzSMA[k]->percdemand);
        for (h = 1; h <= lzSMA[k]->nparts; h++)
        {
            fprintf (SMAstatefile_fp, ", %g", 
                lzSMA[k]->part[h]->basf); 
        }
    }
    for (k = 1; k <= nio; k++)
      //total flows from SMA zones
    {
        fprintf (SMAstatefile_fp, ", %g", 
            ioSMA[k]->wnew);
        fprintf (SMAstatefile_fp, ", %g",
            dqchin[ioSMA[k]->intfout_i][ioSMA[k]->intfout_j][0]);
    }
    for (k = 1; k <= nuz; k++)
      //Upper zone states
    {
        fprintf (SMAstatefile_fp, ", %g, %g, %g",
            uzSMA[k]->wnew,
            uzSMA[k]->twc, 
            uzSMA[k]->twm);
        for (h = 1; h <= uzSMA[k]->nparts; h++)
        {
            fprintf (SMAstatefile_fp, ", %g, %g", 
                uzSMA[k]->part[h]->fwc,
                uzSMA[k]->part[h]->fwm);
        }
    }
    for (k = 1; k <= nlz; k++)
      //Lower zone states
    {
        fprintf (SMAstatefile_fp, ", %g, %g, %g",
            lzSMA[k]->wnew,
            lzSMA[k]->twc,
            lzSMA[k]->twm);
        for (h = 1; h <= lzSMA[k]->nparts; h++)
        {
            fprintf (SMAstatefile_fp, ", %g, %g",
                lzSMA[k]->part[h]->fwc,
                lzSMA[k]->part[h]->fwm);
        }
    }

    //print a carriage return after each complete line
    fprintf (SMAstatefile_fp, "\n");

    //Close water export file
    fclose (SMAstatefile_fp);

//End of function: Return to WriteTimeSeries
}
