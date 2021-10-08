/*---------------------------------------------------------------------
C-  Function:   Infiltration.c
C-
C-  Purpose/    Infiltration.c computes the infiltration rate
C-  Methods:    and cumulative depth of infiltration for each
C-              cell in the overland plane.  Uses the Green-Ampt
C-              equation neglecting a term for head for the ponded
C-              water depth.
C-
C-              After performing infiltration computations
C-              Infiltration.c computes the new infiltration depth
C-              which is used for soil moisture accounting in the 
C-              upper soil zone and initializes lower zone values
C-              using a method similar to the Sacramento Soil Moisture
C-              Accounting procedure.
C-
C-  Inputs:     hov[][] (at time t),
C-              infiltrationdepth[][] (at time t),
C-              soiltype[][][]
C-
C-  Outputs:    infiltrationrate[][] (at time t),
C-              infiltrationdepth[][] (at time t+dt)
C-              infiltrationvol[][] (at time t+dt)
C-
C-  Controls:   hov[][] (at time t)
C-
C-  Calls:      None
C-
C-  Called by:  WaterBalance
C-
C-  Created:    P. Y. Julien, B. Saghafian, B. Johnson,
C-              and R. Rojas (CSU)
C-
C-  Date:       1991
C-
C-  Revised:    Mark Velleux (CSU)
C-              John England (USBR)
C-              James Halgren 
C-
C-  Date:       03-MAR-2007
C-
C----------------------------------------------------------------------*/

//trex global variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for water transport
#include "trex_water_declarations.h"

//variable declarations and definitions specific to the 
//soil moisture acccounting procedure.
#include "trex_SMA_declarations.h"

void Infiltration (void)
{
    //local variable declarations/definitions
    int ilayer,                 //index for layer in the soil stack (ksim > 1)
      isoil,                    //index for soil type [row][col][layer]
      chanlink,                 //channel link number
      channode;                 //channel node number

    float lch,                  //channel length (m) (includes sinuosity)
      twch,                     //top width of channel at flow depth (m)
      achsurf;                  //surface area of channel (m2)

    float p1,                   //first term in average infiltration rate equation
      p2;                       //second term in average infiltration rate equation

    //JSH DEL Temporarily initialize infiltration depth counters to debug wnew problem.
    mininfiltdepth = 0;
    maxinfiltdepth = 0;
    //Loop over rows
    for (i = 1; i <= nrows; i++)
    {
        //Loop over columns
        for (j = 1; j <= ncols; j++)
        {
            //if the cell is in the domain
            if (imask[i][j] != nodatavalue)
            {
                //Note:  nstackov[][] always = 1 when ksim = 1...
                //
                //set the soil layer number for this location
                ilayer = nstackov[i][j];

                //set the soil type for this location
                isoil = soiltype[i][j][ilayer];

                //set first and second terms for infiltration rate equation
                p1 = (float) (khsoil[isoil] * dt[idt] -
                              2.0 * infiltrationdepth[i][j]);
                p2 = khsoil[isoil] * (infiltrationdepth[i][j] +
                                      capshsoil[isoil] * soilmd[isoil]);

                //Compute infiltration rate (m/s)
                infiltrationrate[i][j] =
                    (float) ((p1 +
                              sqrt (pow (p1, 2.0) +
                                    8.0 * p2 * dt[idt])) / (2.0 * dt[idt]));

                //check if the infiltration potential exceeds the available water
                if (infiltrationrate[i][j] * dt[idt] > hov[i][j])
                {
                    //limit infiltration rate to available water supply
                    infiltrationrate[i][j] = hov[i][j] / dt[idt];

                }               //end check of infiltration potential

                //Compute the cumulative depth of infitration
                infiltrationdepth[i][j] = infiltrationdepth[i][j]
                    + infiltrationrate[i][j] * dt[idt];

                //Compute minimum infiltration depth (cumulative) for cell
                mininfiltdepth =
                    Min (infiltrationdepth[i][j], mininfiltdepth);

                //Compute maximum infiltration depth (cumulative) for cell
                maxinfiltdepth =
                    Max (infiltrationdepth[i][j], maxinfiltdepth);

                //if the cell is a channel cell
                if (imask[i][j] > 1)
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
                }
                else            //cell is not a channel cell (overland only)
                {
                    //no channel present, surface area is zero
                    achsurf = 0.0;

                }               //end if imask > 1

                //Compute cumulative infiltration volume for this cell (m3)
                infiltrationvol[i][j] = infiltrationvol[i][j] +
                    infiltrationrate[i][j] * dt[idt] * (w * w - achsurf);
                if (infopt == 2)
                {
                    SMAinfiltrationvol[i][j] =
                        infiltrationrate[i][j] * dt[idt];
                    SMAprecipvol[i][j] = netrainrate[i][j] * dt[idt];
                }               //end if infopt == 2

            }                   //end if cell is in domain

        }                       //end loop over columns

    }                           //end loop over rows

//End of function: Return to WaterTransport
}
