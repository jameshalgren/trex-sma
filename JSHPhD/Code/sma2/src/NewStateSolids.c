/*----------------------------------------------------------------------
C-  Function:	NewStateSolids.c
C-
C-	Purpose/	NewStateSolids is called to store new particle
C-	Methods:	concentrations for use during the next (upcoming)
C-				time step (t + dt).
C-
C-	Inputs:		None
C-
C-	Outputs:	None
C-
C-	Controls:	chnopt
C-
C-	Calls:		None
C-
C-	Called by:	NewState
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
C---------------------------------------------------------------------*/

//trex global variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for water transport
#include "trex_water_declarations.h"

//trex global variable declarations for sediment transport
#include "trex_solids_declarations.h"

void NewStateSolids(void)
{
    //local variable declarations/definitions
    int isolid,                 //loop index for solids number (type)
      ilayer;                   //loop index for surface layer

    float totalsolids0,         //total solids concentration in water
      totalsolids1;             //total solids concentration in surface layer

    //Loop over rows
    for (i = 1; i <= nrows; i++)
    {
        //Loop over columns
        for (j = 1; j <= ncols; j++)
        {
            //If the cell is in the domain
            if (imask[i][j] > 0)
            {
                //set layer number for surface soil layer
                ilayer = nstackov[i][j];

                //loop over solids
                for (isolid = 1; isolid <= nsolids; isolid++)
                {
                    //set overland solids concentration in water column (g/m3) (for use at time t + dt)
                    csedov[isolid][i][j][0] = csedovnew[isolid][i][j][0];

                    //Determine the minimum and the maximum solids concentrations in water
                    mincsedov0[isolid] =
                        Min (mincsedov0[isolid], csedov[isolid][i][j][0]);
                    maxcsedov0[isolid] =
                        Max (maxcsedov0[isolid], csedov[isolid][i][j][0]);

                    //set overland solids concentration in surface sediment (g/m3) (for use at time t + dt)
                    csedov[isolid][i][j][ilayer] =
                        csedovnew[isolid][i][j][ilayer];

                    //Determine the minimum and the maximum solids concentrations in the surface soil layer
                    mincsedov1[isolid] =
                        Min (mincsedov1[isolid],
                             csedov[isolid][i][j][ilayer]);
                    maxcsedov1[isolid] =
                        Max (maxcsedov1[isolid],
                             csedov[isolid][i][j][ilayer]);

                }               //end loop over solids

                //Compute sum of all solids types...
                //
                //set total solids concentrations to zero
                totalsolids0 = 0.0;
                totalsolids1 = 0.0;

                //loop over solids
                for (isolid = 1; isolid <= nsolids; isolid++)
                {
                    //compute the total solids in the water column
                    totalsolids0 = totalsolids0 + csedov[isolid][i][j][0];

                    //compute the total solids in the surface layer
                    totalsolids1 =
                        totalsolids1 + csedov[isolid][i][j][ilayer];

                }               //end loop over solids

                //Determine the minimum and the maximum total solids concentrations in water
                mincsedov0[0] = Min (mincsedov0[0], totalsolids0);
                maxcsedov0[0] = Max (maxcsedov0[0], totalsolids0);

                //Determine the minimum and the maximum total solids concentrations in the surface layer
                mincsedov1[0] = Min (mincsedov1[0], totalsolids1);
                maxcsedov1[0] = Max (maxcsedov1[0], totalsolids1);

            }                   //end if the cell is in the domain (imask > 0)

        }                       //end loop over columns

    }                           //end loop over rows

    //if channels are simulated
    if (chnopt > 0)
    {
        //Loop over links
        for (i = 1; i <= nlinks; i++)
        {
            //Loop over nodes
            for (j = 1; j <= nnodes[i]; j++)
            {
                //set layer number for surface sediment layer
                ilayer = nstackch[i][j];

                //loop over solids
                for (isolid = 1; isolid <= nsolids; isolid++)
                {
                    //set channel solids concentration in water column (g/m3) (for use at time t + dt)
                    csedch[isolid][i][j][0] = csedchnew[isolid][i][j][0];

                    //Determine the minimum and the maximum solids concentrations in water
                    mincsedch0[isolid] =
                        Min (mincsedch0[isolid], csedch[isolid][i][j][0]);
                    maxcsedch0[isolid] =
                        Max (maxcsedch0[isolid], csedch[isolid][i][j][0]);

                    //set channel solids concentration in surface sediment (g/m3) (for use at time t + dt)
                    csedch[isolid][i][j][ilayer] =
                        csedchnew[isolid][i][j][ilayer];

                    //Determine the minimum and the maximum solids concentrations in surface sediment
                    mincsedch1[isolid] =
                        Min (mincsedch1[isolid],
                             csedch[isolid][i][j][ilayer]);
                    maxcsedch1[isolid] =
                        Max (maxcsedch1[isolid],
                             csedch[isolid][i][j][ilayer]);

                }               //end loop over solids

                //Compute sum of all solids types...
                //
                //set total solids concentrations to zero
                totalsolids0 = 0.0;
                totalsolids1 = 0.0;

                //loop over solids
                for (isolid = 1; isolid <= nsolids; isolid++)
                {
                    //compute the total solids in the water column
                    totalsolids0 = totalsolids0 + csedch[isolid][i][j][0];

                    //compute the total solids in the surface layer
                    totalsolids1 =
                        totalsolids1 + csedch[isolid][i][j][ilayer];

                }               //end loop over solids

                //Determine the minimum and the maximum total solids concentrations in water
                mincsedch0[0] = Min (mincsedch0[0], totalsolids0);
                maxcsedch0[0] = Max (maxcsedch0[0], totalsolids0);

                //Determine the minimum and the maximum total solids concentrations in the surface layer
                mincsedch1[0] = Min (mincsedch1[0], totalsolids1);
                maxcsedch1[0] = Max (maxcsedch1[0], totalsolids1);

            }                   //end loop over nodes

        }                       //end loop over links

    }                           //end if chnopt > 0

//End of function: Return to NewState
}
