/*---------------------------------------------------------------------
C-  Function:    percolationSMA.c
C-
C-  Purpose/    percolationSMA.c performs soil moisture accounting
C-         using a method similar to the Sacramento Soil Moisture
C-        Accounting procedure.
C-  Methods:    
C-
C-  Inputs:    
C-
C-  Outputs:    
C-
C-  Controls: 
C-
C-  Calls:      None
C-
C-  Called by:  Infiltration
C-
C-  Created:    James Halgren (CSU)
C-         Fred Ogden made significant contributions by providing "Sac-mini.c", created by the 
C-         NWS as a non-Fortran (ANSI C) version of the SAC-SMA routine. 
C-
C-  Date:       Wed Mar  7 11:56:19 MST 2007
C-  Date:       Thu Nov 21 16:55:36 MDT 2008
C-
C-  Revised:    
C-
C-  Date:      
C-
C----------------------------------------------------------------------*/

//trex global variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for water transport
#include "trex_water_declarations.h"

//variable declarations and definitions specific to the 
//soil moisture accounting procedure.
#include "trex_SMA_declarations.h"

void percolationSMA (void)
{
    //JSH ADD This file has been slightly modified in the
    //JSH ADD sma2optimize branch of this code. 

    //Local variable declaration
    //For volume balance checking
    //JSH DEL DO WE NEED THESE???
    double lf1c0, lf1c1, lf1m, lf2c0, lf2c1, lf2m, ltc0, ltc1, ltm;
    double uf1c0, uf1c1, uf1m, uf2c0, uf2c1, uf2m, utc0, utc1, utm;
    //JSH DEL DO WE NEED THESE???

    //Note that the state variables and outputs are passed by reference - JSH
    //All other values are not being modified. - JSH
    //These are the passed variables in the function call to sac_mini - JSH
    double *uztwc               //Upper zone tension water current volume
     , *uzfwctotal              //Total volume of free water in all upper zones
     , uzfwmtotal               //Total volume of free water capacity in all upper zones
     , *lztwc                   //Lower zone tension water current volume
     , *lzfwctotal              //Total volume of free water in all lower zones
     , lzfwmtotal               //Total volume of free water in all lower zones
     , *uzfwc                   //Upper zone free water current volume
     , *lzfwc                   //Lower zone free water current volume
     , uztwm                    //Upper zone tension water capacity
     , uzfwm                    //Upper zone free water capacity
     , lztwm                    //Lower zone tension water capacity
     , lzfwm                    //Lower zone free water capacity
     , saved                    //Lower zone free water not available to resupply tension water
     , zperc                    //Percolocation multiplier applied to pbase
     , rexp                     //Exponent defining percolation change between wet adn dry soils
     , pfree                    //Minimum fraction of percolated water resupplying deep free water
     , *pcp                     //
     , pbase                    //
     , *flowsf                  //
     , *flowin                  //
     , *flowbf                  //
     , *edmnd                   //
     , *e1                      //
     , *e2                      //
     , *e3;                     //

    //These are the locally declared variables in sac_mini - JSH
    // I have modified some to be pointers
    double *perc                //
     , *percdemand              //
     , *pav                     //
     , rperc                    //
     , red                      //
     , a                        //
     , b                        //
     , del                      //
     , duz                      //
     , duzr                     //
     , dlz                      //
     , dlzr                     //
     , check;                   //

    double *F, ratlcmtot        //
     , ratucmtot                //
     , ratumm                   //
     , ratucm                   //
     , ratlmm                   //
     , ratlcm                   //
     , pcpreserve               //
     , percreserve;             //

    double kpart                //
     , kperc;                   //
    double kbfeff               //
     , kifeff;                  //

    /*Variables for Reinitialization code */
    long isoil;                 //Index of soil type. 
    /*Variables for Reinitialization code */

    //JSH DEL These are unused declarations from Fred Ogden Sac-mini.c - JSH
    //JSH DEL double ratlp,dcuz,
    //JSH DEL dclz,  hpl, percs;
    //JSH DEL double dinc, pinc, evap;
    //JSH DEL int skip, ninc, nskip, inc;

    //JSH ADD Fill the upper zone buckets by looping through each cell
    //JSH ADD this loop could occur under a conditional (infopt == 2)  
    //JSH ADD in the infiltration.c super-loop

    /* 01_EVAPORATION BEGIN */
    // Loop over upper zones
    for (k = 1; k <= nuz; k++)
    {
        uzSMA[k]->etdemand = 0;
        /*
           for (i = 1; i <= nrows; i++)
           {
           for (j = 1; j <= ncols; j++)
           {
           if (imask[i][j] != nodatavalue)
           {
           //Extract ET before accounting for additional infiltration
           //Each upperzone has an evaporation demand computed from the 
           //accumulation of individual cell demands across the domain.
           //The cell ET demand is applied to the cumulative value for 
           //each upperzone according to the partition coefficient which,
           //though it could be different, shall be defined as equal to
           //the kuzSMA partition coefficient.
           uzSMA[k]->etdemand += kuzSMA[k][i][j] * etdemand[i][j];
           }
           }
           }
         */
    }
    /* The evaporation demand for the entire domain is calcuated 
       for all upper zones and then, theoretically, there should
       be some comparison to the upper zone storages in order to
       determine if the demand will be met and if not, which zone's 
       demand will be reduced -- otherwise, we give preference to 
       the first zone in any cell with hybrid upper zones. 

       Of course, this complication is an argument for having only 
       one zone per cell, similar to how there probably should only
       be one lowerzone per upper zone. Inverted pyramid style,
       from top to bottom, we always decrease in complexity. */

    for (k = 1; k <= nuz; k++)
    {
        /* JSH DEL Fixed ET to function for CG Case
         * JSH DEL This is a temporary workaround and needs to be replaced
         edmnd = &(uzSMA[k]->etdemand);
         //Scale the input by the number (and portion) of contributing cells
         (*edmnd) /= uzSMA[k]->areafactor;
         //Convert to millimeters, the internal units for the SMA procedure
         (*edmnd) *= 1000;
         END JSH DEL */

        uzSMA[k]->etdemand = 0.18;      //depth of evaporation in inches per day
        uzSMA[k]->etdemand *= (.3048 / 12);     //convert to meters per day
        uzSMA[k]->etdemand *= (1.0 / 86400);    //convert to meters per second
        uzSMA[k]->etdemand *= (1000 / 1);       //convert to millimeters per second
        edmnd = &(uzSMA[k]->etdemand);
        (*edmnd) *= dt[idt];    //multiply by the time step for meters

        // Calculate actual upper zone ET
        e1 = &(uzSMA[k]->et);
        uztwc = &(uzSMA[k]->twc);
        uztwm = uzSMA[k]->twm;

        //upper zone evaporation is scaled by the available upperzone tension water
        *e1 = (*edmnd) * ((*uztwc) / uztwm);
        (*uztwc) -= (*e1);

        //If upperzone tension water does not satisfy the ET demand ...
        if (*uztwc < 0.0)
        {
            *e1 = (*edmnd) + (*uztwc);
            *uztwc = 0.0;
        }
        //remaining evaporation demand is initialized for use during the rest of the loop.
        red = (*edmnd) - (*e1);
        if (red > 0)
        {
            e2 = &(uzSMA[k]->et_deep);
            (*e2) = 0;
            for (m = 1; m <= nlz; m++)
            {
                // Calculate actual lower zone ET
                e3 = &(lzSMA[m]->et);
                lztwc = &(lzSMA[m]->twc);
                lztwm = lzSMA[m]->twm;
                kperc = uzSMA[k]->kperc[m];

                //lower zone evaporation is scaled by the kperc partition coefficient
                (*e3) = red * kperc * ((*lztwc) / lztwm);
                (*lztwc) -= (*e3);

                //If lower zone tension water does not satisfy the ET demand ...
                if (*lztwc < 0.0)
                {
                    *e3 += (*lztwc);
                    *lztwc = 0.0;
                }
                (*e2) += (*e3);
            }
            //remaining evaporation demand is initialized for use during the rest of the loop.
            red -= (*e2);
            if (red > 0)
            {
                // Go to upper zone free water first for evaporation
                uzfwctotal = &(uzSMA[k]->fwctotal);
                *uzfwctotal = 0;
                for (h = 1; h <= uzSMA[k]->nparts; h++)
                {
                    e3 = &(uzSMA[k]->part[h]->et);
                    uzfwc = &(uzSMA[k]->part[h]->fwc);
                    uzfwm = uzSMA[k]->part[h]->fwm;
                    kpart = uzSMA[k]->kpart[h];

                    //If evap demand is greater than the free water volume,
                    //Add all the free water to evap and set free water to zero
                    if ((*uzfwc - (red * kpart)) < 0.0)
                    {
                        (*e3) += (*uzfwc);
                        *uzfwc = 0.0;
                        printf ("evap not satisfied\n");
                    }
                    //Otherwise, just reduce the free water by the evap amount.
                    else
                    {
                        (*e3) += (red * kpart);
                        (*uzfwc) -= (red * kpart);
                    }
                    (*uzfwctotal) += (*uzfwc);
                }
                red -= (*e3);
            }
        }
    }                           /* 01_EVAPORATION END */


    /* 02_REDISTRIBUTION BEGIN */
    // Loop over upper zones
    for (k = 1; k <= nuz; k++)
    {
        uztwc = &(uzSMA[k]->twc);
        uztwm = uzSMA[k]->twm;
        uzfwctotal = &(uzSMA[k]->fwctotal);
        uzfwmtotal = uzSMA[k]->fwmtotal;

        //Initialize free water total to make sure it adds up
        *uzfwctotal = 0;
        //Loop over free water storages in each lower zone
        for (h = 1; h <= uzSMA[k]->nparts; h++)
        {
            uzfwc = &(uzSMA[k]->part[h]->fwc);
            (*uzfwctotal) += (*uzfwc);
        }

        // if uztwc/uztwm < uzfwc/uzfwm, make them equal */
        // JSH ADD This should actually balance the 
        // JSH ADD storage volumes according to kpart
        a = (*uztwc) / uztwm;
        b = (*uzfwctotal) / uzfwmtotal;
        if (a < b)
        {
            a = ((*uztwc) + (*uzfwctotal)) / (uztwm + uzfwmtotal);
            // JSH The following is the algebraic proof that the redistribution 
            // JSH is not modifying the mass balance in this loop. 
            // a = (tc0 + fc0) / (tm + fm);
            // tc1 = tm * (tc0 + fc0) / (tm + fm);
            // fc1 = fm * (tc0 + fc0) / (tm + fm);
            // tc1 + fc1 = tc0 + fc0;
            // try it -- it works!
            *uztwc = uztwm * a;
            *uzfwctotal = 0;
            for (h = 1; h <= uzSMA[k]->nparts; h++)
            {
                uzfwc = &(uzSMA[k]->part[h]->fwc);
                uzfwm = uzSMA[k]->part[h]->fwm;
                *uzfwc = uzfwm * a;
                (*uzfwctotal) += (*uzfwc);
            }
        }
        //JSH During debugging, it was noted that if the upper zone
        //JSH free water exceeds the maximum, then the rebalancing
        //JSH procedure can put more than the maximum possible into
        //JSH the upper zone tension water which in turn has the effect of 
        //JSH making the available precip increase out of control 
        //JSH (because it is computed as the difference between incoming precipitation 
        //JSH and the tension water deficit -- which means subtracting a negative number
        //JSH if twc > twm).
        if ((*uztwc) > uztwm)
        {
            printf ("Upper zone tension water has exceeded the maximum\n\
                  A serious error has occured in the mass balance.\n");
        }
    }

    // Loop over lower zones
    for (k = 1; k <= nlz; k++)
    {
        lztwc = &(lzSMA[k]->twc);
        lztwm = lzSMA[k]->twm;
        saved = lzSMA[k]->saved;
        lzfwctotal = &(lzSMA[k]->fwctotal);
        lzfwmtotal = lzSMA[k]->fwmtotal;

        //Initialize free water total to make sure it adds up
        *lzfwctotal = 0;
        //Loop over free water storages in each lower zone
        for (h = 1; h <= lzSMA[k]->nparts; h++)
        {
            lzfwc = &(lzSMA[k]->part[h]->fwc);
            (*lzfwctotal) += (*lzfwc);
        }

        // if lower zone tension water becomes small, pull some water up from s & p
        a = (*lztwc) / lztwm;
        b = ((*lzfwctotal) + (*lztwc) - saved) / (lzfwmtotal + lztwm - saved);
        if (a < b)
        {
            del = (b - a) * lztwm;
            (*lztwc) += del;
            *lzfwctotal = 0;
            //Loop backward through the zones 
            //Filling tension water with last free water first.
            for (h = lzSMA[k]->nparts; h >= 1; h--)
            {
                // This has been checked for mass balance errors
                lzfwc = &(lzSMA[k]->part[h]->fwc);
                (*lzfwc) -= del;
                //JSH ADD put a debugging stop here if del gets below 0, which it shouldn't
                //JSH ADD Erroneous array access could occur ...
                if ((*lzfwc) < 0.0)
                {
                    del = -1.0 * (*lzfwc);
                    (*lzfwc) = 0.0;
                }
                else
                {
                    del = 0.0;
                }
                (*lzfwctotal) += (*lzfwc);
            }
        }
    }                           /* 02_REDISTRIBUTION END */


    /* 03_PRECIP BEGIN PART I */
    for (k = 1; k <= nuz; k++)
    {
        uzSMA[k]->wnew = 0;
        uzSMA[k]->precip = 0;
        for (i = 1; i <= nrows; i++)
        {
            for (j = 1; j <= ncols; j++)
            {
                if (imask[i][j] != nodatavalue)
                {
                    //Apply portion of cumulative infiltration to each upper zone
                    uzSMA[k]->wnew +=
                        kuzSMA[k][i][j] * SMAinfiltrationvol[i][j];
                    uzSMA[k]->precip += kuzSMA[k][i][j] * SMAprecipvol[i][j];
                }
            }
        }
    }

    for (k = 1; k <= nuz; k++)
    {
        pcp = &(uzSMA[k]->pcp);
        pav = &(uzSMA[k]->pav);
        *pcp = 0;
        *pcp = uzSMA[k]->wnew;
        //Scale the input by the number (and portion) of contributing cells
        (*pcp) /= uzSMA[k]->areafactor;
        //Convert to millimeters, the internal units for the SMA procedure
        (*pcp) *= 1000;

        uztwc = &(uzSMA[k]->twc);
        uztwm = uzSMA[k]->twm;

        //Does all the precip fit in the available upper zone tension?
        *pav = 0.0;
        (*pav) = (*pcp) - (uztwm - (*uztwc));
        //If it does, fill up the upper zone tension and clear the precip var.
        if ((*pav) < 0.0)
        {
            (*uztwc) += (*pcp);
            *pav = 0.0;
        }
        //If there is some leftover, set tension to max ...
        else
        {
            *uztwc = uztwm;
        }
        //... and
        //Store remaining precip in wnew (via *pcp pointer)
        *pcp = *pav;
    }                           /* 03_PRECIP END PART I */

    //JSH ADD This scaling (below) would be important
    //JSH ADD if we used longer time steps. 
    //JSH ADD This is just a code excerpt from sac_mini.c
    //JSH ADD So please refer to the original for further work
///*############################################################################*/
///* determine the number of time increments required so that no one increment  */
///* will exceed 5.0 mm of (*uzfwc+pav).  pinc is the precip available in each  */
///* time increment.                                                            */
///*############################################################################*/
//                ninc = (int) (1.00001 + ((*uzfwc) + pav) / 5.0);        /* uzfwc, pav in  mm, 5.0 is max rate */
//                dinc = 1.0 / (float) ninc;
//                pinc = pav * dinc;
//                *flowbf = 0.0;
//                *flowsf = 0.0;
//                *flowin = 0.0;
//                duz = uzSMA[k]->part[h]->k;
//                nskip = 1;
//                if (pav > 5.08)
//                {
//                    nskip = 0;
//                }
//
///* if there are sub-increments, modify the rate constants accordingly  */
//                if (ninc > 1)
//                {
//                    duz = 1.0 - pow ((1.0 - dcuz), (double) dinc);
//                }


    /* 04.00_Initialize SMA outlets BEGIN */
    for (m = 1; m <= nio; m++)
    {
        ioSMA[m]->wnew = 0;
    }                           /* 04.00_Initialize SMA outlets END */


    /* 04_BASEFLOW BEGIN */
    for (k = 1; k <= nlz; k++)
    {
        lzSMA[k]->basftotal = 0.0;
        lzfwctotal = &(lzSMA[k]->fwctotal);
        *lzfwctotal = 0;
        //Loop over free water storages in each lower zone
        for (h = 1; h <= lzSMA[k]->nparts; h++)
        {
            lzfwc = &(lzSMA[k]->part[h]->fwc);
            dlz = lzSMA[k]->part[h]->k;

            //Compute baseflow from the previous time step's lower zone free water.
            kbfeff = dlz;
            // Convert depletion coefficient from mm / mm / day
            // To mm / mm / s and multiply by the time step to determine straight volume
            kbfeff /= (24       // hours / day
                       * 60     // minutes / hour
                       * 60);   // seconds / minute 
            kbfeff *= dt[idt];  //seconds 
            //Total infiltration [mm] from this lower zone part is accumulated in the variable 'basf'.
            lzSMA[k]->part[h]->basf = ((*lzfwc) * kbfeff);
            //Amount of interflow is subtracted from fwc
            (*lzfwc) -= lzSMA[k]->part[h]->basf;
            //Amount of baseflow is added to total for the zone
            lzSMA[k]->basftotal += lzSMA[k]->part[h]->basf;
            //Free water total is updated
            (*lzfwctotal) += (*lzfwc);
            //Distribute baseflow to nio interflow outlets
            for (m = 1; m <= nio; m++)
            {
                flowbf = &(ioSMA[m]->wnew);
                (*flowbf) += lzSMA[k]->part[h]->basf
                    * lzSMA[k]->part[h]->kbasf[m]
                    * lzSMA[k]->areafactor * w * w / 1000;
            }
        }
    }                           /* 04_BASEFLOW END */


    /* 05_PERCOLATION_DEMAND BEGIN */
    for (k = 1; k <= nlz; k++)
    {
        lztwc = &(lzSMA[k]->twc);
        lztwm = lzSMA[k]->twm;
        lzfwctotal = &(lzSMA[k]->fwctotal);
        lzfwmtotal = lzSMA[k]->fwmtotal;
        percdemand = &(lzSMA[k]->percdemand);
        pbase = lzSMA[k]->pbase;
        zperc = lzSMA[k]->zperc;
        rexp = lzSMA[k]->rexp;

        //Initialize free water total to make sure it adds up
        *lzfwctotal = 0;
        //Loop over free water storages in each lower zone
        for (h = 1; h <= lzSMA[k]->nparts; h++)
        {
            lzfwc = &(lzSMA[k]->part[h]->fwc);
            (*lzfwctotal) += (*lzfwc);
        }

        //Compute percolation demand for this time step in this lower zone
        //Limit demand to the minimum of pbase
        a = lzfwmtotal - (*lzfwctotal) + lztwm - (*lztwc);      /* sum of lwr zone deficits */
        b = lzfwmtotal + lztwm;
        *percdemand = pbase * (1 + zperc * pow ((a / b), rexp));
    }
    /* 05_PERCOLATION_DEMAND END */


    /* 06_PERCOLATION BEGIN */
    for (k = 1; k <= nlz; k++)
    {
        //initialize lower zone new water
        lzSMA[k]->wnew = 0;
    }

    for (k = 1; k <= nuz; k++)
    {
        //Initialize upper zone to lower zone percolation
        uzSMA[k]->perctotal = 0;
        uzfwctotal = &(uzSMA[k]->fwctotal);
        *uzfwctotal = 0;
        for (h = 1; h <= uzSMA[k]->nparts; h++)
        {
            uzfwc = &(uzSMA[k]->part[h]->fwc);
            uzfwm = uzSMA[k]->part[h]->fwm;
            perc = &(uzSMA[k]->part[h]->percolation);
            *perc = 0;

            //Compute percolation to lower zone from previous 
            //time step percdemand and current time step uzfwc/uzfwm ratio.
            for (m = 1; m <= nlz; m++)
            {
                /* JSH Comment: 2009 12:34:21 GMT-0700 */
                //I am going to make the strategic decision that from upper to 
                //lower, the zones can only aggregate (1 to many relation for lower to upper).
                //This should mean that this loop shouldn't be necessary since
                //for a given upper zone, there will be one lower zone. 
                //(Other upper zones may have the same index
                //but each upper zone will have only one index.)
                //Someone later could make an array of indexes and liven things up a bit. 
                //
                // The problem is that if there is more than
                // one upper zone contributing to a particular lower zone,
                // the first upper zone may max out the lower storage volume
                // (i.e. the check parameter is triggered at > 0 above)
                // but the the storages are not updated until the next 
                // step and this could lead to overfilling of the wnew 
                // and THEN what do we do?
                //
                // This might be a reason to wrap the mass transfer 
                // steps (05 -- 08) into a larger loop (though not necessarily 
                // for the reasons that FLO did it--to compute the incremental 
                // volumes for long time steps). In the larger loop, we could 
                // check for such errors 
                /* JSH Comment: 2009 12:34:43 GMT-0700 */

                lztwc = &(lzSMA[k]->twc);
                lztwm = lzSMA[k]->twm;
                lzfwctotal = &(lzSMA[k]->fwctotal);
                lzfwmtotal = lzSMA[k]->fwmtotal;
                percdemand = &(lzSMA[m]->percdemand);
                kperc = uzSMA[k]->kperc[m];

                //Set aside volume for percolation
                rperc = (*percdemand);
                // Convert demand mm / mm / day
                // To mm / mm / s and multiply by the time step to determine straight volume
                rperc /= (24    // hours / day
                          * 60  // minutes / hour
                          * 60);        // seconds / minute 
                rperc *= dt[idt];       //seconds 
                //percolation is scaled by available upper zone free water
                rperc *= (*uzfwc) / uzfwm;

                //percolation is scaled by the free water portion contributing to each outlet
                //similar to the scaling of evaporation demand.
                rperc *= kperc;
                //If demand is greater than free water supply, take as much as possible
                if (rperc > (kperc * (*uzfwc)))
                {
                    rperc = (kperc * (*uzfwc));
                }
                else
                {
                    //This check reserves as upper zone free water any amount which
                    //does not fit in the lower zone storages.
                    check = (*lzfwctotal) + (*lztwc) - lzfwmtotal - lztwm;
                    check *= kperc;
                    check += rperc;

                    if (check > 0.0)
                    {
                        (rperc) -= check;
                    }
                }

                // Mass Balance Check
                lzSMA[m]->wnew += rperc;
                (*perc) += rperc;
            }                   //end loop over lower zones within upper zone loop
            // Mass Balance Check
            (*uzfwc) -= (*perc);
            (*uzfwctotal) += (*uzfwc);
            uzSMA[k]->perctotal += (*perc);
        }
    }                           //end Loop over upper zones
    /* 06_PERCOLATION END */


    /* 07_INTERFLOW BEGIN */
    for (k = 1; k <= nuz; k++)
    {
        uzSMA[k]->intftotal = 0.0;
        uzfwctotal = &(uzSMA[k]->fwctotal);
        *uzfwctotal = 0;
        //Loop over free water storages in each upper zone
        for (h = 1; h <= uzSMA[k]->nparts; h++)
        {
            uzfwc = &(uzSMA[k]->part[h]->fwc);
            duz = uzSMA[k]->part[h]->k;

            // After satisfying percolation demand, interflow 
            // is computed from the remaining upper zone free water.
            kifeff = duz;
            // Convert depletion coefficient from mm / mm / day
            // To mm / mm / s and multiply by the time step to determine straight volume
            kifeff /= (24       // hours / day
                       * 60     // minutes / hour
                       * 60);   // seconds / minute 
            kifeff *= dt[idt];  //seconds 
            //Total infiltration from this lower zone is accumulated in the variable 'intf'.
            uzSMA[k]->part[h]->intf = ((*uzfwc) * kifeff);
            //Amount of interflow is subtracted from fwc
            (*uzfwc) -= uzSMA[k]->part[h]->intf;
            //Amount of interflow is added to total for the zone
            uzSMA[k]->intftotal += uzSMA[k]->part[h]->intf;
            //Free water total is updated
            (*uzfwctotal) += (*uzfwc);
            for (m = 1; m <= nio; m++)
            {
                flowin = &(ioSMA[m]->wnew);
                (*flowin) += uzSMA[k]->part[h]->intf
                    * uzSMA[k]->part[h]->kintf[m]
                    * uzSMA[k]->areafactor * w * w / 1000;
            }
        }
    }                           /* 07_INTERFLOW END */


    /* 08_NEW WATER BEGIN */
    for (k = 1; k <= nlz; k++)
    {
        perc = &(lzSMA[k]->wnew);
        lztwc = &(lzSMA[k]->twc);
        lztwm = lzSMA[k]->twm;
        lzfwctotal = &(lzSMA[k]->fwctotal);
        lzfwmtotal = lzSMA[k]->fwmtotal;
        pfree = lzSMA[k]->pfree;
        F = &(lzSMA[k]->fwdeficitratio);

        rperc = (*perc) * (1.0 - pfree);

        //check if percolation is less than the lowerzone deficit.
        //If it is, dump the percolated water into the tension bucket, 
        //clear perc, and move on.
        if (rperc <= (lztwm - (*lztwc)))        /* chgd order of m and c - flo */
        {
            *lztwc += rperc;
            rperc = 0.0;
        }
        //If there is more incoming water than the twc can hold ...
        else
        {
            rperc -= (lztwm - (*lztwc));        /* chgd from + to - flo */
            *lztwc = lztwm;
        }
        //Add the pfree portion back in and distribute 
        //this water to the lower zone free water storages 
        rperc += (*perc) * pfree;
        if (rperc > 0.0)
        {
            // Distribute Percolated Water according to demand
            for (h = 1; h <= lzSMA[k]->nparts; h++)
            {
                lzfwc = &(lzSMA[k]->part[h]->fwc);
                lzfwm = lzSMA[k]->part[h]->fwm;
                (*F) = (lzfwm - (*lzfwc)) / (lzfwmtotal - (*lzfwctotal));
                (*lzfwc) += (rperc * (*F));
            }

        }                       // end if tension water leaves water for percolation
    }                           // end loop over lower zones

    for (k = 1; k <= nlz; k++)
    {
        //Initialize free water total to make sure it adds up
        *lzfwctotal = 0;
        //Loop over free water storages in each lower zone
        for (h = 1; h <= lzSMA[k]->nparts; h++)
        {
            lzfwc = &(lzSMA[k]->part[h]->fwc);
            (*lzfwctotal) += (*lzfwc);
        }
    }

    //Loop over upper zones to determine new water distribution
    for (k = 1; k <= nuz; k++)
    {
        pcp = &(uzSMA[k]->pcp);
        if (*pcp > 0)
        {
            // Distribute remaining precipitation according to demand
            for (h = 1; h <= uzSMA[k]->nparts; h++)
            {
                uzfwc = &(uzSMA[k]->part[h]->fwc);
                uzfwm = uzSMA[k]->part[h]->fwm;
                (*F) = (uzfwm - (*uzfwc)) / (uzfwmtotal - (*uzfwctotal));
                (*uzfwc) += ((*pcp) * (*F));
            }

        }
    }

    for (k = 1; k <= nuz; k++)
    {
        //Initialize free water total to make sure it adds up
        *uzfwctotal = 0;
        //Loop over free water storages in each lower zone
        for (h = 1; h <= uzSMA[k]->nparts; h++)
        {
            uzfwc = &(uzSMA[k]->part[h]->fwc);
            (*uzfwctotal) += (*uzfwc);
        }
    }                           /* 08_NEW WATER END */


    /* 09_SATURATION_EXCESS BEGIN */
    // JSH ADD This (09) and (10) are the real research problems
    // JSH ADD (09): How to spit water back i.e.
    // JSH ADD if, after the percolation and interflow has been satisfied, 
    // JSH ADD there is excess free water (fwc > fwm), then 
    // JSH ADD what do we do with the extra water?
    // JSH ADD Note, since the lower zones have a check in the percolation routines,
    // JSH ADD only the upper zones need to be checked for super-charging. 
    // JSH ADD and
    // JSH ADD (10): How to correlate the SAC parameters to the GA parameters.

    // Loop over upper zones
    for (k = 1; k <= nuz; k++)
    {

        uztwc = &(uzSMA[k]->twc);
        uztwm = uzSMA[k]->twm;

        //Initialize free water total to make sure it adds up
        *uzfwctotal = 0;
        for (h = 1; h <= uzSMA[k]->nparts; h++)
        {
            uzfwc = &(uzSMA[k]->part[h]->fwc);
            uzfwm = uzSMA[k]->part[h]->fwm;
            flowsf = &(uzSMA[k]->wreject);
            (*flowsf) = 0.0;
            //The remaining precipitated water is added to the 
            //upper zone free water in the distribution step (08)
            if ((*uzfwc) > uzfwm)
            {
                // Surface flow (from saturation excess) is computed
                // from the excess upper zone free water
                (*flowsf) = (*uzfwc) - uzfwm;
                switch (SMArejectionMethod)
                {
                case 0:        //Allow upperzone to simply overfill (with a warning).
                    //(*uzfwc) = uzfwm;
                    printf ("The SMA routine indicates %g millimeters \
							   saturation excess runoff is ocurring due to over \
							   filling of the upper zone %5.0d, \
							   free water part %5.0d. \n\
							   Please check fwm values and consider \
							   a modification to your model.\n", (*flowsf), k, h);
                    break;
                case 1:        //Redistribute excess just according to kuzSMA
                    // Upper zone free water is set to the maximum
                    (*uzfwc) = uzfwm;
                    printf
                        ("You are now using the saturation excess rejection algorithm.\n");
                    for (i = 1; i <= nrows; i++)
                    {
                        for (j = 1; j <= ncols; j++)
                        {
                            if (imask[i][j] != nodatavalue)
                            {
                                //Apply portion of excess to each overland cell in the upper zone
                                smaRejectvolume =
                                    (*flowsf) / uzSMA[k]->areafactor *
                                    kuzSMA[k][i][j];
                            }
                        }
                    }
                    break;
                case 2:        //Redistribution excess based on infiltrated volume
                    // Upper zone free water is set to the maximum
                    (*uzfwc) = uzfwm;
                    for (i = 1; i <= nrows; i++)
                    {
                        for (j = 1; j <= ncols; j++)
                        {
                            if (imask[i][j] != nodatavalue)
                            {
                                //Redistribute based on infiltration
                                smaRejectvolume =
                                    (*flowsf) * (SMAinfiltrationvol[i][j] /
                                                 uzSMA[k]->wnew);
                            }
                        }
                    }
                    break;
                case 3:        //Deliver excess to channel outlets
                    // Upper zone free water is set to the maximum
                    (*uzfwc) = uzfwm;
                    break;
                case 4:        //Resdistribute based on topographic index
                    // Upper zone free water is set to the maximum
                    (*uzfwc) = uzfwm;
                    break;
                case 5:        //Set infiltration Rate to zero --- MLV suggestion (a good one)
                    //"When there's none --- there's 'Dunne'".
                    break;
                }
            }
            (*uzfwctotal) += (*uzfwc);
        }
//                //ADD Would there be any advantage to allowingThe super-charging of fwm becomes something of a feature that maybe 
//                //ADD could be selectively turned on and off--assuming I can figure out
//                //ADD how to turn it off in the first place. With the feature on, it allows 
//                //ADD the Green and Ampt piston to completely determine the Hortonian rate
//                //ADD and just figuratively stacks up water to drive more percolation and
//                //ADD interflow through the SMA procedure. 
//                //ADD Alternatively, this could be added to the interflow value.
//                //ADD this would require a synthetic hydrograph (except for these extremely
//                //ADD short time steps, the variation would be negligible).
//                //ADD The initialization of these collector variables should occur in one
//                //ADD place and should be the same place that allows them to be printed if
//                //ADD that is desired. These collector variables are, for the most part, 
//                //ADD the sacramento model states and fluxes so we would likely want to see 
//                //ADD them in action.
    }                           /* 09_SATURATION_EXCESS END */


    /* 10_REINITIALIZE_INFILTRATION_PARAMS BEGIN */
    //Loop over upper zone again to pass the information back about losses.

    switch (SMAinfReinitMethod)
    {
    case 0:                    //For this case, do not reinitialize parameters
        printf ("SMA-based infiltration \n\
                    parameter re-initialization is inactive.\n");
        break;
    case 1:                    //Reinitialize parameters based on the precip
        //threshold definining the break between storms. 
        //Infiltrated depth will revert to 0, and the 
        //moisture deficit for each soil group will be 
        //set to the SMASummaryState value. 
        SMASummaryState = 0;
        SMASummaryDivisor = 0;
        for (k = 1; k <= nuz; k++)
        {
            for (m = 1; m <= nlz; m++)
            {
                lzSMA[m]->includeSummaryState = 1;
                if (lzSMA[m]->includeSummaryState)
                {
                    lztwc = &(lzSMA[m]->twc);
                    lztwm = lzSMA[m]->twm;
                    SMASummaryState += (*lztwc);
                    SMASummaryDivisor += lztwm;
                }
                for (h = 1; h <= lzSMA[m]->nparts; h++)
                {
                    lzSMA[m]->part[h]->includeSummaryState = 0;
                    if (lzSMA[m]->part[h]->includeSummaryState)
                    {
                        lzfwc = &(lzSMA[m]->part[h]->fwc);
                        lzfwm = lzSMA[m]->part[h]->fwm;
                        SMASummaryState += (*lzfwc);
                        SMASummaryDivisor += lzfwm;
                    }
                }

            }

            uzSMA[k]->includeSummaryState = 1;
            if (uzSMA[k]->includeSummaryState)
            {
                uztwc = &(uzSMA[k]->twc);
                uztwm = uzSMA[k]->twm;
                SMASummaryState += (*uztwc);
                SMASummaryDivisor += uztwm;
            }
            for (h = 1; h <= uzSMA[k]->nparts; h++)
            {
                uzSMA[k]->part[h]->includeSummaryState = 1;
                if (uzSMA[k]->part[h]->includeSummaryState)
                {
                    uzfwc = &(uzSMA[k]->part[h]->fwc);
                    uzfwm = uzSMA[k]->part[h]->fwm;
                    SMASummaryState += (*uzfwc);
                    SMASummaryDivisor += uzfwm;
                }
            }
        }

        SMASummaryState /= SMASummaryDivisor;

        //if the amount of precipitation is less than the threshold
        if (uzSMA[1]->precip < smaReinitializationStormEpsilon)
        {
            switch (smaReinitializationStormToggle)
            {
            case -1:           //Do nothing if no storm has occurred yet
                if (smaReinitializationPrintMessageToggle != 1)
                {
                    printf ("There has not yet been a storm\n");
                    smaReinitializationPrintMessageToggle = 1;
                }
                break;
            case 0:            // If there has been no storm and the infiltration
                // remains below epsilon, do nothing. 
                if (smaReinitializationPrintMessageToggle != 2)
                {
                    printf ("There has been a storm\n");
                    printf ("but it is over and another \n");
                    printf ("has not yet started\n");
                    smaReinitializationPrintMessageToggle = 2;
                }
                break;
            case 1:            // If there was a storm and it is now over, flip the toggle
                smaReinitializationStormToggle = 0;
                if (smaReinitializationPrintMessageToggle != 3)
                {
                    printf ("The storm is over.\n");
                    printf
                        ("The volume of incoming water averaged per cell\n");
                    printf ("on this upper zone is now less than the \n");
                    printf ("specified minimum of ");
                    printf ("%g\n", smaReinitializationStormEpsilon);
                    printf ("The SMA parameters will continue adjusting.\n");
                    printf ("When a new storm begins, the SMA parameters\n");
                    printf
                        ("will be used to re-initialize the infiltration parameters.\n");
                    smaReinitializationPrintMessageToggle = 3;
                }
                break;
            }
        }
        //else if the amount of precipitation is greater than the threshold
        else
        {
            switch (smaReinitializationStormToggle)
            {
            case -1:           // If no storm has occurred yet, 
                // set the toggle to reflect the onset of the first storm
                if (smaReinitializationPrintMessageToggle != 4)
                {
                    printf ("The first storm is beginning.\n");
                    smaReinitializationPrintMessageToggle = 4;
                }
                smaReinitializationStormToggle = 1;
                break;
            case 0:            // If there has been no storm and the infiltration
                // rises above epsilon, flip the toggle
                // and proceed with infiltration parameter reinitialization. 
                if (smaReinitializationPrintMessageToggle != 5)
                {
                    printf ("Another storm is beginning.\n");
                    printf
                        ("Infiltration parameters will be re-initialized\n");
                    printf ("based on SMA parameter states.\n");

                    printf ("Current Soil Settings are: \n");
                    printf ("soil\tkhsoil\tcapshsoil\tsoilmd\n");
                    smaReinitializationPrintMessageToggle = 5;
                }
                for (isoil = 1; isoil <= nsoils; isoil++)
                {
                    //modify the terms for infiltration rate equation
                    //The infiltration equation in infiltration-rN.c
                    /*              
                       p1 = (float) (khsoil[isoil] * dt[idt] -
                       2.0 * infiltrationdepth[i][j]);
                       p2 = khsoil[isoil] * (infiltrationdepth[i][j] +
                       capshsoil[isoil] * soilmd[isoil]);
                     */
                    printf ("%d\t", isoil);
                    printf ("%g\t", khsoil[isoil]);
                    //JSH ADD the infiltration depth varies by cell
                    //JSH ADD the soil parameters vary by soil type
                    //JSH ADD and the reset values are determined by the SMA
                    //JSH ADD parameter states which come from SMA zones. 
                    //JSH ADD The multiple geometries are starting to get problematic. 
                    printf ("%g\t", capshsoil[isoil]);
                    printf ("%g\n", soilmd[isoil]);
                    soilmd[isoil] = SMASummaryState;
                }
                //loop through rows and columns to reset infiltration depth to 0
                //printf("%g \t", infiltrationdepth[i][j]);
                for (i = 1; i <= nrows; i++)
                {
                    for (j = 1; j <= ncols; j++)
                    {
                        if (imask[i][j] != nodatavalue)
                        {
                            /*
                             * JSH ADD -- 
                             * Code to account for the continuously 
                             * increasing infiltration needs to be added
                             *
                             infiltrationdepth[i][j]
                             [smaReinitializationStormCount];
                             infiltrationdepth[i][j]
                             [smaReinitializationStormCount + 1] -=
                             kuzSMA[k][i][j];
                             */
                            infiltrationdepth[i][j] = 0;
                        }
                    }
                }
                smaReinitializationStormCount++;
                smaReinitializationStormToggle = 1;
                break;
            case 1:            // If there has been a storm and the infiltration
                // remains above epsilon, do nothing. 
                if (smaReinitializationPrintMessageToggle != 6)
                {
                    printf ("The storm is continuing\n");
                    smaReinitializationPrintMessageToggle = 6;
                }
            }
        }
        break;
    }                           /* 10_REINITIALIZE_INFILTRATION_PARAMS END */

//                //ADD How does the Sacramento model decide how much to send to primary
//                //ADD and secondary lower zones? Is there a balancing process so they 
//                //ADD are the same percent full after each filling step? 
//                //ADD I know they are filled based on demand so perhaps that is the balancing 
//                //ADD factor. 
//                //ADD I am inclined to use 1-c/m (current/max) and compare that between 
//                //ADD the various lower zone parts but that lends itself to a problem
//                //ADD with floating point error if the zones are very empty or if 
//                //ADD the difference in c/m ratio is very small. 
}                               /* End of function percolationSMA(void): Return to WaterTransport */
