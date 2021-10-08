/*----------------------------------------------------------------------
C-  Function:	ChannelWaterDepth.c
C-
C-	Purpose/	Computes channel water depth hch[][] in each node in the
C-	Methods:	channel network.  If depth in a node becomes unstable (a
C-				negative channel depth occurs), then aborts routine and
C-              calls Simulation Error to report error message.
C-				
C-
C-	Inputs:		dqch[][], bwidth[][], hbank[][], sideslope[][],
C-              deadstoragedepth[][], chanlength[][], dt[], hov[][],
C-              ichnrow[][], ichncol[][]  (Globals)
C-
C-	Outputs:	hchnew[][], hovnew[][] (values at time t+dt)(Globals)
C-              vchintermediate[][]
C-
C-	Controls:   ksim
C-
C-	Calls:		SimulationError
C-
C-	Called by:	WaterBalance
C-
C-	Created:	Original Coding: Rosalia Rojas-Sanchez
C-              Department of Civil Engineering
C-              Colorado State University
C-				Fort Collins, CO 80523
C-
C-	Date:		19-JUN-2003 (last version)
C-
C-	Revised:	Mark Velleux
C-              Department of Civil Engineering
C-              Colorado State University
C-              Fort Collins, CO  80523
C-				
C-				John F. England, Jr.
C-				Bureau of Reclamation
C-				Flood Hydrology Group, D-8530
C-				Bldg. 67, Denver Federal Center, Denver, CO  80225
C-
C-	Date:		03-SEP-2003
C-
C-	Revisions:	Document variables and main computations, correct
C-				variable names; correct channel lengths	orientated
C-              in 8 directions; added trapezoidal channel geometry
C-				(rectangular/triangle special cases).
C-
C-  Revised:	Mark Velleux (CSU)
C-
C-  Date:		05-May-2005
C-
C-  Revisions:	Code to control floodplain water transfer between
C-              overland plane and channels moved to a separate
C-              process routine (FloodplainWaterTransfer).
C-
C-  Revised
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

void ChannelWaterDepth(void)
{
    //local variable declarations/definitions
    int row,                    //row number of present (upstream) node
      col;                      //column number of present (upstream) node

    float bwch,                 //bottom width of channel (m)
      hbch,                     //bank height of channel (m)
      sslope,                   //side slope of channel bank (dimensionless)
      lch,                      //channel length (m) (includes sinuosity)
      achnew,                   //new cross-section area of water in channel (m2)
      achsurf,                  //suface area of channel (m2)
      vch,                      //volume of water in channel (m3) (at depth hch)
      vchin,                    //inflowing volume of water in channel from upstream node (m3)
      vchnew,                   //new volume of water in channel (m3) (after flow transfer)
      vchbank,                  //volume of water channel can hold at top of bank (m3)
      vchavail,                 //available (unfilled) volume of channel before it is filled to top of bank (m3)
      twch;                     //top width of channel at top of bank (m)

    float derivative;           //volumetric derivative (m3/s)

    //Compute Surface Water Balance...
    //
    //The depth of water in the channel network is the sum of water
    //inputs from net rainfall (equals gross rainfall, no interception),
    //external water sources, and internal water transfers, (flooding
    //and return flow from channels).
    //
    //Developer's Note:  Transmission loss and evaporation would be
    //                   subtracted from the water balance as well.
    //                   However, these features are not included in
    //                   the present version...
    //
    //Loop over number of links
    for (i = 1; i <= nlinks; i++)
    {
        //Loop over number of nodes for current link
        for (j = 1; j <= nnodes[i]; j++)
        {
            //Determine row/column location for current link/node
            row = ichnrow[i][j];        //Row number of link i, node j
            col = ichncol[i][j];        //Col number of link i, node j

            //Assign channel characteristics (present link/node)
            bwch = bwidth[i][j];        //channel bottom width (m)
            hbch = hbank[i][j]; //channel bank height (m)
            sslope = sideslope[i][j];   //channel side slope
            lch = chanlength[i][j];     //channel length (m) (includes sinuosity)
            twch = twidth[i][j];        //channel top width at top of bank
            vchbank = (bwch + sslope * hbch) * hbch * lch;      //channel volume at top of bank (m)

            //Compute surface area of channel...
            //
            //The channel (and overland) surface area of a cell is
            //constant and defined by the top width of the channel
            //at the bank height and the channel length (including
            //sinuosity):
            //
            //  achsurf = twch * lch
            //
            //Surface area of channel (m2)
            achsurf = twch * lch;

            //Build derivative term by term...
            //
            //  derivative = netrainrate[i][j]*area
            //             - translossrate[i][j]*area
            //             + dqch[i][j]
            //
            //Note: the derivative has units of m3/s.
            //
            //Start with net rain (m3/s) (apportioned for channel surface area)
            derivative = netrainrate[row][col] * achsurf;

            //Subtract channel transmission loss (m3/s) (if simulated)
            if (ctlopt > 0)
                derivative = derivative - (translossrate[i][j] * bwch * lch);

            //Add net channel flow rate (includes floodplain transfer)
            derivative = derivative + dqch[i][j];

            //Compute volume of water entering (or leaving) this node
            //over this time step
            vchin = derivative * dt[idt];       //channel inflow volume (m3)

            //Compute water volume in channel...
            //
            //if the starting channel water depth is less than the bank height
            if (hch[i][j] < hbch)
            {
                //Compute volume of water in channel: (b+zy)y * length
                vch = (float) (bwch + sslope * hch[i][j]) * hch[i][j] * lch;

                //Compute available volume in channel (from top of
                //water surface to top of bank)
                vchavail = vchbank - vch;

                //if inflowing volume < = available volume in channel
                if (vchin <= vchavail)
                {
                    //Add inflowing volume to channel...
                    //
                    //Compute new water volume in channel
                    vchnew = vch + vchin;

                    //Compute new channel cross-section area (m2)
                    achnew = vchnew / lch;

                    //Compute new channel depth...
                    //
                    //if sslope > 0 (the channel is trapezoidal or triangular)
                    if (sslope > 0)
                    {
                        //Compute new channel depth (positive root from quadratic eqn)
                        hchnew[i][j] = (float) ((-bwch + sqrt (pow (bwch, 2.0)
                                                               -
                                                               4.0 * sslope *
                                                               (-achnew))) /
                                                (2.0 * sslope));
                    }
                    else        //sslope = 0 (channel is rectangular)
                    {
                        //Compute new channel depth (from area and width)
                        hchnew[i][j] = achnew / bwch;

                    }           //end if sslope > 0
                }
                else            //inflowing volume exceeds available volume in channel
                {
                    //First, fill channel to top of bank...
                    //
                    //New (interim) channel depth is bank height
                    hchnew[i][j] = hbch;

                    //Compute remaining inflowing volume
                    vchin = vchin - vchavail;

                    //Second, add remaining volume as a rectangular block...
                    //
                    //Set new channel depth
                    hchnew[i][j] = hchnew[i][j] + vchin / achsurf;

                }               //end if vchin <= vchavail
            }
            else                //channel water depth is >= bank height
            {
                //Add all inflowing volume as a rectangular block
                //
                //Set new channel depth
                hchnew[i][j] = hch[i][j] + vchin / achsurf;

            }                   //end if hch[][] < hbank

            //If the new channel depths is negative
            if (hchnew[i][j] < 0.0)
            {
                //Check for round-off error...
                //
                //if the magnitude is < the error tolerance
                if ((float) fabs (hchnew[i][j]) < TOLERANCE)
                {
                    //The error is round-off...
                    //
                    //Reset the new channel depth to exactly zero
                    hchnew[i][j] = 0.0;
                }
                else            //the error is numerical instability
                {
                    //Report error type (negative depth in channel) and location
                    SimulationError (2, i, j, 0);

                    exit (EXIT_FAILURE);        //abort

                }               //end check for round-off error

            }                   //end if hchnew < 0.0

            //if the new depth is less than the TOLERANCE
            if (hchnew[i][j] < TOLERANCE)
            {
                //reset the depth to zero
                hchnew[i][j] = 0.0;

            }                   //end if hchnew[i][j] < TOLERANCE

        }                       //end loop over nodes

    }                           //end loop over links

//End of Function: Return to WaterBalance
}
