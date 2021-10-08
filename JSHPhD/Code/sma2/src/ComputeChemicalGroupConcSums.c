/*----------------------------------------------------------------------
C-  Function:	ComputeChemicalGroupConcSums.c
C-
C-	Purpose/	Compute sums of chemical state variable concentrations
C-  Methods:    for chemical reporting groups as specified in Data
C-              Group D.
C-
C-
C-	Inputs:		igrid, jgrid, ilayer 
C-
C-	Outputs:	cgroupsum[]
C-
C-	Controls:   nchems, ncgroups, imask[][]
C-
C-	Calls:		none
C-
C-	Called by:	WriteGridsChemical
C-
C-	Created:	Mark Velleux
C-				Department of Civil Engineering
C-              Colorado State University
C-				Fort Collins, CO 80523
C-
C-	Date:		12-SEP-2004
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

//trex global variable declarations for chemical transport
#include "trex_chemical_declarations.h"

void ComputeChemicalGroupConcSums(float *cgroupsum, int igrid, int jgrid, int ilayer, int iphase)
{
    //Local variable declarations/definitions
    int igroup,                 //local counter for reporting group number
      ichem,                    //local counter for chemical type
      isolid,                   //local counter for solids type
      chanlink,                 //link number of channel in channel cell
      channode;                 //node number of channel in channel cell

    float fraction;             //fraction of chemical in phase iphase

    //initialize chemical group sums...
    //
    //loop over chemical groups
    for (igroup = 1; igroup <= ncgroups; igroup++)
    {
        //set group values to zero
        cgroupsum[igroup] = 0.0;

    }                           //end loop over chemical groups

    //if the cell is a channel cell
    if (imask[igrid][jgrid] > 1)
    {
        //set channel link number
        chanlink = link[igrid][jgrid];

        //set channel node number
        channode = node[igrid][jgrid];

        //loop over chemicals
        for (ichem = 1; ichem <= nchems; ichem++)
        {
            //get group number for this chemical type
            igroup = cgroupnumber[ichem];

            //Set phase/fraction of chemical
            //
            //if the phase = 1 (dissolved)
            if (iphase == 1)
            {
                //the fraction is the dissolved fraction
                fraction = fdissolvedch[ichem][chanlink][channode][ilayer];
            }
            //else if the phase = 2 (bound)
            else if (iphase == 2)
            {
                //the fraction is the dissolved fraction
                fraction = fboundch[ichem][chanlink][channode][ilayer];
            }
            //else if the phase = 3 (particulate)
            else if (iphase == 3)
            {
                //initialize the fraction
                fraction = 0.0;

                //loop over solids
                for (isolid = 1; isolid <= nsolids; isolid++)
                {
                    //the fraction is the sum of particulate fractions
                    fraction = fraction +
                        fparticulatech[ichem][isolid][chanlink][channode]
                        [ilayer];

                }               //end loop over solids
            }
            else                //else if the phase = 0 (total chemical)
            {
                //the fraction is the total (always 1.0)
                fraction = 1.0;

            }                   //end if iphase = 1

            //add the concentration of each chemical type to its reporting group
            cgroupsum[igroup] = cgroupsum[igroup]
                + cchemch[ichem][chanlink][channode][ilayer] * fraction;

        }                       //end loop over chemicals
    }
    else                        //else the cell is an overland cell
    {
        //loop over chemicals
        for (ichem = 1; ichem <= nchems; ichem++)
        {
            //get group number for this chemical type
            igroup = cgroupnumber[ichem];

            //Set phase/fraction of chemical
            //
            //if the phase = 1 (dissolved)
            if (iphase == 1)
            {
                //the fraction is the dissolved fraction
                fraction = fdissolvedov[ichem][igrid][jgrid][ilayer];
            }
            //else if the phase = 2 (bound)
            else if (iphase == 2)
            {
                //the fraction is the dissolved fraction
                fraction = fboundov[ichem][igrid][jgrid][ilayer];
            }
            //else if the phase = 3 (particulate)
            else if (iphase == 3)
            {
                //initialize the fraction
                fraction = 0.0;

                //loop over solids
                for (isolid = 1; isolid <= nsolids; isolid++)
                {
                    //the fraction is the sum of particulate fractions
                    fraction = fraction +
                        fparticulateov[ichem][isolid][igrid][jgrid][ilayer];

                }               //end loop over solids
            }
            else                //else if the phase = 0 (total chemical)
            {
                //the fraction is the total (always 1.0)
                fraction = 1.0;

            }                   //end if iphase = 1

            //add the concentration of each chemical type to its reporting group
            cgroupsum[igroup] = cgroupsum[igroup]
                + cchemov[ichem][igrid][jgrid][ilayer] * fraction;

        }                       //end loop over chemicals

    }                           //end if imask[][] > 1

//End of function: Return to WriteGridsChemical
}
