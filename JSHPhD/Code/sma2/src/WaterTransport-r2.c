/*----------------------------------------------------------------------
C-  Function:	WaterTransport
C-
C-  Purpose/    WaterTransport is called to compute derivative
C-  Methods:    terms (rates) for water transport processes:
C-              rainfall, snowmelt, interception, infiltration,
C-              overland flow/routing, and channel flow/routing.
C-
C-  Inputs:     None
C-
C-  Outputs:    None
C-
C-  Controls:   infopt, chnopt, ctlopt
C-
C-  Calls:      Rainfall,
C-              Interception,
C-              Infiltration,
C-              OverlandWaterRoute,
C-              TransmissionLoss
C-              ChannelWaterRoute,
C-              FloodplainWaterTransfer
C-
C-  Called by:  trex (main)
C-
C-  Created:    Mark Velleux (CSU)
C-              John England (USBR)
C-
C-  Date:       19-AUG-2003
C-
C-  Revised:	Mark Velleux (CSU)
C-
C-	Date:		05-MAY-2005
C-
C-	Revisions:	Added call for floodplain water transfer process
C-              module (FloodplainWaterTransfer)
C-
C-  Revised:	Mark Velleux (CSU)
C-              James Halgren
C-
C-	Date:		12-MAR-2007
C-
C-	Revisions:	Added call for channel transmission loss process
C-              module (TransmissionLoss)
C-                      Added calls to SMA procedures
C-
C-----------------------------------------------------------------------*/

//trex global variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for water transport
#include "trex_water_declarations.h"

//variable declarations and definitions specific to the 
//groundwater accounting procedure.
#include "trex_SMA_declarations.h"

void WaterTransport(void)
{
    //Compute gross rainfall rate (intensity)
    Rainfall ();

    //Compute interception and net rainfall rate
    Interception ();

//jfe   //if snowpack/snowmelt is simulated
//jfe   if(snowopt > 0)
//jfe   {
//jfe           //Compute snowmelt rate (expressed as equivalent rate of water addition)
//jfe           Snowmelt();
//jfe   }

    //if infiltration is simulated
    if (infopt > 0)
    {
        //Compute infiltration rate (average over integration time step)
        Infiltration ();

        //if soil moisture accounting will be performed
        if (infopt == 2)
        {
            //Compute infiltration rate and perform Soil Moisture Accounting
            percolationSMA ();
        }
    }

    //Compute overland flows and routing
    OverlandWaterRoute ();

    //if channels are simulated
    if (chnopt > 0)
    {
        //if channel transmission loss is simulated
        if (ctlopt > 0)
        {
            //Compute channel transmission loss rate
            TransmissionLoss ();

        }                       //end if ctlopt > 0

        //Compute channel flows and routing
        ChannelWaterRoute ();

        //Compute water transfers between overland plane and channels
        FloodplainWaterTransfer ();
    }

//End of function: Return to trex
}
