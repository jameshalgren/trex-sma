/*----------------------------------------------------------------------
C-  Function:	ComputeSolidsGroupConcSums.c
C-
C-	Purpose/	Compute sums of solids state variable concentrations
C-  Methods:    for total solids and other solids reporting groups as
C-              specified in Data Group C.
C-
C-
C-	Inputs:		igrid, jgrid, ilayer 
C-
C-	Outputs:	sgroupsum[]
C-
C-	Controls:   nsolids, nsgroups, imask[][]
C-
C-	Calls:		none
C-
C-	Called by:	WriteGridsSolids
C-
C-	Created:	Mark Velleux
C-				Department of Civil Engineering
C-              Colorado State University
C-				Fort Collins, CO 80523
C-
C-	Date:		24-APR-2004
C-
C-	Revisions:
C-
C-	Date:		
C-
C-	Revisions:
C-
C-  Date:
C-
C---------------------------------------------------------------------*/

//trex global variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for water transport
#include "trex_water_declarations.h"

//trex global variable declarations for sediment transport
#include "trex_solids_declarations.h"

void ComputeSolidsGroupConcSums(float *sgroupsum, int igrid, int jgrid, int ilayer)
{
    //Local variable declarations/definitions
    int igroup,                 //local counter for reporting group number
      isolid,                   //local counter for solids type
      chanlink,                 //link number of channel in channel cell
      channode;                 //node number of channel in channel cell

    //initialize solids group sums...
    //
    //loop over solids groups (include zero for sum of all solids)
    for (igroup = 0; igroup <= nsgroups; igroup++)
    {
        //set group values to zero
        sgroupsum[igroup] = 0.0;

    }                           //end loop over solids groups

    //if the cell is a channel cell
    if (imask[igrid][jgrid] > 1)
    {
        //set channel link number
        chanlink = link[igrid][jgrid];

        //set channel node number
        channode = node[igrid][jgrid];

        //loop over solids
        for (isolid = 1; isolid <= nsolids; isolid++)
        {
            //get group number for this solids type
            igroup = sgroupnumber[isolid];

            //sum total solids
            sgroupsum[0] = sgroupsum[0]
                + csedch[isolid][chanlink][channode][ilayer];

            //add the concentration of each solids type to its reporting group
            sgroupsum[igroup] = sgroupsum[igroup]
                + csedch[isolid][chanlink][channode][ilayer];

        }                       //end loop over solids
    }
    else                        //else the cell is an overland cell
    {
        //loop over solids
        for (isolid = 1; isolid <= nsolids; isolid++)
        {
            //get group number for this solids type
            igroup = sgroupnumber[isolid];

            //sum total solids
            sgroupsum[0] =
                sgroupsum[0] + csedov[isolid][igrid][jgrid][ilayer];

            //add the concentration of each solids type to its reporting group
            sgroupsum[igroup] = sgroupsum[igroup]
                + csedov[isolid][igrid][jgrid][ilayer];

        }                       //end loop over solids

    }                           //end if imask[][] > 1

//End of function: Return to WriteGridsSolids
}
