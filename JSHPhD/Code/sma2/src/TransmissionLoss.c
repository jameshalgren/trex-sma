/*---------------------------------------------------------------------
C-  Function:	TransmissionLoss.c
C-
C-  Purpose/    TransmissionLoss computes the rate and cumulative
C-  Methods:    depth of transmission loss (infiltration) for each
C-              node in the chennel network.  Uses the Green-Ampt
C-              equation including a term for the ponded (channel)
C-              water depth.  Transmission loss is computed for the
C-              channel bed surface area only (since Green-Ampt only
C-              applies in the z-direction) and excludes bank loss.
C-
C-
C-  Inputs:     hch[][] (at time t),
C-              translossdepth[][] (at time t),
C-              khbed[][], capshsed[][], sedmd[][]
C-
C-  Outputs:    translossrate[][] (at time t),
C-              translossdepth[][] (at time t+dt)
C-              translossvol[][] (at time t+dt)
C-
C-  Controls:   hch[][] (at time t)
C-
C-  Calls:      None
C-
C-  Called by:  WaterBalance
C-
C-  Created:	Mark Velleux
C-              Department of Civil Engineering
C-              Colorado State University
C-              Fort Collins, CO  80523
C-
C-  Date:       03-JUN-2005
C-
C-  Revised:
C-
C-  Revisions:
C-
C-  Date:
C-
C----------------------------------------------------------------------*/

//trex global variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for water transport
#include "trex_water_declarations.h"

void TransmissionLoss(void)
{
    //local variable declarations/definitions
    int ilayer;                 //index for layer in the sediment stack (ksim > 1)

    float lch,                  //channel length (m) (includes sinuosity)
      bwch,                     //bottom width of channel (m)
      abed;                     //surface area of channel bed (m2)

    float p1,                   //first term in average transmission loss rate equation
      p2;                       //second term in average transmission loss rate equation

    //Loop over links
    for (i = 1; i <= nlinks; i++)
    {
        //Loop over nodes
        for (j = 1; j <= nnodes[i]; j++)
        {
            //Note:  nstackch[][] always = 1 when ksim = 1...
            //
            //set the sediment layer number for this location
            ilayer = nstackch[i][j];

            //set first and second terms for transmission loss rate equation
            p1 = (float) (khsed[i][j] * dt[idt] - 2.0 * translossdepth[i][j]);

            p2 = khsed[i][j]
                * (translossdepth[i][j] + (hch[i][j] + capshsed[i][j])
                   * sedmd[i][j]);

            //Compute transmission loss rate (m/s)
            translossrate[i][j] =
                (float) ((p1 +
                          sqrt (pow (p1, 2.0) +
                                8.0 * p2 * dt[idt])) / (2.0 * dt[idt]));

            //check if the transmission loss potential exceeds the available water
            if (translossrate[i][j] * dt[idt] > hch[i][j])
            {
                //limit transmission loss rate to available water supply
                translossrate[i][j] = hch[i][j] / dt[idt];

            }                   //end check of transmission loss potential

            //Compute the cumulative depth of transmission loss (m)
            translossdepth[i][j] = translossdepth[i][j]
                + translossrate[i][j] * dt[idt];

            //Compute minimum transmission loss depth (cumulative) for node
            mintranslossdepth = Min (translossdepth[i][j], mintranslossdepth);

            //Compute maximum transmission loss depth (cumulative) for node
            maxtranslossdepth = Max (translossdepth[i][j], maxtranslossdepth);

            //Note:  achbed is a global sediment transport variable that
            //       holds the values of the channel bed surface area (m2).
            //       However, global sediment variables are not known to
            //       hydrology modules so the bed surface area must be
            //       computed locally from bottom width and length.
            //
            //Compute bed area of channel...
            //
            //Assign channel characteristics
            lch = chanlength[i][j];     //channel length (m) (includes sinuosity)
            bwch = twidth[i][j];        //channel bottom width (m)

            //Surface area of channel bed (m2)
            abed = bwch * lch;

            //Compute cumulative transmission loss volume for this node (m3)
            translossvol[i][j] = translossvol[i][j]
                + translossrate[i][j] * abed * dt[idt];

        }                       //end loop over nodes

    }                           //end loop over links

//End of function: Return to WaterTransport
}
