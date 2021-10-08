/*----------------------------------------------------------------------
C-  Function:	ChannelWaterRoute.c
C-
C-	Purpose/	Explicit, one-dimensional channel water routing using
C-	Methods:	diffusive wave approximation.
C-
C-
C-	Inputs:		hov[][], landuse[][], nmanningch[][], storagedepth[][],
C-              interceptionrate[][], dt[], ichnrow[][], ichncol[][],
C-              chanlength[][]  (Globals)
C-
C-	Outputs:	dqch[][] (Global)
C-              dqchin[][][] (Global)
C-              dqchout[][][] (Global)
C-
C-	Controls:   None
C-
C-	Calls:      None
C-
C-	Called by:	WaterTransport.c
C-
C-
C-	Created:	Rosalia Rojas-Sanchez
C-              Department of Civil Engineering
C-              Colorado State University
C-				Fort Collins, CO 80523
C-
C-              Based on earlier work of:
C-
C-              Bahram Saghafian (CSU) (1991)
C-
C-	Date:		19-JUN-2003 (date assumed)
C-
C-	Revised:	Mark Velleux
C-              Department of Civil Engineering
C-              Colorado State University
C-              Fort Collins, CO  80523
C-				
C-				John F. England, Jr.
C-				Bureau of Reclamation
C-				Flood Hydrology Group, D-8530
C-				Bldg. 67, Denver Federal Center
C-              Denver, CO  80225
C-
C-	Date:		21-AUG-2003
C-
C-	Revisions:	Documentation and streamlining, revise local
C-				variable names and loop indices, added error
C-              trapping for friction slope = zero, added
C-              module for flow to diverging branches.
C-
C-	Revised:	Mark Velleux
C-				HydroQual, Inc.
C-				1200 MacArthur Boulevard
C-				Mahwah, NJ 07430
C-
C-	Date:		20-May-2008
C-
C-	Revisions:	Reorganized code to assign point source flows.
C-              Imbedded if/then in main link/node loops was
C-              replaced with code to loop over loads to make
C-              direct assignment of values.
C-
C-    Revised:      James Halgren
C-
C-    Date:         01-DEC-2008
C-
C-    Revisions:    Added interflow outlet addition to channel if 
C-                  infopt = 2
C-
C-    Revised:
C-
C-    Date:
C-
C-    Revisions:
C-
C---------------------------------------------------------------------*/

//trex global variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for water transport
#include "trex_water_declarations.h"

//variable declarations and definitions specific to the 
//soil moisture accounting procedure.
#include "trex_SMA_declarations.h"

void ChannelWaterRoute (void)
{
    //Local variable declarations/definitions
    int row,                    //row number of present (upstream) node
      col,                      //column number of present (upstream) node
      downrow,                  //row number of downstream node
      downcol,                  //column number of downstream node
//mlv           uplink,         //link number of upstream link (for branching)
//mlv           upnode,         //node number of upstream node (for branching)
      downlink,                 //link number of downstream link (for branching)
      downnode,                 //node number of downstream node (for branching)
      l,                        //loop counter (for nested loop over branches)
      kup,                      //upstream direction from a downstream node (1-8)
      kdown;                    //downstream direction from the present node (1-8)

    float upbed,                //bed elevation of upstream node
      downbed,                  //bed elevation of downstream node
      a,                        //sign to set flow direction
      so,                       //bed slope (dimensionless)
      sf,                       //friction slope (dimensionless)
      dhdx,                     //water surface slope (dimensionless)
      hchan,                    //depth of water in channel (m)
      bwch,                     //bottom width of channel (m)
      hbch,                     //bank height of channel (m)
      sslope,                   //side slope of channel bank (dimensionless)
      nch,                      //Manning n value of channel (no, not dimensionless)
      stordep,                  //dead storage depth of channel (m)
      twch,                     //top width of channel at bank height (m)
      tloss,                    //transmission loss depth (m)
      area,                     //cross-sectional area of flow (m2)
      wp,                       //wetted perimeter (m)
      rh,                       //hydraulic radius (m)
      elevbound,                //bed elevation of boundary node (m)
      dq,                       //channel flow (m3/s)
      velocity,                 //mean flow velocity (m/s) (from Manning Equation)
      courant;                  //courant number (to track Courant-Friedrich-Levy stability)

    float dqsum,                //sum of flows for converging/diverging channel branches (m3/s)
      abranch[9],               //sign to set flow direction for a diverging branch
      velbranch[9],             //mean flow velocity for a diverging branch (m/s)
      dqbranch[9];              //flow down a channel branch at a diverging junction (m3/s)

//mlv   float
//mlv           volume,                 //volume of water available for flow (m3)
//mlv           usf,                    //upstream flow out of junction up link (m3/s)
//mlv           dsf,                    //downstream flow out of junction down a link (m3/s)
//mlv           dqfraction,             //fraction of total outflow down a channel branch at a junction (m3/s)
//mlv           dqlimit;                //branch flow limited to volume (m3/s)

    //Initialize channel flow arrays for use this time step...
    //
    //Loop over links
    for (i = 1; i <= nlinks; i++)
    {
        //Initialize link inflows and outflows to zero
        qinch[i] = 0.0;
        qoutch[i] = 0.0;

        //Loop over nodes
        for (j = 1; j <= nnodes[i]; j++)
        {
            //Set net channel flow array to zero
            dqch[i][j] = 0.0;

            //Loop over all possible flow sources (0 for flow point source)
            for (k = 0; k <= 10; k++)
            {
                //Set gross channel inflow and outflow arrays to zero
                dqchin[i][j][k] = 0.0;
                dqchout[i][j][k] = 0.0;

            }                   //end loop over all possible flow sources

            //Loop over all possible branches (directions)
            for (k = 1; k <= 8; k++)
            {
                //Set branch flow direction array to zero
                abranch[k] = 0.0;

                //Set branch flow velocity array to zero
                velbranch[k] = 0.0;

                //Set gross branch flow array to zero
                dqbranch[k] = 0.0;

            }                   //end loop over all possible branches

        }                       //end loop over nodes

    }                           //end loop over links

    //Add external flows...
    //
    //Note: Only flow sources (qwch > 0) are safely considered.
    //      Sources bring flow to the node and are added as
    //      dqchin[][][0]. Sinks (qwch < 0) take flow from the
    //      node and should be added as dqchout[][][0] but
    //      should also have a check comparing the sink volume
    //      (qwchinterp[] * dt[]) to the available volume.  The
    //      check should also consider total outflow potential
    //      due to channel flow as well as the sink.  As the
    //      code stands, a sink can be specified but it would
    //      be tracked as a negative source and does not have a
    //      check to make sure the sink is smaller than volume
    //      available for flow.
    //
    //Loop over number of external flow sources
    for (k = 1; k <= nqwch; k++)
    {
        //set link and node references for the flow point source
        i = qwchlink[k];
        j = qwchnode[k];

        //Add (temporally interpolated) external flow to channel flow
        dqch[i][j] = dqch[i][j] + qwchinterp[k];

        //In case there is more than one external flow source
        //to this node, flows must be summed.  This summation
        //is ok because we do not need to sperately track each
        //possible external flow source...
        //
        //Gross inflow to present node from external source
        dqchin[i][j][0] = dqchin[i][j][0] + qwchinterp[k];

        //Developer's Note:  Since dqchinvol[][][0] is the same as qwchvol[][],
        //                   the qwchvol array could be eliminated.  For now both
        //                   arrays are retained because they provide a separate
        //                   check on point source flows.
        //
        //increment cumulative node flow volume
        dqchinvol[i][j][0] = dqchinvol[i][j][0] + qwchinterp[k] * dt[idt];

        //Compute sum of external flow volumes (m3)
        qwchvol[i][j] = qwchvol[i][j] + qwchinterp[k] * dt[idt];
        if (infopt == 2)
        {
            // loop over SMA interflow outlets 
            for (m = 1; m <= nio; m++)
            {
                //ADD SMA JSH the flows may also be added to points in the overland domain
                //ADD SMA JSH but that is trickier, computationally so I'll start here
                if ((ioSMA[m]->intfout_i == i) && (ioSMA[m]->intfout_j == j))
                {
                    // Add interflow
                    dqch[i][j] += ioSMA[m]->wnew / dt[idt];
                    dqchin[i][j][0] += ioSMA[m]->wnew / dt[idt];
                    dqchinvol[i][j][0] += ioSMA[m]->wnew;
                    //JSH DEBUG printf ("\nThe first loop delivered to interflow outlet\n");
                    //JSH DEBUG printf ("%g m^3\n", ioSMA[m]->wnew);
                    //JSH DEBUG printf ("in %g seconds\n", dt[idt]);
                    qwchvol[i][j] += ioSMA[m]->wnew;
                }
            }                   // end loop over SMA interflow outlets
        }                       // end if (infopt ==2) SMA 
    }                           //end loop over number of external flow sources

    //Route flows though channel network...
    //
    //Loop over links
    for (i = 1; i <= nlinks; i++)
    {
        //Route flow from head of a link down to its second to last
        //node (upstream of a junction).  Flow routing across the
        //junctions from the last node of the link to the first node
        //of all downstream branches is performed in a seperate loop.
        //
        //Loop over nodes in curent link
        for (j = 1; j <= nnodes[i] - 1; j++)
        {
            //Compute channel bed slope, so
            //
            //Compute bed elevation at present (upstream) node
            upbed = elevationch[i][j];

            //Compute bed elevation of downstream node
            downbed = elevationch[i][j + 1];

            //Compute bed slope
            so = (upbed - downbed) / chanlength[i][j];

            //Compute water surface slope, dh/dx
            dhdx = (hch[i][j + 1] - hch[i][j]) / chanlength[i][j];

            //Compute friction slope, sf
            sf = so - dhdx;

            //Store friction slope for use in sediment transport computations
            sfch[i][j] = sf;

            //Assign channel characteristics between nodes
            bwch = bwidth[i][j];        //bottom width (m)
            hbch = hbank[i][j]; //bank height (m)
            sslope = sideslope[i][j];   //side slope (dimensionless)
            nch = nmanningch[i][j];     //Manning n for channel (not dimensionless)
            stordep = deadstoragedepth[i][j];   //dead storage depth (m)
            twch = twidth[i][j];        //top width at top of bank (m)

            //if channel transmission loss is simulated (ctlopt > 0)
            if (ctlopt > 0)
            {
                //set node transmission loss depth for current time step
                tloss = translossrate[i][j] * dt[idt];
            }
            else                //transmission loss is not simulated
            {
                //set tloss to zero (no transmission loss)
                tloss = 0.0;

            }                   //end if ctlopt > 0 (transmission loss is simulated)

            //if the friction slope is positive (or zero) (flow is in downstream direction)
            if (sf >= 0.0)
            {
                //set the sign = +1 to send flow in downstream direction
                a = 1.0;

                //Flow depth is water depth in channel of present (upstream) link/node
                hchan = hch[i][j];      //water depth in channel (m)
            }
            else if (sf < 0.0)  //friction slope is negative (flow in upstream direction)
            {
                //set the sign = -1 to send flow in upstream direction
                a = -1.0;

                //Flow depth is water depth in channel of downstream link/node
                hchan = hch[i][j + 1];  //water depth in channel (m)

            }                   //end if sf >= 0 (or < 0)

            //Channel flow will occur when the local water depth exceeds
            //the dead storage depth and the equivalent depth of the
            //transmission loss.  (This should prevent negative flows...)
            //
            //      if((hchan > (stordep + transloss)) && sf != 0.0)
            //
            //if the depth is sufficient for channel flow and sf not zero
            if (hchan > (stordep + tloss) && sf != 0.0)
            {
                //if channel depth <= bank height
                if (hchan <= hbch)      //Flow depth less than channel depth
                {
                    //Cross-sectional area for trapezoidal, triangular, or rectangular channel
                    area =
                        (bwch + sslope * (hchan - stordep)) * (hchan -
                                                               stordep);

                    //Wetted perimeter for trapezoidal, triangular, or rectangular channel
                    wp = (float) (bwch + 2.0
                                  * (hchan - stordep) * sqrt (1.0 +
                                                              sslope *
                                                              sslope));
                }
                else            //Flow depth greater than channel depth
                {
                    //Cross-sectional area for trapezoidal, triangular, or rectangular channel
                    //plus rectangular area above bank height
                    area =
                        (bwch + sslope * (hchan - stordep)) * (hchan -
                                                               stordep) +
                        (hchan - hbch) * twch;

                    //Wetted perimeter for trapezoidal, triangular, or rectangular channel
                    //plus rectangular perimeter above bank height
                    wp = (float) (bwch +
                                  2.0 * (hbch - stordep) * sqrt (1.0 +
                                                                 sslope *
                                                                 sslope) +
                                  2.0 * (hchan - hbch));

                }               //end if hchan <= bank height

                //Compute hydraulic radius (m)
                rh = area / wp;

                //Determine flow velocity (m/s) using Manning equation
                velocity =
                    (float) (sqrt (fabs (sf)) / nch * pow (rh, 0.6667));

                //Compute courant number:
                //
                //  courant = celerity * dt / w = 5/3 * velocity * dt / w
                //
                //ADD JSH I could do something about this in the final dissertation.
                //Note:  This check is actually a poor indicator of model stability
                //       because it only examines the advective component of water
                //       transport.  The overall "velocity" of water includes
                //       contributions from rainfall, infiltration, and overland
                //       flow.  We really need to do this on a volumetric basis
                //       (i.e. compute hydraulic residence time) and do it in the
                //       water mass balance routine...
                //
                //Compute Courant number
                courant = (float) (5.0 * velocity * dt[idt] / (3.0 * w));

                //Determine maximum Courant number
                maxcourant = (float) Max (maxcourant, courant);

                //Determinine channel discharge (the direction of flow is set by "a")
                dq = (float) (a * velocity * area);     //flow potential (m3/s)
            }
            else                //hchan not sufficient or sf = 0 (no flow occurs)
            {
                //Flow is zero
                dq = 0.0;

            }                   //end if hchan > stordep and sf not zero

            //Transfer flow from present (upstream) cell
            dqch[i][j] = dqch[i][j] - dq;

            //Transfer flow to downstream cell
            dqch[i][j + 1] = dqch[i][j + 1] + dq;

            //store nodal flow for sediment and chemical transport computations...
            //
            //Interior node
            //
            //if the flow is in the downstream direction (a = 1.0)
            if (a == 1.0)
            {
                //Gross outflow from present (upstream) node (m3/s)
                dqchout[i][j][downdirection[i][j][0]] = dq;

                //increment cumulative node flow volume
                dqchoutvol[i][j][downdirection[i][j][0]] =
                    dqchoutvol[i][j][downdirection[i][j][0]] + dq * dt[idt];

                //Gross inflow to downstream node (m3/s)
                dqchin[i][j + 1][updirection[i][j + 1][0]] = dq;

                //increment cumulative node flow volume
                dqchinvol[i][j + 1][updirection[i][j + 1][0]] =
                    dqchinvol[i][j + 1][updirection[i][j + 1][0]] +
                    dq * dt[idt];

            }
            else                // flow is in the reverse direction (a = -1.0)
            {
                //Gross outflow from upgradient node (m3/s)
                dqchout[i][j + 1][updirection[i][j + 1][0]] =
                    (float) fabs (dq);

                //increment cumulative node flow volume
                dqchoutvol[i][j + 1][updirection[i][j + 1][0]] =
                    dqchoutvol[i][j + 1][updirection[i][j + 1][0]] +
                    (float) fabs (dq) * dt[idt];

                //Gross inflow to downgradient (present) node (m3/s)
                dqchin[i][j][downdirection[i][j][0]] = (float) fabs (dq);

                //increment cumulative node flow volume
                dqchinvol[i][j][downdirection[i][j][0]] =
                    dqchinvol[i][j][downdirection[i][j][0]] +
                    (float) fabs (dq) * dt[idt];

            }                   //end if a = 1.0 (or -1.0)

            //Check if present link/node is a flow reporting station...
            //
            //get row of present node
            row = ichnrow[i][j];

            //get column of present node
            col = ichncol[i][j];

            //Loop over number of flow reporting stations
            for (k = 1; k <= nqreports; k++)
            {
                //If current cell is a flow reporting station
                if (row == qreprow[k] && col == qrepcol[k])
                {
                    //store the flow for reporting (m3/s)
                    qreportch[k] = dq;
                }

            }                   //end loop over reporting stations

        }                       //end loop over nodes (excluding junctions)

        //Compute flow at junctions...
        //
        //Set the present (upstream) node
        j = nnodes[i];

        //Determine number of downstream branches...
        //
        //The number of downstream branches is stored in the zeroeth
        //element of the ndownbranches array.  The link number of any
        //downstream link is stored in element corresponding to the
        //direction to the junction (array elements 1-8).
        //
        //if the number of downstream branches <= 1
        if (ndownbranches[i][0] <= 1)
        {
            //Determine row and column location of present (upstream) node
            row = ichnrow[i][j];
            col = ichncol[i][j];

            //There is only one downstream location to "push" water...
            //
            //The downstream location is either a:
            //
            //  1. downstream link; or
            //  2. boundary/outlet.
            //
            //Determine the downstream link
            downlink =
                link[ichnrow[link[row][col]][nnodes[link[row][col]] + 1]]
                [ichncol[link[row][col]][nnodes[link[row][col]] + 1]];

            //Determine downstream node (first node of link, except at boundaries)
            downnode =
                node[ichnrow[link[row][col]][nnodes[link[row][col]] + 1]]
                [ichncol[link[row][col]][nnodes[link[row][col]] + 1]];

            //Initialize the downstream direction index
            kdown = -1;

            //Get the downstream direction to the downstream node...
            //
            //Loop over directions (include zeroeth element to look for boundary)
            for (k = 0; k <= 8; k++)
            {
                //if the direction > -1 (is not null)
                if (downdirection[i][j][k] > -1)
                {
                    //save the downstream direction
                    kdown = downdirection[i][j][k];

                }               //end if downdirection[][] > -1

            }                   //end loop over directions

            //Initialize the upstream direction index
            kup = -1;

            //if the downstream point is an interior junction (not a boundary)
            if (downlink > 0 && downnode > 0)
            {
                //Determine row and column location of downstream node
                downrow = ichnrow[downlink][downnode];
                downcol = ichncol[downlink][downnode];

                //Get the upstream direction from the downstream node...
                //
                //Loop over directions
                for (k = 0; k <= 8; k++)
                {
                    //if the direction > -1 (is not null)
                    if (updirection[downlink][downnode][k] > -1)
                    {
                        //if the present link is an upstream branch
                        if (i == nupbranches[downlink][k])
                        {
                            //save the upstream direction
                            kup = updirection[downlink][downnode][k];

                        }       //end if i = nupbranches[][]

                    }           //end if updirection[][] > -1

                }               //end loop over directions 
            }
            else                //else the downstream point is a boundary
            {
                //Set row and column location of downstream node to zero
                downrow = 0;
                downcol = 0;

            }                   //end if downstream point is not (or is) a boundary

            //Compute channel bed slope, so
            //
            //Compute bed elevation at present (upstream) node
            upbed = elevationch[i][j];

            //if the downstream point is an interior junction (not a boundary)
            if (downrow > 0 && downcol > 0)
            {
                //Compute bed elevation of downstream node
                downbed = elevationch[downlink][downnode];

                //Compute water surface slope, dh/dx
                dhdx =
                    (hch[downlink][downnode] - hch[i][j]) / chanlength[i][j];
            }
            else                //downstream point is a boundary (exterior junction)
            {
                //Compute assumed elevation of boundary node (j+1):
                //
                // elevbound = ground elevation at present node (j) minus the
                //             elevation difference between the present node (j)
                //             and the prior node (j-1)
                //
                //           = present - (prior - present) = 2 * present - prior
                //
                //elevation at boundary (virtual) node downstream of present (j) nodes
                elevbound =
                    (float) (2.0 * elevationov[ichnrow[i][j]][ichncol[i][j]] -
                             elevationov[ichnrow[i][j - 1]][ichncol[i]
                                                            [j - 1]]);

                //Compute assumed bed elevation of boundary node (j+1)...
                //
                //Assume that boundary node bank height is the same as
                //the bank height of the present (j) node.
                //
                //Compute bed elevation for boundary
                downbed = elevbound - hbank[i][j];

                //if the domain boundary condition option is zero
                if (dbcopt[qchoutlet[i]] == 0)
                {
                    //Normal Depth Downstream Boundary Condition...
                    //
                    //Set water surface slope to boundary node (j+1)
                    //
                    //Assume that boundary node water depth is the same as
                    //the water depth of the present (j) node (sf = so).
                    //
                    //Set water surface slope, dh/dx = 0
                    dhdx = 0.0;
                }
                else            //else if dbcopt[] = 1
                {
                    //Specified water depth at boundary...
                    //
                    //Compute water surface slope, dh/dx
                    dhdx =
                        (hbcinterp[qchoutlet[i]] -
                         hch[i][j]) / chanlength[i][j];

                }               //end if dbcopt[] = 0

            }                   //end if downstream point is not (or is) a boundary

            //Compute bed slope
            so = (upbed - downbed) / chanlength[i][j];

            //Compute friction slope, sf
            sf = so - dhdx;

            //Store friction slope for use in sediment transport computations
            sfch[i][j] = sf;

            //Assign channel characteristics between nodes
            bwch = bwidth[i][j];        //bottom width (m)
            hbch = hbank[i][j]; //bank height (m)
            sslope = sideslope[i][j];   //side slope (dimensionless)
            nch = nmanningch[i][j];     //Manning n for channel (not dimensionless)
            stordep = deadstoragedepth[i][j];   //dead storage depth (m)

            //if channel transmission loss is simulated (ctlopt > 0)
            if (ctlopt > 0)
            {
                //set node transmission loss depth for current time step
                tloss = translossrate[i][j] * dt[idt];
            }
            else                //transmission loss is not simulated
            {
                //set tloss to zero (no transmission loss)
                tloss = 0.0;

            }                   //end if ctlopt > 0 (transmission loss is simulated)

            //if the friction slope is positive (flow is in downstream direction)
            if (sf > 0.0)
            {
                //set the sign = +1 to send flow in downstream direction
                a = 1.0;

                //Flow depth is water depth in channel of present (upstream) link/node
                hchan = hch[i][j];      //water depth in channel (m)
            }
            else if (sf < 0.0)  //friction slope is negative (flow in upstream direction)
            {
                //set the sign = -1 to send flow in upstream direction
                a = -1.0;

                //if the downstream point is an interior junction (not a boundary)
                if (downrow > 0 && downcol > 0)
                {
                    //Flow depth is water depth in channel of downstream link/node
                    hchan = hch[downlink][downnode];    //water depth in channel (m)
                }
                else            //downstream point is a boundary (exterior junction)
                {
                    //boundary depth assumed to equal depth of present node...
                    //
                    //Assign flow depth (boundary node = present node)
                    hchan = hch[i][j];  //water depth in channel (m)

                }               //end if the downstream point is an interior junction (not a boundary)

            }                   //end if sf > 0 (or < 0)

            //if the depth is sufficient for channel flow and sf not zero
            if (hchan > (stordep + tloss) && sf != 0.0)
            {
                //if flow depth <= bank height
                if (hchan <= hbch)      //Flow depth less than bank height
                {
                    //Cross-sectional area for trapezoidal, triangular, or rectangular channel
                    area =
                        (bwch + sslope * (hchan - stordep)) * (hchan -
                                                               stordep);

                    //Wetted perimeter for trapezoidal, triangular, or rectangular channel
                    wp = (float) (bwch +
                                  2.0 * (hchan - stordep)
                                  * sqrt (1.0 + sslope * sslope));
                }
                else            //Flow depth greater than channel depth
                {
                    //Cross-sectional area for trapezoidal, triangular, or rectangular channel
                    //plus rectangular area above bank height
                    area =
                        (bwch + sslope * (hchan - stordep)) * (hchan -
                                                               stordep) +
                        (hchan - hbch) * twch;

                    //Wetted perimeter for trapezoidal, triangular, or rectangular channel
                    //plus rectangular perimeter above bank height
                    wp = (float) (bwch + 2.0 * (hbch - stordep)
                                  * sqrt (1.0 + sslope * sslope) +
                                  2.0 * (hchan - hbch));

                }               //end if hchan <= bank height

                //Compute hydraulic radius (m)
                rh = area / wp;

                //Determine flow velocity (m/s) using Manning equation (at [i][j])
                velocity =
                    (float) (sqrt (fabs (sf)) / nch * pow (rh, 0.6667));

                //Compute Courant number
                courant = (float) (5.0 * velocity * dt[idt] / (3.0 * w));

                //Determine maximum Courant number
                maxcourant = (float) Max (maxcourant, courant);

                //Determinine channel discharge (the direction of flow is set by "a")
                dq = (float) (a * velocity * area);     //flow potential (m3/s)
            }
            else                //hchan not sufficient or sf = 0 (no flow occurs)
            {
                //Flow is zero
                dq = 0.0;

            }                   //end if hchan > stordep and sf not zero

            //Transfer flow from present (upstream) cell
            dqch[i][j] = dqch[i][j] - dq;

            //If the downstream point is in the domain, flow is transferred
            //to that node.  If the downstream point is not in the domain,
            //no flow is transferred to the downstream point...
            //
            //if the downstream point is an interior node (not a boundary)
            if (downrow > 0 && downcol > 0)
            {
                //Transfer flow to downstream cell
                dqch[downlink][downnode] = dqch[downlink][downnode] + dq;

            }                   //end if downstream node is not a boundary

            //store nodal flow for sediment and chemical transport computations...
            //
            //if the flow is in the downstream direction (a = 1.0)
            if (a == 1.0)
            {
                //Gross outflow from present (upstream) node (m3/s)
                dqchout[i][j][kdown] = dq;

                //increment cumulative node flow volume
                dqchoutvol[i][j][kdown] =
                    dqchoutvol[i][j][kdown] + dq * dt[idt];

                //if the downstream point is within the domain (not a boundary)
                if (downrow > 0 && downcol > 0)
                {
                    //Gross inflow to downstream node (m3/s)
                    dqchin[downlink][downnode][kup] = dq;

                    //increment cumulative node flow volume
                    dqchinvol[downlink][downnode][kup] =
                        dqchinvol[downlink][downnode][kup] + dq * dt[idt];
                }
                else            //else downstream point is outside of the domain (boundary)
                {
                    //store link outflow (m3/s)
                    qoutch[i] = dq;

                    //Compute cumulative flow volume exiting domain from this link (m3)
                    qoutchvol[i] = qoutchvol[i] + dq * dt[idt];

                }               //end if downstream point is an interior node
            }
            else if (a == -1.0) // flow is in the reverse (upstream) direction (a = -1.0)
            {
                //if the downstream point is an interior node (not a boundary)
                if (downrow > 0 && downcol > 0)
                {
                    //Gross outflow from upgradient node (m3/s)
                    dqchout[downlink][downnode][kup] = (float) fabs (dq);

                    //increment cumulative node flow volume
                    dqchoutvol[downlink][downnode][kup] =
                        dqchoutvol[downlink][downnode][kup] +
                        (float) fabs (dq) * dt[idt];
                }
                else            //else downstream point is outside of the domain (boundary)
                {
                    //store link inflow (m3/s) (inflow from downstream boundary)
                    qinch[i] = (float) fabs (dq);

                    //Compute cumulative flow volume entering domain from this link (m3)
                    qinchvol[i] = qinchvol[i] + (float) fabs (dq) * dt[idt];

                }               //end if downstream point is an interior node

                //Gross inflow to downgradient (present) node (m3/s)
                dqchin[i][j][kdown] = (float) fabs (dq);

                //increment cumulative node flow volume
                dqchinvol[i][j][kdown] = dqchinvol[i][j][kdown]
                    + (float) fabs (dq) * dt[idt];

            }                   //end if a = 1.0 (or -1.0)
        }
        else                    //number of ndownbranches[link][0] > 1 (diverging branches)
        {
            //set sum of branch flow potentials to zero
            dqsum = 0.0;

            //Loop over directions to search for downstream branches
            //(include zero to look for boundaries/outlets)
            for (k = 0; k <= 8; k++)
            {
                //if a downstream link exists in the current direction
                if (ndownbranches[i][k] >= 0)
                {
                    //Determine row and column location of present (upstream) node
                    row = ichnrow[i][j];
                    col = ichncol[i][j];

                    //Compute potential for flow down this branch...
                    //
                    //Determine the downstream link
                    downlink = ndownbranches[i][k];

                    //Set downstream node (first node of downstream link)
                    downnode = 1;

                    //if the downstream point is an interior junction (not a boundary)
                    if (downlink > 0 && downnode > 0)
                    {
                        //Determine row and column location of downstream node
                        downrow = ichnrow[downlink][downnode];
                        downcol = ichncol[downlink][downnode];
                    }
                    else        //else the downstream point is a boundary
                    {
                        //Set row and column location of downstream node to zero
                        downrow = 0;
                        downcol = 0;

                    }           //end if downstream point is not (or is) a boundary

                    //Compute channel bed slope, so...
                    //
                    //Compute bed elevation at present (upstream) node
                    upbed = elevationch[i][j];

                    //if the downstream point is an interior node (not a boundary)
                    if (downrow > 0 && downcol > 0)
                    {
                        //Compute bed elevation of downstream node
                        downbed = elevationch[downlink][downnode];

                        //Compute water surface slope, dh/dx
                        dhdx =
                            (hch[downlink][downnode] -
                             hch[i][j]) / chanlength[i][j];
                    }
                    else        //downstream point is a boundary (exterior node)
                    {
                        //Compute assumed elevation of boundary node (j+1):
                        //
                        // elevbound = ground elevation at present node (j) minus the
                        //             elevation difference between the present node (j)
                        //             and the prior node (j-1)
                        //
                        //           = present - (prior - present) = 2 * present - prior
                        //
                        //elevation at boundary (virtual) node downstream of present (j) nodes
                        elevbound =
                            (float) (2.0 *
                                     elevationov[ichnrow[i][j]][ichncol[i][j]]
                                     -
                                     elevationov[ichnrow[i][j - 1]][ichncol[i]
                                                                    [j - 1]]);

                        //Compute assumed bed elevation of boundary node (j+1)...
                        //
                        //Assume that boundary node bank height is the same as
                        //the bank height of the present (j) node.
                        //
                        //Compute bed elevation for boundary
                        downbed = elevbound - hbank[i][j];

                        //if the domain boundary condition is zero
                        if (dbcopt[qchoutlet[i]] == 0)
                        {
                            //Normal Depth Downstream Boundary Condition...
                            //
                            //Set water surface slope to boundary node (j+1)
                            //
                            //Assume that boundary node water depth is the same as
                            //the water depth of the present (j) node (sf = so).
                            //
                            //Set water surface slope, dh/dx = 0
                            dhdx = 0.0;
                        }
                        else    //else if dbcopt[] = 1
                        {
                            //Specified water depth at boundary...
                            //
                            //Compute water surface slope, dh/dx
                            dhdx =
                                (hbcinterp[qchoutlet[i]] -
                                 hch[i][j]) / chanlength[i][j];

                        }       //end if dbcopt[] = 0

                    }           //end if downstream point is not (or is) a boundary

                    //Compute bed slope
                    so = (upbed - downbed) / chanlength[i][j];

                    //Compute friction slope, sf
                    sf = so - dhdx;

                    //Store friction slope for use in sediment transport computations
                    sfch[i][j] = sf;

                    //Assign channel characteristics between upstream and downstream nodes
                    bwch = bwidth[i][j];        //bottom width (m)
                    hbch = hbank[i][j]; //bank height (m)
                    sslope = sideslope[i][j];   //side slope (dimensionless)
                    nch = nmanningch[i][j];     //Manning n for channel (not dimensionless)
                    stordep = deadstoragedepth[i][j];   //dead storage depth (m)

                    //if channel transmission loss is simulated (ctlopt > 0)
                    if (ctlopt > 0)
                    {
                        //set node transmission loss depth for current time step
                        tloss = translossrate[i][j] * dt[idt];
                    }
                    else        //transmission loss is not simulated
                    {
                        //set tloss to zero (no transmission loss)
                        tloss = 0.0;

                    }           //end if ctlopt > 0 (transmission loss is simulated)

                    //if the friction slope is positive (flow is in downstream direction)
                    if (sf > 0.0)
                    {
                        //set the sign = +1 to send flow in downstream direction
                        abranch[k] = 1.0;

                        //Flow depth is water depth in channel of present link/node
                        hchan = hch[i][j];      //water depth in channel (m)
                    }
                    else if (sf < 0.0)  //friction slope is negative (flow in upstream direction)
                    {
                        //set the sign = -1 to send flow in upstream direction
                        abranch[k] = -1.0;

                        //if the downstream point is an interior junction (not a boundary)
                        if (downrow > 0 && downcol > 0)
                        {
                            //Flow depth is water depth in channel of downstream link/node
                            hchan = hch[downlink][downnode];
                        }
                        else    //downstream point is a boundary (exterior node)
                        {
                            //boundary depth assumed to equal depth of present node...
                            //
                            //Assign flow depth (boundary node = present node)
                            hchan = hch[i][j];

                        }       //end if downstream point is not (or is) a boundary

                    }           //end if sf > 0 (or < 0)

                    //if the depth is sufficient for channel flow and sf not zero
                    if (hchan > (stordep + tloss) && sf != 0.0)
                    {
                        //if channel depth <= bank height
                        if (hchan <= hbch)      //Flow depth less than bank height
                        {
                            //Cross-sectional area for trapezoidal, triangular, or rectangular channel
                            area =
                                (bwch + sslope * (hchan - stordep)) * (hchan -
                                                                       stordep);

                            //Wetted perimeter for trapezoidal, triangular, or rectangular channel
                            wp = (float) (bwch + 2.0 * (hchan - stordep)
                                          * sqrt (1.0 + sslope * sslope));
                        }
                        else    //Flow depth greater than channel depth
                        {
                            //Cross-sectional area for trapezoidal, triangular, or rectangular channel
                            //plus rectangular area above bank height
                            area = (bwch + sslope * (hchan - stordep))
                                * (hchan - stordep) + (hchan - hbch) * w;

                            //Wetted perimeter for trapezoidal, triangular, or rectangular channel
                            //plus rectangular perimeter above bank height
                            wp = (float) (bwch + 2.0 * (hbch - stordep)
                                          * sqrt (1.0 + sslope * sslope)
                                          + 2.0 * (hchan - hbch));

                        }       //end if hchan <= bank height

                        //Compute hydraulic radius (m)
                        rh = area / wp;

                        //Determine flow potential velocity (m/s) using Manning equation
                        velbranch[k] =
                            (float) (sqrt (fabs (sf)) / nch *
                                     pow (rh, 0.6667));

                        //Compute Courant number
                        courant =
                            (float) (5.0 * velbranch[k] * dt[idt] /
                                     (3.0 * w));

                        //Determine maximum Courant number
                        maxcourant = (float) Max (maxcourant, courant);

                        //Determinine flow potential for current branch (flow direction set by "abranch")
                        dqbranch[k] = (float) (abranch[k] * velbranch[k] * area);       //flow potential (m3/s)
                    }
                    else        //hchan not sufficient or sf = 0 (no flow occurs)
                    {
                        //Flow is zero (branch exists, but there is no flow)
                        dqbranch[k] = 0.0;

                    }           //end if hchan > stordep and sf not zero
                }
                else            //no branch exists in this direction (ndownbranches[i][k] < 0)
                {
                    //Flow is zero (branch does not exist)
                    dqbranch[k] = 0.0;

                }               //end if ndownbranches[i][k] >= 0

            }                   //end loop over number of downstream branches

            //Transfer branch flows to/from downstream nodes...
            //
            //Loop over number of downstream branches
            for (k = 1; k <= ndownbranches[i][0]; k++)
            {
                //if a downstream link exists in the current direction
                if (ndownbranches[i][k] >= 0)
                {
                    //Determine the downstream link
                    downlink = ndownbranches[i][k];

                    //Set downstream node (first node of downstream link)
                    downnode = 1;

                    //Determine row and column location of downstream node
                    downrow = ichnrow[downlink][downnode];
                    downcol = ichncol[downlink][downnode];

                    //Transfer flow from present (upstream) cell
                    dqch[i][j] = dqch[i][j] - dqbranch[k];

                    //if the downstream point is an interior junction (not a boundary)
                    if (downrow > 0 && downcol > 0)
                    {
                        //Transfer flow to downstream cell
                        dqch[downlink][downnode] =
                            dqch[downlink][downnode] + dqbranch[k];

                    }           //end if downstream node is not a boundary

                    //store nodal flow for sediment and chemical transport computations...
                    //
                    //Exterior node with diverging branches
                    //
                    //if the flow is in the downstream direction (abranch = 1.0)
                    if (abranch[k] == 1.0)
                    {
                        //Gross outflow from present (upstream) node (m3/s)
                        dqchout[i][j][downdirection[i][j][k]] = dqbranch[k];

                        //if the downstream point is within the domain (not a boundary)
                        if (downrow > 0 && downcol > 0)
                        {
                            //Gross inflow to downstream node (m3/s)
                            dqchin[downlink][downnode][updirection[downlink]
                                                       [downnode][k]] =
                                dqbranch[k];

                            //increment cumulative node flow volume
                            dqchinvol[downlink][downnode][updirection
                                                          [downlink][downnode]
                                                          [k]] =
                                dqchinvol[downlink][downnode][updirection
                                                              [downlink]
                                                              [downnode][k]] +
                                dqbranch[k] * dt[idt];
                        }
                        else    //else downstream point is outside of the domain (boundary)
                        {
                            //store link outflow (sum) (m3/s)
                            qoutch[i] = qoutch[i] + dqbranch[k];

                            //Compute cumulative flow volume exiting domain from this link (m3)
                            qoutchvol[i] =
                                qoutchvol[i] + dqbranch[k] * dt[idt];

                        }       //end if downstream point is an interior node
                    }
                    else        // flow is in the reverse (upstream) direction (abranch = -1.0)
                    {
                        //if the downstream point is an interior node (not a boundary)
                        if (downrow > 0 && downcol > 0)
                        {
                            //Gross outflow from upgradient node (m3/s)
                            dqchout[downlink][downnode][updirection[downlink]
                                                        [downnode][k]] =
                                (float) fabs (dqbranch[k]);

                            //increment cumulative node flow volume
                            dqchoutvol[downlink][downnode][updirection
                                                           [downlink]
                                                           [downnode][k]] =
                                dqchoutvol[downlink][downnode][updirection
                                                               [downlink]
                                                               [downnode][k]]
                                + (float) fabs (dqbranch[k]) * dt[idt];
                        }
                        else    //else downstream point is outside of the domain (boundary)
                        {
                            //store link inflow (sum) (m3/s) (inflow from downstream boundary)
                            qinch[i] = qinch[i] + (float) fabs (dqbranch[k]);

                            //Compute cumulative flow volume entering domain from this link (m3)
                            qinchvol[i] =
                                qinchvol[i] +
                                (float) fabs (dqbranch[k]) * dt[idt];

                        }       //end if downstream point is an interior node

                        //Gross inflow to downgradient (present) node (m3/s)
                        dqchin[i][j][downdirection[i][j][k]] =
                            (float) fabs (dqbranch[k]);

                        //increment cumulative node flow volume
                        dqchinvol[i][j][downdirection[i][j][k]] =
                            dqchinvol[i][j][downdirection[i][j][k]]
                            + (float) fabs (dqbranch[k]) * dt[idt];

                    }           //end if abranch = 1.0 (or -1.0)

                }               //end if ndownbranches[i][k] >= 0

            }                   //end loop over downstream branches

        }                       //end if ndownbranches <= 1

        //Check if present link/node is a flow reporting station...
        //
        //Loop over number of flow reporting stations
        for (k = 1; k <= nqreports; k++)
        {
            //If current cell is a flow reporting station
            if (row == qreprow[k] && col == qrepcol[k])
            {
                //if the number of downstream branches <= 1
                if (ndownbranches[i][0] <= 1)
                {
                    //save the flow for reporting (m3/s)
                    qreportch[k] = dq;
                }
                else            //else ndownbranches[i][0] > 1
                {
                    //Set sum of branch flows to zero
                    dqsum = 0.0;

                    //Loop over branches, l ("ell") is a local loop counter
                    for (l = 1; l <= ndownbranches[i][0]; l++)
                    {
                        //Sum branch flows (all branches, whether + or -)
                        dqsum = dqsum + dqbranch[l];

                    }           //end loop over branches

                    //Note: The flow from node [i][j] can include
                    //      reverse flows from downstream some nodes
                    //      so at junctions with diverging branches
                    //      the reported flow is actually the net
                    //      downstream flow to all downstream nodes.

                    //save the flow for reporting (m3/s)
                    qreportch[k] = dqsum;

                }               //end if ndownbranches[i][0] <= 1

            }                   //end if current cell is a reporting station

        }                       //end loop over reporting stations

    }                           //end loop over links

    //Store flows at watershed (overland) outlets...
    //Loop over number of outlets
    for (k = 1; k <= noutlets; k++)
    {
        //if the outlet cell is a channel cell (imask > 1)
        if (imask[iout[k]][jout[k]] > 1)
        {
            //set link and node references for the outlet cell
            i = link[iout[k]][jout[k]];
            j = node[iout[k]][jout[k]];

            //Search for peak flow at this outlet...
            //
            //if qoutch[i] is greater than qpeakch[k]
            if (qoutch[i] > qpeakch[k])
            {
                //Save peak flow at outlet
                qpeakch[k] = qoutch[i];

                //Save time of peak flow;
                tpeakch[k] = (float) (simtime);

            }                   //end if qoutch[i] > qpeakch[k]

        }                       //end if imask[][] > 1

    }                           //end loop over outlets

//End of Function: Return to WaterTransport
}
