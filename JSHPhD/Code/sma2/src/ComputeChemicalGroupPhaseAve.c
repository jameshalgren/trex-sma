/*----------------------------------------------------------------------
C-  Function:	ComputeChemicalGroupPhaseAve.c
C-
C-	Purpose/	Compute averages of chemical state variable phase
C-	Methods:	fractions for chemical reporting groups as specified
C-              in Data Group D.
C-
C-
C-	Inputs:		igrid, jgrid, ilayer 
C-
C-	Outputs:	cgroupave[]
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
C-	Date:		18-FEB-2005
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

void ComputeChemicalGroupPhaseAve(float *cgroupave, int igrid, int jgrid, int ilayer, int iphase)
{
    //Local variable declarations/definitions
    int igroup,                 //local counter for reporting group number
      ichem,                    //local counter for chemical type
      isolid,                   //local counter for solids type
      chanlink,                 //link number of channel in channel cell
      channode;                 //node number of channel in channel cell

    float fraction,             //fraction of chemical in phase iphase
      total;                    //total chemical concentration

    //initialize chemical group sums...
    //
    //loop over chemical groups
    for (igroup = 1; igroup <= ncgroups; igroup++)
    {
        //set group values to zero
        cgroupave[igroup] = 0.0;

    }                           //end loop over chemical groups

    //initialize the phase fraction
    fraction = 0.0;

    //initialize the total concentration
    total = 0.0;

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
                fraction = fraction
                    + fdissolvedch[ichem][chanlink][channode][ilayer]
                    * cchemch[ichem][chanlink][channode][ilayer];
            }
            //else if the phase = 2 (bound)
            else if (iphase == 2)
            {
                //the fraction is the dissolved fraction
                fraction = fraction
                    + fboundch[ichem][chanlink][channode][ilayer]
                    * cchemch[ichem][chanlink][channode][ilayer];
            }
            //else if the phase = 3 (mobile = dissolved + bound)
            else if (iphase == 3)
            {
                //the fraction is the mobile fraction
                fraction = fraction
                    + (fdissolvedch[ichem][chanlink][channode][ilayer]
                       + fboundch[ichem][chanlink][channode][ilayer])
                    * cchemch[ichem][chanlink][channode][ilayer];
            }
            //else if the phase = 4 (particulate)
            else if (iphase == 4)
            {
                //loop over solids
                for (isolid = 1; isolid <= nsolids; isolid++)
                {
                    //the fraction is the sum of particulate fractions
                    fraction = fraction
                        +
                        fparticulatech[ichem][isolid][chanlink][channode]
                        [ilayer] * cchemch[ichem][chanlink][channode][ilayer];

                }               //end loop over solids
            }
            else                //else if the phase = 0 (total chemical)
            {
                //the fraction is the total
                fraction = fraction
                    + cchemch[ichem][chanlink][channode][ilayer];

            }                   //end if iphase = 1

            //compute the total concentration
            total = total + cchemch[ichem][chanlink][channode][ilayer];

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
                fraction = fraction
                    + fdissolvedov[ichem][igrid][jgrid][ilayer]
                    * cchemov[ichem][igrid][jgrid][ilayer];
            }
            //else if the phase = 2 (bound)
            else if (iphase == 2)
            {
                //the fraction is the dissolved fraction
                fraction = fraction
                    + fboundov[ichem][igrid][jgrid][ilayer]
                    * cchemov[ichem][igrid][jgrid][ilayer];
            }
            //else if the phase = 3 (mobile = dissolved + bound)
            else if (iphase == 3)
            {
                //the fraction is the mobile fraction
                fraction = fraction
                    + (fdissolvedov[ichem][igrid][jgrid][ilayer]
                       + fboundov[ichem][igrid][jgrid][ilayer])
                    * cchemov[ichem][igrid][jgrid][ilayer];
            }
            //else if the phase = 4 (particulate)
            else if (iphase == 4)
            {
                //loop over solids
                for (isolid = 1; isolid <= nsolids; isolid++)
                {
                    //the fraction is the sum of particulate fractions
                    fraction = fraction
                        + fparticulateov[ichem][isolid][igrid][jgrid][ilayer]
                        * cchemov[ichem][igrid][jgrid][ilayer];

                }               //end loop over solids
            }
            else                //else if the phase = 0 (total chemical)
            {
                //the fraction is the total
                fraction = fraction + cchemov[ichem][igrid][jgrid][ilayer];

            }                   //end if iphase = 1

            //compute the total concentration
            total = total + cchemov[ichem][igrid][jgrid][ilayer];

        }                       //end loop over chemicals

    }                           //end if imask[][] > 1

    //if the total concentration is greater than zero
    if (total > 0.0)
    {
        //loop over chemical groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //compute average phase fraction of each chemical in the reporting group
            cgroupave[igroup] = fraction / total;

        }                       //end loop over chemical groups
    }
    else                        //else the total concentration is zero (it can't be < 0)
    {
        //loop over chemical groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //set the average phase fraction to zero
            cgroupave[igroup] = 0.0;

        }                       //end loop over chemical groups

    }                           //end if total > 0.0

//End of function: Return to WriteGridsChemical
}
