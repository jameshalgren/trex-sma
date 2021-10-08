/*---------------------------------------------------------------------
C-  Function:	ComputeFinalStateWater.c
C-
C-  Purpose/    Computes the final state of water variables
C-  Methods:    at the end of the simulation.
C-  
C-  
C-  Inputs:     
C-
C-  Outputs:    
C-
C-  Controls:   ksim
C-
C-  Calls:      None
C-
C-  Called by:  ComputeFinalState
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
C-	Date:		20-MAR-2004
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

void ComputeFinalStateWater(void)
{
    //local variable declaration
    int chanlink,               //channel link number
      channode;                 //channel node number

    float hflow,                //flow depth of channel (m)
      bwch,                     //bottom width of channel (m)
      hbch,                     //bank height of channel (m)
      sslope,                   //side slope of channel bank (dimensionless)
      lch,                      //channel length (m) (includes sinuosity)
      twch,                     //top width of channel at flow depth (m)
      achcross,                 //cross section area of channel at flow depth (m2)
      achsurf;                  //surface area of channel portion of a cell (m2)

    //Loop over rows
    for (i = 1; i <= nrows; i++)
    {
        //Loop over columns
        for (j = 1; j <= ncols; j++)
        {
            //Compute cumulative rainfall, interception, and infiltration...
            //
            //if the cell is in the domain
            if (imask[i][j] != nodatavalue)
            {
                //Cumulative gross rainfall volume (m3)
                totalgrossrainvol = totalgrossrainvol + grossrainvol[i][j];

                //Cumulative net rainfall volume (m3)
                totalnetrainvol = totalnetrainvol + netrainvol[i][j];

                //Cumulative interception volume (m3)
                totalinterceptvol = totalinterceptvol + interceptionvol[i][j];

                //if infiltration is simulated
                if (infopt > 0)
                {
                    //Cumulative infiltration volume (m3)
                    totalinfiltvol = totalinfiltvol + infiltrationvol[i][j];

                }               //end if infopt > 0

                //Compute cumulative sum of external flow volumes entering domain (m3)
                totalqwov = totalqwov + qwovvol[i][j];

                //Compute cumulative water volume in the overland plane (m3)
                //
                //if the cell is an overland cell
                if (imask[i][j] == 1)
                {
                    //No channel exists, channel surface area is zero
                    achsurf = 0.0;

                }
                //else if the cell is a channel cell
                else if (imask[i][j] == 2)
                {
                    //Get channel link and node
                    chanlink = link[i][j];
                    channode = node[i][j];

                    //Compute area of channel within cell...
                    //
                    //Assign channel characteristics
                    lch = chanlength[chanlink][channode];       //channel length (m) (includes sinuosity)
                    twch = twidth[chanlink][channode];  //channel top width at bank height (m)

                    //Surface area of channel portion of cell (m2)
                    achsurf = twch * lch;

                }               //end if imask == 1

                //Add water volume from overland portion of channel cell (m3)
                finalwaterov[i][j] = hov[i][j] * (w * w - achsurf);

                //Compute cumulative volume of water (m3)
                finalwaterovvol = finalwaterovvol + finalwaterov[i][j];

            }                   //end if cell is in domain

        }                       //end loop over columns

    }                           //end loop over rows

    //Loop over number of outlets
    for (i = 1; i <= noutlets; i++)
    {
        //Compute cumulative sum of flow volumes exiting domain (m3)
        totalqoutov = totalqoutov + qoutovvol[i];

    }                           //end loop over outlets

    //if channels are simulated
    if (chnopt == 1)
    {
        //Loop over links
        for (i = 1; i <= nlinks; i++)
        {
            //Compute cumulative sum of flow volumes entering domain (m3)
            totalqinch = totalqinch + qinchvol[i];

            //Compute cumulative sum of flow volumes exiting domain (m3)
            totalqoutch = totalqoutch + qoutchvol[i];

            //Loop over nodes
            for (j = 1; j <= nnodes[i]; j++)
            {
                //Compute cumulative sum of external flow volumes entering domain (m3)
                totalqwch = totalqwch + qwchvol[i][j];

                //if channel transmission loss is simulated
                if (ctlopt > 0)
                {
                    //Cumulative transmission loss volume (m3)
                    totaltranslossvol = totaltranslossvol
                        + translossvol[i][j];

                }               //end if ctlopt > 0

                //Compute area of channel within cell...
                //
                //Assign channel flow depth
                hflow = hch[i][j];      //channel water depth (m)

                //Assign channel characteristics
                bwch = bwidth[i][j];    //channel bottom width (m)
                hbch = hbank[i][j];     //channel bank height (m)
                sslope = sideslope[i][j];       //channel side slope
                lch = chanlength[i][j]; //channel length (m) (includes sinuosity)

                //if flow depth <= bank height (cell is not flooded)
                if (hflow <= hbch)
                {
                    //channel cross-sectional area at flow depth (m2)
                    achcross = (float) (bwch + sslope * hflow) * hflow;
                }
                else            //cell is flooded
                {
                    //channel top width of channel at top of bank (m)
                    twch = twidth[i][j];

                    //channel cross-sectional area at top of bank
                    //plus retangular area above the bank height (m2)
                    achcross = (bwch + sslope * hbch) * hbch
                        + (hflow - hbch) * twch;

                }               //end if hflow <= hbch

                //channel volume at flow depth
                finalwaterch[i][j] = achcross * lch;

                //Compute cumulative water volume in the channel network (m3)
                finalwaterchvol = finalwaterchvol + finalwaterch[i][j];

            }                   //end loop over nodes

        }                       //end loop over links

    }                           //end if chnopt == 1

//End of function: Return to ComputeFinalState
}
