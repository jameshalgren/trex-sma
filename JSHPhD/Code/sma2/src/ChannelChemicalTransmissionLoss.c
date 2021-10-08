/*---------------------------------------------------------------------
C-  Function:	ChannelChemicaltransmissionLoss.c
C-
C-	Purpose/	Computes the transmission loss flux of chemicals in the
C-	Methods:	channel network (chemical tranmission loss to sediment).
C-
C-				Note: Chemical tranmission loss is not linked to the
C-				      sediment column.  The transmission loss flux is
C-				      used to compute the flux of chemicals entering
C-				      the surface sediment layer.  However, sediment
C-				      chemical concentrations are not updated based on
C-                    the flux.
C-
C-
C-  Inputs:	    translossrate[][], fdissolvedch[][][][],
C-              fboundch[][][][]
C-
C-  Outputs:    infchemchoutflux[][][][][], infchemchinflux[][][][][]
C-
C-  Controls:   imask[][]
C-
C-  Calls:      None
C-
C-  Called by:  ChemicalTransport
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
C-	Date:		17-JUN-2005
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

//trex global variable declarations for chemical transport
#include "trex_chemical_declarations.h"

//trex global variable declarations for environmental conditions
#include "trex_environmental_declarations.h"

void ChannelChemicalTransmissionLoss(void)
{
    //local variable declarations/definitions
    int ichem,                  //loop index for chemical type
      ilayer;                   //index for surface layer in the soil stack

    float bwch,                 //bottom width of channel (m)
      hbch,                     //bank height of channel (m)
      sslope,                   //side slope of channel bank (dimensionless)
      twch,                     //top width of channel at top of bank (m)
      achcross,                 //cross sectional area of channel (m2)
      watervol,                 //water column volume of node (at time t) (m3)
      abed,                     //surface area of sediment bed
      fmobile;                  //sum of mobile fractions (dissolved + bound)

    double potential,           //chemical infiltration mass potential (g)
      available;                //chemical mass available for transport (g)

    //loop over links
    for (i = 1; i <= nlinks; i++)
    {
        //loop over nodes
        for (j = 1; j <= nnodes[i]; j++)
        {
            //Flux from the sediment surface (transmission loss)
            //
            //Note:  Leaching transports dissolved and bound phases
            //
            //Assign channel characteristics
            bwch = bwidth[i][j];        //bottom width (m)
            hbch = hbank[i][j]; //bank height (m)
            sslope = sideslope[i][j];   //side slope (dimensionless)

            //Compute present water volume...
            //
            //if the present flow depth <= bank height
            if (hch[i][j] <= hbch)
            {
                //Cross-sectional area (m2)
                achcross = (bwch + sslope * hch[i][j]) * hch[i][j];
            }
            else                //else flow depth greater than channel depth
            {
                //Channel top width at top of bank (m)
                twch = twidth[i][j];

                //Cross-sectional area (m2)
                achcross = (bwch + sslope * hbch) * hbch
                    + (hch[i][j] - hbch) * twch;

            }                   //end if hch <= bank height

            //compute present water column volume (m3) (at time t)
            watervol = achcross * chanlength[i][j];

            //set the surface layer number
            ilayer = nstackch[i][j];

            //set surface area of bed (m2)
            abed = achbed[i][j][ilayer];

            //loop over number of chemical types
            for (ichem = 1; ichem <= nchems; ichem++)
            {
                //compute mobile fraction (sum of dissolved and bound)
                fmobile =
                    fdissolvedch[ichem][i][j][0] + fboundch[ichem][i][j][0];

                //Developer's Note:  Also, for consistency with code for overland
                //                   chemical infiltration, flux and mass tracking
                //                   variables for channel chemical transmission
                //                   loss are infchemch (for infiltration) rather
                //                   than trlchemch (for transmission loss).  As
                //                   needed the variable names can be changed by
                //                   a global search and replace...
                //                   
                //compute the leaching flux leaving the water column (g/s)
                infchemchoutflux[ichem][i][j][0] = translossrate[i][j]
                    * abed * cchemch[ichem][i][j][0] * fmobile;

                //compute the deposition mass potential (g)
                potential = infchemchoutflux[ichem][i][j][0] * dt[idt];

                //Developer's Note:  When computing the mass available
                //                   for transmission loss, the mass
                //                   lost to transformation processes
                //                   should only include the mass lost
                //                   from the mobile phase.  This will
                //                   require further code development.
                //
                //compute the mass available in the water column (g)
                available = watervol * cchemch[ichem][i][j][0] * fmobile
                    - (biochemchoutflux[ichem][i][j][0]
                       + hydchemchoutflux[ichem][i][j][0]
                       + oxichemchoutflux[ichem][i][j][0]
                       + phtchemchoutflux[ichem][i][j][0]
                       + radchemchoutflux[ichem][i][j][0]
                       + vltchemchoutflux[ichem][i][j][0]
                       + udrchemchoutflux[ichem][i][j][0]) * dt[idt];

                //if the available mass < 0.0
                if (available < 0.0)
                {
                    //set the available mass (g) to zero
                    available = 0.0;

                }               //end if available < 0.0

                //if the deposition potential > available mass
                if (potential > available)
                {
                    //scale the chemical infiltration flux leaving the water column (g/s)
                    infchemchoutflux[ichem][i][j][0] =
                        (float) (available / dt[idt]);

                }               //end if potential > available

                //compute the leaching flux entering the surface sediment layer (g/s)
                infchemchinflux[ichem][i][j][ilayer] =
                    infchemchoutflux[ichem][i][j][0];

            }                   //end loop over number of chemical types

        }                       //end loop over nodes

    }                           //end loop over links

//End of function: Return to ChemicalTransport
}
