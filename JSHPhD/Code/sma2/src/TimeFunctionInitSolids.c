/*---------------------------------------------------------------------
C-  Function:	TimeFunctionInitSolids.c
C-
C-	Purpose/	TimeFunctionInitSolids is called at the start of the
C-  Methods:	simulation to set the starting values of parameters
C-				used for sediment transport time series functions.
C-
C-  Inputs:	    simtime, swovtime[][], nsovpairs[][], swchtime[][],
C-              nswchpairs[][], sbctime[][], nsbcpairs[][]
C-
C-  Outputs:    swovpoint[], swchpoint[][], sbcpoint[][]
C-
C-  Controls:   chnopt, dbcopt[]
C-
C-  Calls:      None
C-
C-  Called by:  TimeFunctionInit
C-
C-	Created:	Mark Velleux
C-				Department of Civil Engineering
C-				Colorado State University
C-				Fort Collins, CO 80523
C-
C-				John F. England, Jr.
C-				Bureau of Reclamation
C-				Flood Hydrology Group, D-8530
C-				Bldg. 67, Denver Federal Center, Denver, CO 80225
C-
C-	Date:		23-OCT-2003
C-
C-	Revised:
C-
C-	Date:	
C-
C-	Revisions:
C-
C----------------------------------------------------------------------*/

//trex global variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for water transport
#include "trex_water_declarations.h"

//trex global variable declarations for sediment transport
#include "trex_solids_declarations.h"

void TimeFunctionInitSolids(void)
{
    //local variable declarations/definitions
    int ip;                     //pointer to current position in time series

    //Initialize indices for overland solids load time series
    //
    //loop over number of solids
    for (i = 1; i <= nsolids; i++)
    {
        //set the initial overland solids load function update time to the simulation start time
        ntswov[i] = (float) (tstart);

        //Loop over number of overland loads for current solids type
        for (j = 1; j <= nswov[i]; j++)
        {
            //Set the time series position index
            ip = 1;

            //while the simulation time is greater than the ending
            //  time of the current solids load AND the time series
            //  index is less than the number of pairs input
            while ((simtime > swovtime[i][j][ip]) && (ip < nswovpairs[i][j]))
            {
                //increment the time series position index
                ip = ip + 1;

            }                   //end while

            //Pointer to the time series position index for this
            //sediment load time series corrsesponding to simtime
            swovpoint[i][j] = ip;

            //Set the next (upper) time for the function update window
            nswovt[i][j] = (float) simtime;

            //Set the prior (lower) time for the function update window
            pswovt[i][j] = (float) simtime;

        }                       //end loop over number of loads for current solid

    }                           //end loop over number of solids

    //if channels are simulated
    if (chnopt > 0)
    {
        //loop over number of solids
        for (i = 1; i <= nsolids; i++)
        {
            //set the initial channel solids load function update time to the simulation start time
            ntswch[i] = (float) (tstart);

            //Loop over number of channel loads for current solids type
            for (j = 1; j <= nswch[i]; j++)
            {
                //Set the time series position index
                ip = 1;

                //while the simulation time is greater than the ending
                //  time of the current solids load AND the time series
                //  index is less than the number of pairs input
                while ((simtime > swchtime[i][j][ip])
                       && (ip < nswchpairs[i][j]))
                {
                    //increment the time series position index
                    ip = ip + 1;

                }               //end while

                //Pointer to the time series position index for this
                //sediment load time series corrsesponding to simtime
                swchpoint[i][j] = ip;

                //Set the next (upper) time for the function update window
                nswcht[i][j] = (float) simtime;

                //Set the prior (lower) time for the function update window
                pswcht[i][j] = (float) simtime;

            }                   //end loop over number of loads for current solid

        }                       //end loop over number of solids

    }                           //end if chnopt > 0

    //Initialize indices for outlet solids boundary condition time series
    //
    //loop over number of outlets
    for (i = 1; i <= noutlets; i++)
    {
        //if a time series is specified for this outlet
        if (dbcopt[i] > 0)
        {
            //Loop over number of solidss
            for (j = 1; j <= nsolids; j++)
            {
                //set the initial solids boundary condition update time to the simulation start time
                ntsbc[j] = (float) (tstart);

                //Set the time series position index
                ip = 1;

                //while the simulation time is greater than the ending
                //  time of the current solids bc AND the time series
                //  index is less than the number of pairs input
                while ((simtime > sbctime[i][j][ip])
                       && (ip < nsbcpairs[i][j]))
                {
                    //increment the time series position index
                    ip = ip + 1;

                }               //end while

                //Pointer to the time series position index for this
                //sediment load time series corrsesponding to simtime
                sbcpoint[i][j] = ip;

                //Set the next (upper) time for the function update window
                nsbct[i][j] = (float) simtime;

                //Set the prior (lower) time for the function update window
                psbct[i][j] = (float) simtime;

            }                   //end loop over number solids

        }                       //end if dbcopt[i] > 0

    }                           //end loop over outlets

//End of function: Return to TimeFunctionInit
}
