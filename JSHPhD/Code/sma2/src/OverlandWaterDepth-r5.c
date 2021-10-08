/*----------------------------------------------------------------------
C-  Function:	OverlandWaterDepth.c
C-
C-	Purpose/	Updates overland water depths (x,y) in each grid cell,
C-	Methods:	checks for negative depth value (exit program option);
C-
C-
C-	Inputs:		netrainrate[][], infiltrationrate[][], dqov[][],
C-              hov[][] (values at time t)
C-
C-	Outputs:	hovnew[][] (values at time t+dt)
C-              vovintermediate[][]
C-
C-	Controls:   ksim
C-
C-	Calls:		SimulationError
C-
C-	Called by:	WaterBalance
C-
C-	Created:	Original Coding in Fortran - CASC2D User's Manual
C-				Julien, P.Y. and Saghafian, B. 1991
C-				Civil Engineering, Colorado State University
C-				Fort Collins, CO 80523
C-
C-				Code later converted from Fortran to C
C-				Last Update by Rosalia Rojas-Sanchez, CSU
C-
C-	Date:		19-JUN-2003
C-
C-	Revised:	John F. England, Jr.
C-				Bureau of Reclamation
C-				Flood Hydrology Group, D-8530
C-				Bldg. 67, Denver Federal Center, Denver, CO 80225
C-
C-              Mark Velleux (CSU)
C-
C-	Date:		02-SEPT-2003
C-
C-	Revisions:	Document variables and main computations, restructure
C-              to separate transport components from water balance.
C-
C-  Revised:
C-
C-	Date:
C-
C-	Revisions:
C-
C---------------------------------------------------------------------*/

//trex global variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for water transport
#include "trex_water_declarations.h"

void OverlandWaterDepth( )
{
    //JSH ADD This file has been slightly modified in the
    //JSH ADD sma2optimize branch of this code. 

    //local variable declarations/definitions
    int chanlink,               //channel link number
      channode;                 //channel node number

    float lch,                  //channel length (m) (includes sinuosity)
      twch,                     //top width of channel at flow depth (m)
      achsurf,                  //surface area of channel portion of cell (m)
      aovsurf;                  //surface area of overland portion of cell (m)

    float derivative;           //sum of rates -  rainfall, snowmelt, infiltration, flows (m/s)

    //Compute Surface Water Balance...
    //
    //The depth of water in the overland plane is the sum of water
    //inputs from net rainfall (gross rainfall minus interception),
    //snowmelt, external water sources, internal water transfers
    //and (flooding/return flow from channels) minus infiltration.
    //Because flooding/return flow from channels also depends on
    //water depth in the channel, floodplain interactions are
    //computed in ChannelDepth (and hov updated).
    //
    //Developer's Note: evapotranspiration would be subtracted from this
    //the water balance as well.  However, this feature not included in
    //present version...
    //
    //Loop over rows
    for (i = 1; i <= nrows; i++)
    {
        //Loop over columns
        for (j = 1; j <= ncols; j++)
        {
            //If the cell is in the domain
            if (imask[i][j] != nodatavalue)
            {
                //Check if the cell is a channel cell.  We don't
                //really need to check if chnopt > 0 because the
                //imask should be 1 but adding a chnopt check
                //makes the code a bit more "error proof"...
                //
                //if the cell is a channel cell and channels are simulated
                if ((imask[i][j] > 1) && (chnopt > 0))
                {
                    //Compute surface areas of overland and channel
                    //potions of the cell...
                    //
                    //Get channel link and node at this row and column
                    chanlink = link[i][j];
                    channode = node[i][j];

//jfe:  Note: these do not need to be computed locally because the
//jfe:        values are constant and depend only on input.  The
//jfe:        surface area can be computed at the time the channel
//jfe:        property file is read and make achsurf a global...

                    //Compute area of channel within cell...
                    //
                    //Assign channel characteristics
                    lch = chanlength[chanlink][channode];       //channel length (m) (includes sinuosity)
                    twch = twidth[chanlink][channode];  //channel top width at bank height (m)

                    //Surface area of channel portion of cell (m2)
                    achsurf = twch * lch;

                    //Surface area of overland portion of cell (m2)
                    aovsurf = w * w - achsurf;
                }
                else            //the cell is an overland cell
                {
                    //Overland area of cell = entire area of cell
                    aovsurf = w * w;

                }               //end if imask[][] > 1

                //Build derivative term by term...
                //
                //  derivative = netrainrate[i][j]
                //             - infiltrationrate[i][j]
                //             + snowmeltrate[i][j]
                //             + (dqov[i][j] / aovsurf)
                //
                //Note: the derivative has units of m/s and
                //      dqov is divided by the overland area
                //      of the cell to obtain a velocity.
                //
                //Start with net rain (apportioned for overland surface area)
                derivative = netrainrate[i][j];

                //Subtract infiltration
                if (infopt > 0)
                    derivative = derivative - infiltrationrate[i][j];

//mlv                           //Add snowmelt
//mlv                           derivative = derivative + snowmeltrate[i][j]

                //Add net overland flow rate over overland area of cell
                derivative = derivative + dqov[i][j] / aovsurf;

                //update overland flow depth to new depth
                //new depth = current depth + derivative * time step
                hovnew[i][j] = hov[i][j] + derivative * dt[idt];

                //Flow depths must always be >= to zero. When hov < 0,
                //this simulation aborts.  Warn user and write error
                //message...
                //
                //if the overland water depth in this cell < 0
                if (hovnew[i][j] < 0.0)
                {
                    //Check for round-off error...
                    //
                    //if the magnitude of the depth < the error tolerance
                    if ((float) fabs (hovnew[i][j]) < TOLERANCE)
                    {
                        //The error is round-off...
                        //
                        //Reset the new overland depth to exactly zero
                        hovnew[i][j] = 0.0;
                    }
                    else        //the error is numerical instability
                    {
                        //simulation error number;      simerrorno = 1;
                        //
                        //Report error type (negative depth in overland cell) and location
                        SimulationError (1, i, j, 0);

                        exit (EXIT_FAILURE);    //abort

                    }           //end check for round-off error

                }               //end if negative depth

                //if the new depth is less than the TOLERANCE
                if (hovnew[i][j] < TOLERANCE)
                {
                    //reset the depth to zero
                    hovnew[i][j] = 0.0;

                }               //end if hovnew[i][j] < TOLERANCE

            }                   //end if current cell is in the domain (imask != nodatavalue)

        }                       //end loop over columns

    }                           //end loop over rows

}                               //End of function: Return to WaterBalance
