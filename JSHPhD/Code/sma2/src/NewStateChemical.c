/*----------------------------------------------------------------------
C-  Function:	NewStateChemical.c
C-
C-	Purpose/	NewStateChemical is called to store new chemical
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

//trex global variable declarations for chemical transport
#include "trex_chemical_declarations.h"

void NewStateChemical(void)
{
    //local variable declarations/definitions
    int ichem,                  //loop index for solids number (type)
      ilayer;                   //loop index for surface layer

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

                //loop over chemicals
                for (ichem = 1; ichem <= nchems; ichem++)
                {
                    //set overland chemical concentration in water column (g/m3) (for use at time t + dt)
                    cchemov[ichem][i][j][0] = cchemovnew[ichem][i][j][0];

                    //Determine the minimum and the maximum chemical concentrations in water
                    mincchemov0[ichem] =
                        Min (mincchemov0[ichem], cchemov[ichem][i][j][0]);
                    maxcchemov0[ichem] =
                        Max (maxcchemov0[ichem], cchemov[ichem][i][j][0]);

                    //set overland chemical concentration in surface sediment (g/m3) (for use at time t + dt)
                    cchemov[ichem][i][j][ilayer] =
                        cchemovnew[ichem][i][j][ilayer];

                    //Determine the minimum and the maximum chemical concentrations in the surface soil layer
                    mincchemov1[ichem] =
                        Min (mincchemov1[ichem],
                             cchemov[ichem][i][j][ilayer]);
                    maxcchemov1[ichem] =
                        Max (maxcchemov1[ichem],
                             cchemov[ichem][i][j][ilayer]);

                }               //end loop over chemicals

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

                //loop over chemicals
                for (ichem = 1; ichem <= nchems; ichem++)
                {
                    //set channel chemical concentration in water column (g/m3) (for use at time t + dt)
                    cchemch[ichem][i][j][0] = cchemchnew[ichem][i][j][0];

                    //Determine the minimum and the maximum chemical concentrations in water
                    mincchemch0[ichem] =
                        Min (mincchemch0[ichem], cchemch[ichem][i][j][0]);
                    maxcchemch0[ichem] =
                        Max (maxcchemch0[ichem], cchemch[ichem][i][j][0]);

                    //set channel chemical concentration in surface sediment (g/m3) (for use at time t + dt)
                    cchemch[ichem][i][j][ilayer] =
                        cchemchnew[ichem][i][j][ilayer];

                    //Determine the minimum and the maximum chemical concentrations in surface sediment
                    mincchemch1[ichem] =
                        Min (mincchemch1[ichem],
                             cchemch[ichem][i][j][ilayer]);
                    maxcchemch1[ichem] =
                        Max (maxcchemch1[ichem],
                             cchemch[ichem][i][j][ilayer]);

                }               //end loop over chemicals

            }                   //end loop over nodes

        }                       //end loop over links

    }                           //end if chnopt > 0

//End of function: Return NewState
}
