/*----------------------------------------------------------------------
C-  Function:	OverlandWaterRoute.c
C-
C-	Purpose/	Explicit, two-dimensional overland water routing using
C-	Methods:	diffusive wave approximation.
C-
C-
C-	Inputs:		elevationov[][], hov[][], landuse[][], nmanningov[][],
C-              storagedepth[][],infiltrationrate[][] dt[] (Globals)
C-
C-	Outputs:	dqov[][] (Global)
C-				dqovin[][][] (Global)
C-				dqovout[][][] (Global)
C-
C-	Controls:   imask[][], infopt, outopt, dbcopt
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
C-				Bldg. 67, Denver Federal Center, Denver, CO  80225
C-
C-	Date:		02-SEP-2003
C-
C-	Revisions:	Complete rewrite including documentation, streamlining,
C-              revised local variable names and loop indices, error
C-              trapping for friction slope = zero, and options for
C-              computing flow at outlets/boundaries.
C-
C-  Revised:
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

void OverlandWaterRoute(void)
{
    //local variable declarations/definitions
    int ii,                     //adjacent cell row
      jj,                       //adjacent cell column
      outlink,                  //link number of channel in boundary cell
      outnode;                  //node number of channel in boundary cell

    float a,                    //sign to set flow direction
      so,                       //ground slope between cells (dimensionless)
      sf,                       //friction flope between cells (dimensionless)
      dhdx,                     //water surface slpe between cells (dimensionless)
      hcell,                    //cell overland water depth (m)
      ncell,                    //cell Manning n value (no, Manning n is not dimensionless...)
      scell,                    //cell depression storage depth (m)
      icell,                    //cell equivalent infiltration depth (for current time step) (m)
      alpha,                    //scalar coefficient in manning resistance equation
      dq,                       //overland flow between cells (m3/s)
      hboundary;                //water depth of boundary at outlet cell

    //Initialize overland flow arrays for use this time step...
    //
    //Loop over rows
    for (i = 1; i <= nrows; i++)
    {
        //Loop over columns
        for (j = 1; j <= ncols; j++)
        {
            //If cell is in the domain (if imask is not the no data value)
            if (imask[i][j] != nodatavalue)
            {
                //Initialize net flow array
                dqov[i][j] = 0.0;

                //Loop over directions/sources
                for (k = 0; k <= 10; k++)
                {
                    //Initialize gross inflow array
                    dqovin[i][j][k] = 0.0;

                    //Initialize gross outflow array
                    dqovout[i][j][k] = 0.0;

                }               //end loop over directions/sources

            }                   //end if imask[][] != nodatvalue

        }                       //end loop over columns

    }                           //end loop over rows

    //Initialize overland outlet arrays for use this time step...
    //
    //Loop over number of outlets
    for (i = 1; i <= noutlets; i++)
    {
        //Initialize outlet overland outflow
        qoutov[i] = 0.0;

        //Initialize outlet overland inflow
        qinov[i] = 0.0;

    }                           //end loop over number of outlets

    //Compute overland flows in the domain
    //
    //Loop over rows
    for (i = 1; i <= nrows; i++)
    {
        //Loop over columns
        for (j = 1; j <= ncols; j++)
        {
            //If cell is in the domain (imask is not the no data value)
            if (imask[i][j] != nodatavalue)
            {

                //From the starting cell (s), the eight directions of branch
                //connection are defined as:
                //
                //   8 1 2
                //   7 s 3
                //   6 5 4
                //
                //North = 1, Northeast = 2, East = 3, Southeast = 4, South = 5,
                //Southwest = 6, West = 7, and Northwest = 8.
                //

                //Search cells (9 total: 8 adjacent (a) plus the starting cell (s)):
                //
                //   a a a      8 1 2
                //   a s a      7 s 3
                //   a a a      6 5 4
                //
                //In terms of directions, sweep from 1-5 and 7-3


                //Loop over adjacent cells
                for (k = -1; k <= 0; k++)
                {
                    //This if/else structure alternates the direction
                    //of computations from time step to time step. For
                    //even numbered iterations, flow is first computed
                    //in the x-direction (West to East), and then in the
                    //y-direction (North to South).  For odd numbered
                    //iterations, flows are computed in the opposite
                    //order.
                    //
                    //if the iteration counter is zero
                    if (iteration == 0)
                    {
                        ii = i - k;     //row of adjacent cell (first to south, then same row)
                        jj = j + k + 1; //column of adjacent cell (first same column, then to east)
                    }
                    else        //else iteration != 0 (iteration == 1)
                    {
                        ii = i + k + 1; //row of adjacent cell (first same row, then to south)
                        jj = j - k;     //column of adjacent cell (first to east, then same column)

                    }           //end if iteration == 0

//mlv & jfe: The following "if" represents the "Highlander" assumption for overland flow.
//           It restricts water from flowing off the edge of the domain by the check of
//           the mask value (&& imask[][] != nodatavalue).  If we remove that check, water
//           can flow off a boundary.  We would need to track this boundary edge flow...

                    //If the adjacent cell is in the domain
                    if (ii <= nrows && jj <= ncols
                        && imask[ii][jj] != nodatavalue)
                    {
                        //Compute the ground slope of the land surface (between cells)
                        so = (elevationov[i][j] - elevationov[ii][jj]) / w;     //ground slope

                        //Compute the water surface slope (between cells)
                        dhdx = (hov[ii][jj] - hov[i][j]) / w;   //water surface slope

                        //Compute the friction slope
                        sf = so - dhdx; //friction slope

                        //Store friction slope for use in soil transport computations
                        //
                        //Note:  The friction slope is stored here.  For more
                        //       compact code, the logic to store sf could be
                        //       moved below to where dqovin and dqovout are
                        //       stored.  However, for clarity and parallelism
                        //       with the channel water routing code, sf is
                        //       stored here...
                        //
                        //if transfer is in the E-W direction (i = ii, same row...)
                        if (i == ii)
                        {
                            //if sf is positive (transfer from east to west) (from j to jj)
                            if (sf >= 0.0)
                            {
                                //store friction slope to east (direction = 3)
                                sfov[i][j][3] = sf;
                            }
                            //else if sf is negative (transfer from west to east) (from jj to j)
                            else if (sf < 0.0)
                            {
                                //store friction slope to west (direction = 7)
                                sfov[ii][jj][7] = (float) fabs (sf);

                            }   //end if sf >= 0.0
                        }
                        //else if transfer is in the N-S direction (j = jj, same column...)
                        else if (j == jj)
                        {
                            //if sf is positive (transfer from north to south) (from i to ii)
                            if (sf >= 0.0)
                            {
                                //store friction slope to south (direction = 5)
                                sfov[i][j][5] = sf;
                            }
                            //else if flow is negative (transfer from south to north) (from ii to i)
                            else if (sf < 0.0)
                            {
                                //store friction slope to north (direction = 1)
                                sfov[ii][jj][1] = (float) fabs (sf);

                            }   //end if sf >= 0.0

                        }       //end if flow is in E-W or N-S directions

                        //if the friction slope is positive
                        if (sf > 0)
                        {
                            //flow is from cell i,j to cell ii,jj
                            //
                            //set the sign = +1 to send flow in present direction
                            a = 1.0;

                            //set cell water depth (local) to water depth of cell i,j
                            hcell = hov[i][j];

                            //Note: the local variable name ncell is potentially
                            //      confusing.  Do not confuse ncell (the local
                            //      cell Manning n value) with ncells (the total
                            //      number of active grid cells in the domain)...
                            //
                            //set cell roughness (local) to n value for cell i,j
                            ncell = nmanningov[landuse[i][j]];  //Manning n value

                            //set cell storage depth
                            scell = storagedepth[i][j];

                            //if infiltration is simulated (infopt > 0)
                            if (infopt > 0)
                            {
                                //set cell infiltration depth for current time step
                                icell = infiltrationrate[i][j] * dt[idt];
                            }
                            else        //infiltration is not simulated
                            {
                                //set icell to zero (no infiltration)
                                icell = 0.0;

                            }   //end if infopt > 0 (infiltration is simulated)
                        }
                        else if (sf < 0)        //if the friction slope is negative
                        {
                            //flow is from cell ii,jj to cell i,j
                            //
                            //set the sign = +1 to send flow in oposite direction
                            a = -1.0;

                            //set cell water depth (local) to water depth of cell ii,jj
                            hcell = hov[ii][jj];

                            //Note: the local variable name ncell is potentially
                            //      confusing.  Do not confuse ncell (the local
                            //      cell Manning n value) with ncells (the total
                            //      number of active grid cells in the domain)...
                            //
                            //set cell roughness (local) to n value for cell ii,jj
                            ncell = nmanningov[landuse[ii][jj]];        //Manning n value

                            //set cell storage depth
                            scell = storagedepth[ii][jj];

                            //if infiltration is simulated (infopt > 0)
                            if (infopt > 0)
                            {
                                //set cell infiltration depth for current time step
                                icell = infiltrationrate[i][j] * dt[idt];
                            }
                            else        //infiltration is not simulated
                            {
                                //set icell to zero (no infiltration)
                                icell = 0.0;

                            }   //end if infopt > 0 (infiltration is simulated)
                        }
                        else    //sf == 0 (no flow)
                        {
                            //set flow direction to zero (no flow)
                            a = (float) (0.0);

                            //set cell water depth (local) to water depth of cell i,j
                            hcell = hov[i][j];

                            //set cell roughness (local) to n value for cell i,j
                            ncell = nmanningov[landuse[i][j]];  //Manning n value

                            //set cell storage depth
                            scell = storagedepth[i][j];

                            //if infiltration is simulated (infopt > 0)
                            if (infopt > 0)
                            {
                                //set cell infiltration depth for current time step
                                icell = infiltrationrate[i][j] * dt[idt];
                            }
                            else        //infiltration is not simulated
                            {
                                //set icell to zero (no infiltration)
                                icell = 0.0;

                            }   //end if infopt > 0 (infiltration is simulated)

                        }       //end if sf > 0

                        //Overland flow will occur when the local water depth exceeds
                        //the depression storage depth and the depth of water that
                        //will infiltrate.  (This should prevent negative flows...)
                        //
                        //if cell depth is sufficient for overland flow and sf not zero
                        if ((hcell > (scell + icell)) && sf != 0.0)
                        {
                            //Compute overland flow using Manning's resistance equation:
                            //
                            //  q = alpha * (h)^beta
                            //
                            //  alpha = (sf^0.5) / n  (n is Manning's n)
                            //
                            //  beta = 5/3 = 1.667  (constant)
                            //
                            //Compute alpha
                            alpha = (float) (pow (fabs (sf), 0.5) / ncell);

                            //compute flow rate (m3/s) (unit flow * w = Q)
                            dq = (float) (a * w * (alpha
                                                   *
                                                   pow ((hcell -
                                                         (scell + icell)),
                                                        1.667)));
                        }
                        else    //else hcell < (scell + icell) or sf = 0
                        {
                            //Flow is zero
                            dq = 0.0;

                        }       //end if hcell > (scell + icell) and sf not zero...

                        //subtract flow from upgradient cell (opposite direction for a = -1.0)
                        dqov[i][j] = dqov[i][j] - dq;

                        //add flow to downgradient cell (opposite direction for a = -1.0)
                        dqov[ii][jj] = dqov[ii][jj] + dq;

                        //store cell flow for sediment and chemical transport computations...
                        //
                        //if transfer is in the E-W direction (i = ii, same row...)
                        if (i == ii)
                        {
                            //if flow is positive (transfer from east to west) (from j to jj)
                            if (a == 1.0)
                            {
                                //store cell outflow to east (direction = 3)
                                dqovout[i][j][3] = (float) fabs (dq);

                                //increment cumulative cell flow volume (m3)
                                dqovoutvol[i][j][3] = dqovoutvol[i][j][3]
                                    + (float) fabs (dq) * dt[idt];

                                //store cell inflow from west (direction = 7)
                                dqovin[ii][jj][7] = (float) fabs (dq);

                                //increment cumulative cell flow volume (m3)
                                dqovinvol[ii][jj][7] = dqovinvol[ii][jj][7]
                                    + (float) fabs (dq) * dt[idt];
                            }
                            //else if flow is negative (transfer from west to east) (from jj to j)
                            else if (a == -1.0)
                            {
                                //store cell outflow to west (direction = 7)
                                dqovout[ii][jj][7] = (float) fabs (dq);

                                //increment cumulative cell flow volume (m3)
                                dqovoutvol[ii][jj][7] = dqovoutvol[ii][jj][7]
                                    + (float) fabs (dq) * dt[idt];

                                //store cell inflow from east (direction = 3)
                                dqovin[i][j][3] = (float) fabs (dq);

                                //increment cumulative cell flow volume (m3)
                                dqovinvol[i][j][3] = dqovinvol[i][j][3]
                                    + (float) fabs (dq) * dt[idt];

                            }   //end if a = 1.0
                        }
                        //else if transfer is in the N-S direction (j = jj, same column...)
                        else if (j == jj)
                        {
                            //if flow is positive (transfer from north to south) (from i to ii)
                            if (a == 1.0)
                            {
                                //store cell outflow to south (direction = 5)
                                dqovout[i][j][5] = (float) fabs (dq);

                                //increment cumulative cell flow volume (m3)
                                dqovoutvol[i][j][5] = dqovoutvol[i][j][5]
                                    + (float) fabs (dq) * dt[idt];

                                //store cell inflow from north (direction = 1)
                                dqovin[ii][jj][1] = (float) fabs (dq);

                                //increment cumulative cell flow volume (m3)
                                dqovinvol[ii][jj][1] = dqovinvol[ii][jj][1]
                                    + (float) fabs (dq) * dt[idt];
                            }
                            //else if flow is negative (transfer from south to north) (from ii to i)
                            else if (a == -1.0)
                            {
                                //store cell outflow to north (direction = 1)
                                dqovout[ii][jj][1] = (float) fabs (dq);

                                //increment cumulative cell flow (m3)
                                dqovoutvol[ii][jj][1] = dqovoutvol[ii][jj][1]
                                    + (float) fabs (dq) * dt[idt];

                                //store cell inflow from south (direction = 5)
                                dqovin[i][j][5] = (float) fabs (dq);

                                //increment cumulative cell flow volume (m3)
                                dqovinvol[i][j][5] = dqovinvol[i][j][5]
                                    + (float) fabs (dq) * dt[idt];

                            }   //end if a = 1.0
                        }
                        //This condition should never occur...
                        //
                        //else flow is neither E-W or N-S
                        else
                        {
                            //warn user of error
                            printf ("\n\nWhoa!  Flow is on a diagonal??");
                            printf ("\n  Time = %10.5f", simtime);
                            printf ("\n     i = %d", i);
                            printf ("\n     j = %d", j);
                            printf ("\n    ii = %d", ii);
                            printf ("\n    jj = %d\n\n", jj);

                        }       //end if flow is in E-W or N-S directions

                    }           //end if the adjacent cell is in the domain

                }               //end loop over adjacent cells

                //Add overland external flows...
                //
                //Loop over number of external flow sources
                for (k = 1; k <= nqwov; k++)
                {
                    //if flow source row and column match the present row and column
                    if ((qwovrow[k] == i) && (qwovcol[k] == j))
                    {
                        //Add (temporally interpolated) external flow to overland flow
                        dqov[i][j] = dqov[i][j] + qwovinterp[k];

                        //In case there is more than one external flow source
                        //to this node, flows must be summed.  This summation
                        //is ok because we do not need to separately track each
                        //possible external flow source...
                        //
                        //Gross inflow to present node from external source
                        dqovin[i][j][0] = dqovin[i][j][0] + qwovinterp[k];

                        //increment cumulative node flow volume
                        dqchinvol[i][j][0] = dqchinvol[i][j][0]
                            + qwovinterp[k] * dt[idt];

                        //Compute cumulative sum of external flow volumes (m3)
                        qwovvol[i][j] =
                            qwovvol[i][j] + qwovinterp[k] * dt[idt];

                    }           //end if the rows and column match

                }               //end loop over number of external flow sources

                //Check if present row/col is a flow reporting station...
                //
                //Loop over number of flow reporting stations
                for (k = 1; k <= nqreports; k++)
                {
                    //If current cell is a flow reporting station
                    if (i == qreprow[k] && j == qrepcol[k])
                    {
                        //Note: Overland flows are two dimensional so
                        //      the reported flow is the net flow that
                        //      moves through the cell.  The direction
                        //      of the flow is not preserved...
                        //
                        //      If the reporting station is also an
                        //      outlet cell, the reported flow saved
                        //      below will be overwitten and replaced
                        //      by the 1-D outflow from the overland
                        //      plane (which can be zero) depending on
                        //      the outlet option value (outopt)...
                        //
                        //store the net 2-d flow through the cell
                        qreportov[k] = dqov[i][j];
                    }

                }               //end loop over reporting stations

            }                   //end if current cell is in the domain (imask != nodatavalue)

        }                       //end loop over columns

    }                           //end loop over rows

    //Compute overland flow leaving outlets
    //
    //Loop over number of outlets
    for (i = 1; i <= noutlets; i++)
    {
        //Outlets can be overland cells or channel cells.  If channels
        //are simulated and the outlet is a channel cell, flow routing
        //from the overland portion of the outlet depends on outopt.
        //If outopt = 0, water in the overland portion of a channel is
        //routed to the channel portion of the cell (in ChannelDepth)
        //and will leave the domain via the channel network
        //
        //if the cell is not a channel OR the outlet option = 1
        if ((imask[iout[i]][jout[i]] < 2) || (outopt == 1))
        {
            //if the normal depth boundary condition is selected
            if (dbcopt[i] == 0)
            {
                //Normal Depth Downstream Boundary Condition...
                //
                //set friction slope equal to the ground slope
                sf = sovout[i];
            }
            else                //else, dbcopt[] == 1 (downstream water depth input from time series)
            {
                //Set water depth of boundary...
                //
                //if the outlet has a channel outlet:
                //
                //if channels are simulated (chnopt > 0) and the
                //outlet cell is a channel cell (imask > 1) and the
                //node is the last node of the link (node = nnodes)
                if (chnopt > 0 && imask[iout[i]][jout[i]] > 1 &&
                    node[iout[i]][jout[i]] == nnodes[link[iout[i]][jout[i]]])
                {
                    //set the outflowing link
                    outlink = link[iout[i]][jout[i]];

                    //set the outflowing node (should be last node)
                    outnode = node[iout[i]][jout[i]];

                    //if the boundary depth > channel bank height
                    if (hbcinterp[i] > hbank[outlink][outnode])
                    {
                        //set overland boundary water depth
                        hboundary = hbcinterp[i] - hbank[outlink][outnode];
                    }
                    else        //else hbcinterp < hbank
                    {
                        //overland boundary water depth is zero
                        hboundary = 0.0;

                    }           //end if hchinterp > hbank

                }               //end if outlet has a channel outlet

                //compute the water surface slope at outlet
                dhdx = (hboundary - hov[iout[i]][jout[i]]) / w; //water surface slope

                //Compute the friction slope
                sf = sovout[i] - dhdx;  //friction slope

            }                   //end if dbcopt[] = 0

            //if the friction slope is positive
            if (sf > 0)
            {
                //flow is from outlet to boundary
                //
                //set the sign = +1 to send flow in present direction
                a = 1.0;

                //Note: the local variable name ncell is potentially
                //      confusing.  Do not confuse ncell (the local
                //      cell Manning n value) with ncells (the total
                //      number of active grid cells in the domain)...
                //
                //set cell water depth (local) to water depth of outlet cell
                hcell = hov[iout[i]][jout[i]];

                //set cell roughness (local) to n value for outlet
                ncell = nmanningov[landuse[iout[i]][jout[i]]];  //Manning n value

                //set cell storage depth
                scell = storagedepth[iout[i]][jout[i]];

                //if infiltration is simulated (infopt > 0)
                if (infopt > 0)
                {
                    //set cell infiltration depth for current time step
                    icell = infiltrationrate[iout[i]][jout[i]] * dt[idt];
                }
                else            //infiltration is not simulated
                {
                    //set icell to zero (no infiltration)
                    icell = 0.0;

                }               //end if infopt > 0 (infiltration is simulated)
            }
            else if (sf < 0)    //if the friction slope is negative
            {
                //flow is from boundary to outlet
                //
                //set the sign = +1 to send flow in oposite direction
                a = -1.0;

                //set cell water depth (local) to boundary value
                hcell = hboundary;

                //Note: the local variable name ncell is potentially
                //      confusing.  Do not confuse ncell (the local
                //      cell Manning n value) with ncells (the total
                //      number of active grid cells in the domain)...
                //
                //set cell roughness (local) to n value for outlet
                ncell = nmanningov[landuse[iout[i]][jout[i]]];  //Manning n value

                //set cell storage depth for boundary to zero
                scell = 0.0;

                //set cell infiltration depth for boundary to zero
                icell = 0.0;
            }
            else                //sf == 0 (no flow)
            {
                //set flow direction to zero (no flow)
                a = 0;

            }                   //end if sf > 0

            //Compute overland flow using Manning's resistance equation:
            //
            //  q = alpha * (h)^beta (beta = 1.667)
            //
            //Note: this computations assumes that the friction slope at
            //      the outlet cell is equal to the ground surface slope
            //      (sf = so).
            //
            //Compute alpha
            alpha = (float) (pow (fabs (sf), 0.5) / ncell);

            //if cell depth is sufficient for overland flow
            if (hcell > (scell + icell))
            {
                //compute overland flow (m3/s) from/to the outlet cell
                dq = (float) (a * w * alpha
                              * pow ((hcell - (scell + icell)), 1.667));
            }
            else                //cell depth is not sufficent for overland flow
            {
                //overland flow (m3/s) at the outlet cell is zero
                dq = 0.0;

            }                   //end if cell depth is sufficient
        }
        else                    //overland flow is not routed through this outlet
        {
            //overland flow at this outlet is zero at this outlet
            dq = 0.0;

        }                       //end if (imask[][] < 2) or (outopt = 1)

        //Check if outlet cell row/col is a flow reporting station...
        //
        //Loop over number of flow reporting stations
        for (k = 1; k <= nqreports; k++)
        {
            //If this outlet is a flow reporting station
            if (iout[i] == qreprow[k] && jout[i] == qrepcol[k])
            {
                //report the overland outflow (m3/s)
                qreportov[k] = dq;
            }

        }                       //end loop over reporting stations

        //if the flow direction is positive (a = 1.0) (dq > 0)
        if (a == 1.0)
        {
            //store outlet outflow (m3/s)
            qoutov[i] = dq;

            //subtract flow from outlet cell
            dqov[iout[i]][jout[i]] = dqov[iout[i]][jout[i]] - qoutov[i];

            //store gross outflow to boundary (m3/s) (direction = 10)
            dqovout[iout[i]][jout[i]][10] = qoutov[i];

            //increment cumulative cell outflow volume
            dqovoutvol[iout[i]][jout[i]][10] =
                dqovoutvol[iout[i]][jout[i]][10] + qoutov[i] * dt[idt];
        }
        else if (a == -1.0)     //else if flow direction is negative (dq < 0)
        {
            //store outlet inflow (m3)
            qinov[i] = (float) fabs (dq);

            //add flow to outlet cell
            dqov[iout[i]][jout[i]] = dqov[iout[i]][jout[i]] + qinov[i];

            //store gross inflow from boundary (m3/s) (direction = 10)
            dqovin[iout[i]][jout[i]][10] = qinov[i];

            //increment cumulative cell inflow volume
            dqovinvol[iout[i]][jout[i]][10] = dqovinvol[iout[i]][jout[i]][10]
                + qinov[i] * dt[idt];

        }                       //end if a = 1.0

        //Search for peak outflow at this outlet...
        //
        //Note: dq is signed (+ or -).  A negative dq value indicates
        //      an inflow from the boundary.  The peak outflow at the
        //      outlet (positive dq) are stored in the qpeakov array.
        //      Only outflows are stored, inflows are < 0 and are not
        //      stored as peak flows (outflows).
        //
        //if qoutov[i] is greater than qpeakov[i]
        if (dq > qpeakov[i])
        {
            //Save peak flow at outlet
            qpeakov[i] = dq;

            //Save time of peak flow;
            tpeakov[i] = (float) (simtime);

        }                       //end if dq > qpeakov[]

        //Compute cumulative flow volume exiting domain at this outlet (m3)
        qoutovvol[i] = qoutovvol[i] + qoutov[i] * dt[idt];

    }                           //end loop over outlets

    //Increment the flow routing iteration index...
    //
    //if the iteration counter is zero
    if (iteration == 0)
    {
        //Set the iteration counter to 1
        iteration = 1;
    }
    else                        //else iteration != 0
    {
        //Reset the iteration counter to 0
        iteration = 0;

    }                           //end if iteration == 0

//End of function: Return to WaterTransport
}
