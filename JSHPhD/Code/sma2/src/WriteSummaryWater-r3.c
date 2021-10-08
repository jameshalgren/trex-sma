/*---------------------------------------------------------------------
C-  Function:   WriteSummaryWater.c
C-
C-  Purpose/    Write summary flow (hydrology and hydraulics)
C-  Methods:    information at the end of a successful model run.
C-
C-  Inputs:
C-
C-  Outputs:
C-
C-  Controls:   infopt, chnopt
C-
C-  Calls:      None
C-
C-  Called by:  WriteSummary
C-
C-  Created:    Rosalia Rojas-Sanchez
C-              Department of Civil Engineering
C-              Colorado State University
C-              Fort Collins, CO 80523
C-
C-  Date:       19-JUN-2003 (assumed date)
C-
C-  Revised:    John F. England, Jr.
C-              Bureau of Reclamation
C-              Flood Hydrology Group, D-8530
C-              Bldg. 67, Denver Federal Center
C-              Denver, CO 80225
C-
C-              Mark Velleux
C-              Department of Civil Engineering
C-              Colorado State University
C-              Fort Collins, CO  80523
C-
C-  Date:       14-JUN-2005
C-
C-  Revisions:  Documentation, reorganization, expanded reporting
C-
C-  Date:
C-
C-  Revisions:
C-
C--------------------------------------------------------------------*/

//trex global variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for water transport
#include "trex_water_declarations.h"

void WriteSummaryWater(void)
{
    //local variable declarations
    double initialwatervol,     //initial water volume in domain (m3)
      finalwatervol,            //final water volume in domain (m3)
      totalwatervolin,          //total water volume entering domain (m3)
      totalwatervolout,         //total water volume exiting domain (m3)
      sourcewatervol,           //sum of all water sources (m3)
      sinkwatervol,             //sum of all water sinks (m3)
      watermberror;             //mass balance error (%)

    //set the summary statistics file pointer to null
    statsfile_fp = NULL;

    //open the summary statistics file (in append mode)
    statsfile_fp = fopen (statsfile, "a");

    //if file pointer is still null, then abort
    if (statsfile_fp == NULL)
    {
        //write error message to screen
        printf ("Error! Can't open Summary Statistics file %s\n", statsfile);
        exit (EXIT_FAILURE);    //exit with error
    }

    //Compute total water volumes in domain at simulation start and end...
    //
    //compute initial overland water volume at start (m3)
    initialwatervol = initialwaterovvol;

    //compute final overland water volume at end (m3)
    finalwatervol = finalwaterovvol;

    //if channels are simulated
    if (chnopt > 0)
    {
        //add initial channel water volume at start (m3)
        initialwatervol = initialwatervol + initialwaterchvol;

        //add final channel water volume at end (m3)
        finalwatervol = finalwatervol + finalwaterchvol;

    }                           //end if chnopt > 0

    //Compute total volume of water entering domain (m3)
    //
    //Note: totalwatervolin = netrainvol + snowvol + totalqwov + totalqwch + totalqinch
    totalwatervolin = totalnetrainvol + totalqwov + totalqwch + totalqinch;

    //Compute total water volumes exiting domain during simulation...
    //
    //Add total volume leaving the overland plane (m3)
    totalwatervolout = totalqoutov;

    //if infiltration is simulated
    if (infopt > 0)
    {
        //Add total volume infiltrated (m3)
        totalwatervolout = totalwatervolout + totalinfiltvol;

    }                           //end if infopt > 0

    //if channels are simulated
    if (chnopt > 0)
    {
        //Add total volume leaving the channel network (via channels) (m3)
        totalwatervolout = totalwatervolout + totalqoutch;

        //if channel transmission loss is simulated
        if (ctlopt > 0)
        {
            //Add total tranmission loss volume  (m3)
            totalwatervolout = totalwatervolout + totaltranslossvol;

        }                       //end if ctlopt > 0

    }                           //end if chnopt > 0

    //Compute sum of water source volumes (m3)
    sourcewatervol = initialwatervol + totalwatervolin;

    //Compute sum of water sink volumes (m3)
    sinkwatervol = finalwatervol + totalwatervolout;

    //Compute water mass balance error (percent)
    watermberror = (sourcewatervol - sinkwatervol) / sourcewatervol * 100.0;

    //Flow and Volume Output Summary
    fprintf (statsfile_fp, "\nFLOW AND VOLUME OUTPUT SUMMARY\n");
    fprintf (statsfile_fp, "==============================\n\n");

    fprintf (statsfile_fp,
             "Initial Water Volume Within Domain (m3)....................= ");
    fprintf (statsfile_fp, "%15.2f\n", initialwatervol);

    fprintf (statsfile_fp,
             "Cumulative Gross Rainfall Volume Entering Domain (m3)......= ");
    fprintf (statsfile_fp, "%15.2f\n", totalgrossrainvol);

    fprintf (statsfile_fp,
             "Cumulative Interception Volume Within Domain (m3)..........= ");
    fprintf (statsfile_fp, "%15.2f\n", totalinterceptvol);

    fprintf (statsfile_fp,
             "Cumulative Net Rainfall Volume Entering Domain (m3)........= ");
    fprintf (statsfile_fp, "%15.2f\n", totalnetrainvol);

    fprintf (statsfile_fp,
             "Cumulative Rainfall Excess (Rain-Intercept-Infilt) (m3)....= ");
    fprintf (statsfile_fp, "%15.2f\n", totalnetrainvol - totalinfiltvol);

    //mlv   fprintf(statsfile_fp,
    //mlv           "Cumulative Snow Volume (Gross) Within Domain (m3)..........= ");
    //mlv   fprintf(statsfile_fp,"%15.2f\n", totalsnowmeltvol);

    //mlv   fprintf(statsfile_fp,
    //mlv           "Cumulative Snowmelt Volume (Net) Within Domain (m3)........= ");
    //mlv   fprintf(statsfile_fp,"%15.2f\n", totalsnowmeltvol);

    fprintf (statsfile_fp,
             "Flow Volume Entering Domain from External Sources (m3).....= ");
    fprintf (statsfile_fp, "%15.2f\n", totalqwov + totalqwch);

    fprintf (statsfile_fp,
             "  Volume Entering the Overland Plane (m3).....= ");
    fprintf (statsfile_fp, "%f\n", totalqwov);

    //if channels are simulated
    if (chnopt > 0)
    {
        fprintf (statsfile_fp,
                 "  Volume Entering the Channel Network (m3)....= ");
        fprintf (statsfile_fp, "%f\n", totalqwch);

    }                           //end if chnopt > 0

    fprintf (statsfile_fp,
             "Cumulative Volume Entering Domain, V_in (m3)...............= ");
    fprintf (statsfile_fp, "%15.2f\n", totalwatervolin);

    fprintf (statsfile_fp,
             "Volume leaving the Watershed, V_out (m3)...................= ");
    fprintf (statsfile_fp, "%15.2f\n", totalwatervolout);

    fprintf (statsfile_fp,
             "Percentage of V_out to V_in (%%)............................= ");
    fprintf (statsfile_fp, "%15.2f\n",
             (totalwatervolout / totalwatervolin) * 100.0);

    fprintf (statsfile_fp,
             "Volume leaving the Watershed via Overland Flow (m3)........= ");
    fprintf (statsfile_fp, "%15.2f\n", totalqoutov);

    //if channels are simulated
    if (chnopt > 0)
    {
        fprintf (statsfile_fp,
                 "Volume leaving the Watershed via Channel Flow (m3).........= ");
        fprintf (statsfile_fp, "%15.2f\n", totalqoutch);

    }                           //end if chnopt > 0

    fprintf (statsfile_fp,
             "Percentage of Overland/Channel Outflow to V_in (%%).........= ");
    fprintf (statsfile_fp, "%15.2f\n",
             ((totalqoutov + totalqoutch) / totalwatervolin) * 100.0);

    fprintf (statsfile_fp,
             "Final Surface Volume, V_final (m3).........................= ");
    fprintf (statsfile_fp, "%15.2f\n", finalwatervol);

    fprintf (statsfile_fp,
             "Percentage of V_final to V_in (%%)..........................= ");
    fprintf (statsfile_fp, "%15.2f\n",
             (finalwatervol / totalwatervolin) * 100.0);

    fprintf (statsfile_fp,
             "Percentage of Overland/Channel Outflow to V_in-V_final (%%).= ");
    fprintf (statsfile_fp, "%15.2f\n", ((totalqoutov + totalqoutch) /
                                        (totalwatervolin -
                                         finalwatervol)) * 100.0);

    fprintf (statsfile_fp,
             "Volume Infiltrated Overland, V_inf (m3)....................= ");
    fprintf (statsfile_fp, "%15.2f\n", totalinfiltvol);

    //if channels are simulated
    if (chnopt > 0)
    {
        //if channel transmission loss is simulated (ctlopt > 0)
        if (ctlopt > 0)
        {
            fprintf (statsfile_fp,
                     "Volume of Transmission Losses, V_ctl (m3)..................= ");
            fprintf (statsfile_fp, "%15.2f\n", totaltranslossvol);

        }                       //end if ctlopt > 0 (transmission loss is simulated)

    }                           //end if chnopt > 0

    fprintf (statsfile_fp,
             "Percentage of (V_inf + V_ctl) to V_in (%%)..................= ");
    fprintf (statsfile_fp, "%15.2f\n\n",
             ((totalinfiltvol +
               totaltranslossvol) / totalwatervolin) * 100.0);

    fprintf (statsfile_fp,
             "Percent Mass Balance Error (%%).............................= ");
    fprintf (statsfile_fp, "%15.2f\n", watermberror);

    //Print Min/Max hydrologic variable (rainfall, infiltration, etc.) summary
    fprintf (statsfile_fp,
             "\nMINIMUM AND MAXIMUM VALUES FOR SELECT HYDROLOGIC/HYDRAULIC PROCESSES\n");
    fprintf (statsfile_fp,
             "=====================================================================\n\n");

    //Loop over outlets
    for (i = 1; i <= noutlets; i++)
    {
        fprintf (statsfile_fp,
                 "Outlet: %d (Row %d, Col %d)\n", i, iout[i], jout[i]);

        //if channels are not simulated or the outlet option is 1
        if (chnopt == 0 || outopt == 1)
        {
            fprintf (statsfile_fp,
                     "Peak Overland Discharge (m3/s).............................= ");
            fprintf (statsfile_fp, "%15.2f\n", qpeakov[i]);

            fprintf (statsfile_fp,
                     "Time to Peak (hours).......................................= ");
            fprintf (statsfile_fp, "%15.2f\n", tpeakov[i]);

        }                       //end if chnopt = 0 or outopt = 1

        //if channels are simulated
        if (chnopt > 0)
        {
            fprintf (statsfile_fp,
                     "Peak Channel Discharge (m3/s)..............................= ");
            fprintf (statsfile_fp, "%15.2f\n", qpeakch[i]);

            fprintf (statsfile_fp,
                     "Time to Peak (hours).......................................= ");
            fprintf (statsfile_fp, "%15.2f\n", tpeakch[i]);

        }                       //end of chnopt > 0

        fprintf (statsfile_fp, "\n");

    }                           //end loop over outlets

    fprintf (statsfile_fp,
             "Min. Rainfall Intensity (mm/hr)............................= ");
    fprintf (statsfile_fp, "%15.2f\n", minrainintensity * 3600.0 * 1000.0);

    fprintf (statsfile_fp,
             "Max. Rainfall Intensity (mm/hr)............................= ");
    fprintf (statsfile_fp, "%15.2f\n\n", maxrainintensity * 3600.0 * 1000.0);

    //if infiltration is simulated
    if (infopt > 0)
    {
        //print infiltration statistics
        fprintf (statsfile_fp,
                 "Min. Infiltration Depth (mm)...............................= ");
        fprintf (statsfile_fp, "%15.2f\n", mininfiltdepth * 1000.0);

        fprintf (statsfile_fp,
                 "Max. Infiltration Depth (mm)...............................= ");
        fprintf (statsfile_fp, "%15.2f\n\n", maxinfiltdepth * 1000.0);
    }
    else                        //infiltration not simulated
    {
        //print message that infiltration not simulated...
        fprintf (statsfile_fp, "\nInfiltration was not simulated\n\n");

    }                           //end if infopt > 0

    //if channels and transmission losses are  simulated
    if (chnopt > 0 && ctlopt > 0)
    {
        //print infiltration statistics
        fprintf (statsfile_fp,
                 "Min. Transmission Loss Depth (mm)..........................= ");
        fprintf (statsfile_fp, "%15.2f\n", mintranslossdepth * 1000.0);

        fprintf (statsfile_fp,
                 "Max. Transmision Loss Depth (mm)...........................= ");
        fprintf (statsfile_fp, "%15.2f\n\n", maxtranslossdepth * 1000.0);
    }
    else                        //transmission losses not simulated
    {
        //print message that transmission losses not simulated...
        fprintf (statsfile_fp, "\nTransmission loss was not simulated\n\n");

    }                           //end if chnopt > 0 AND ctlopt > 0

    //print water depth statistics
    fprintf (statsfile_fp,
             "Min. Overland Depth (m)....................................= ");
    fprintf (statsfile_fp, "%15.3f\n", minhovdepth);

    fprintf (statsfile_fp,
             "Max. Overland Depth (m)....................................= ");
    fprintf (statsfile_fp, "%15.3f\n\n", maxhovdepth);

    //if channels are simulated
    if (chnopt > 0)
    {
        fprintf (statsfile_fp,
                 "Min. Channel Depth (m).....................................= ");
        fprintf (statsfile_fp, "%15.3f\n", minhchdepth);

        fprintf (statsfile_fp,
                 "Max. Channel Depth (m).....................................= ");
        fprintf (statsfile_fp, "%15.3f\n\n", maxhchdepth);
    }
    else                        //channels not simulated
    {
        //print message that channel flow not simulated...
        fprintf (statsfile_fp, "\nChannel flow was not simulated\n\n");

    }                           //end if chnopt > 0

    //close the summary statistics file
    fclose (statsfile_fp);

//End of function: Return to WriteSummary
}
