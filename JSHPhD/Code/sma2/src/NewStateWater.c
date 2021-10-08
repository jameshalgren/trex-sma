/*----------------------------------------------------------------------
C-  Function:	NewStateWater.c
C-
C-	Purpose/	
C-	Methods:	NewState is called to store new water depths for use
C-              during the next (upcoming) time step (t + dt).
C-
C-	Inputs:		None
C-
C-	Outputs:	None
C-
C-	Controls:	ksim, chnopt
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

void NewStateWater(void)
{
    //Set overland depth conditions...
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
                //set overland depths (m) (for use at time t + dt)
                hov[i][j] = hovnew[i][j];

                //Determine the minimum and the maximum water depths
                minhovdepth = Min (minhovdepth, hov[i][j]);
                maxhovdepth = Max (maxhovdepth, hov[i][j]);

            }                   //end if the cell is in the domain (imask != nodatavalue)

        }                       //end loop over columns

    }                           //end loop over rows

    //if channels are simulated
    if (chnopt > 0)
    {
        //Set channel depth conditions...
        //
        //Loop over links
        for (i = 1; i <= nlinks; i++)
        {
            //Loop over nodes
            for (j = 1; j <= nnodes[i]; j++)
            {
                //set channel depths (m) (for use at time t + dt)
                hch[i][j] = hchnew[i][j];

                //Determine the minimum and the maximum water depths in channels
                minhchdepth = Min (minhchdepth, hch[i][j]);
                maxhchdepth = Max (maxhchdepth, hch[i][j]);

            }                   //end loop over nodes

        }                       //end loop over links

    }                           //end if chnopt > 0

//End of function: Return to NewState
}
