/*---------------------------------------------------------------------
C-  Function:   Rainfall.c                                              
C-
C-  Purpose/    Spatial interpolation of gross rainfall intensity for
C-  Methods:    each cell for the current time step.  Options include:
C-              spatially uniform, Inverse Distance Weighting (IDW),
C-              uniform in space with number of gages/locations,
C-              radar grid locations and rainrates, and space-time
C-              design storms
C-
C-  Inputs:     rfinterp[], idwradius, idwexponent
C-
C-  Outputs:    grossrainrate[][]
C-
C-  Controls:   rainopt (0,1,2,3,4,...)
C-              rainopt = 0: uniform rainfall in space
C-              rainopt = 1: rain gages are data input (i=1, nrg)
C-                and spatially-distributed with IDW
C-
C-              rainopt = 2-4: options from John England but
C-                not yet included in this version of TREX...
C-
C-              rainopt = 5: rainfall rates read from grid
C-                inputs (i.e. rainfall for each cell) and
C-                used as a step function without temporal
C-                interpolation
C-
C-  Calls:      ReadRainGrid, Min, Max
C-
C-  Called by:  WaterTransport
C-
C-  Created:    Original Coding in Fortran - CASC2D User's Manual
C-              Julien, P.Y. and Saghafian, B. 1991
C-              Civil Engineering, Colorado State University
C-              Fort Collins, CO 80523
C-              Code later converted from Fortran to C
C-              Last Update by Rosalia Rojas-Sanchez, CSU
C-
C-  Date:       19-JUN-2003
C-
C-  Revised:    John F. England, Jr.
C-              Bureau of Reclamation
C-              Flood Hydrology Group, D-8530
C-              Bldg. 67, Denver Federal Center
C-              Denver, CO 80225
C-
C-              Mark Velleux
C-              Department of Civil Engineering
C-              Colorado State University
C-              Fort Collins, CO  80523
C-
C-  Date:       18-JUL-2004
C-
C-  Revisions:  Documentation, reorganization, expanded input and
C-              spatial interpolation options
C-
C-  Revised:    Mark Velleux
C-              HydroQual, Inc.
C-              1200 MacArthur Boulevard
C-              Mahwah, NJ  07430
C-
C-  Date:	    27-JUL-2006	
C-
C-  Revisions:	Added rainopt = 5 to permit use of grid-based
C-              rainfall intensities (variable in space) used
C-              as step functions in time
C-
C-  Revised:
C-
C-  Date:		
C-
C-  Revisions:	
C-
C--------------------------------------------------------------------*/

//trex global variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for water transport
#include "trex_water_declarations.h"

void Rainfall(void)
{
    //local variable declarations/definitions
    int distanceflag;           //interpolation distance flag (indicates if all
    //gages are outside of radius of current cell)
    float x,                    //UTM x coordinate of grid cell i,j
      y,                        //UTM y coordinate of grid cell i,j
      distance,                 //distance from cell i,j to gage
      sumdistance,              //sum of inverse weighted distances
      sumrain;                  //sum of inverse weighted rainfall rates

    //Developer's Note:  Rainfall.c was initially conceived as a module
    //                   where spatial interpolation of rainfall values
    //                   from individual rainfall gages would be assigned
    //                   to each cell in the model domain using IDW or
    //                   other approaches.  As this module was developed
    //                   and rainfall options added, a wider range of
    //                   approaches was implemented such that not all
    //                   options require spatial interpolation.
    //
    //                   Rainopt 2-4 were created by John England to
    //                   handle rainfall input as depth-area-duration
    //                   curves and to input rainfall using grids that
    //                   have a different cell size and do not exactly
    //                   align with the model domain.  These options
    //                   are not yet integrated into this code.
    //
    //                   Rainopt 5 was latter added to input rainfall
    //                   using grids that have exactly the same cell
    //                   size and alignment as the model domain.  This
    //                   option does not require spatial interpolation
    //                   because a unique value it assigned to each
    //                   model cell via an rainfall grid file.
    //
    //                   To improve code readability and simplify code
    //                   maintainability, Rainfall.c and other time
    //                   function update modules should be streamlined
    //                   to explicitly segregate rainfall options that
    //                   require spatial (and temporal) interpolation
    //                   into distinct blocks of code...
    //
    //if the rainfall option requires spatial interpolation (rainopt <= 4)
    if (rainopt <= 4)
    {
        //Loop over rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is in the domain
                if (imask[i][j] != nodatavalue)
                {
                    //Perform spatial interpolation
                    //
                    //check rainopt
                    if (rainopt == 0)   //uniform rain
                    {
                        //rainfall is uniform (there is only one gage)
                        grossrainrate[i][j] = rfinterp[1];
                    }
                    else if (rainopt == 1)      //spatially distributed rain
                    {
                        //Raise the interpolation distance flag
                        distanceflag = 1;

                        //Initialize weighted sums
                        sumdistance = 0.0;
                        sumrain = 0.0;

                        //Translate grid location to UTM x,y coordinates
                        //
                        //This assumes an ESRI ASCII Grid format
                        //and converts cell row and column (i,j)
                        //to coordinate (x,y) located at grid cell center

                        //x coordinate
                        x = xllcorner + ((float) (j - 0.5)) * w;

                        //y coordiate
                        y = yllcorner + ((float) (nrows - i + 0.5)) * w;

                        //perform Inverse Distance Weighting (IDW) interpolation
                        //
                        //Loop over number of rain gages
                        for (k = 1; k <= nrg; k++)
                        {
                            //Compute distance between cell and rain gage
                            distance =
                                (float) (sqrt
                                         (pow ((y - rgy[k]), 2.0) +
                                          pow ((x - rgx[k]), 2.0)));

                            //if distance <= idwradius
                            if (distance <= idwradius)
                            {
                                //Lower the distance flag
                                distanceflag = 0;

                                //if the gage is located within the current cell
                                if (distance < w)
                                {
                                    //cell rainfall value equals the gage value
                                    //(only one gage per cell)
                                    grossrainrate[i][j] = rfinterp[k];
                                }
                                else    //distance >= w
                                {
                                    //sum weighted distances
                                    sumdistance = sumdistance +
                                        (float) (1.0 /
                                                 pow (distance, idwexponent));

                                    //sum weighted rainfall intensities
                                    sumrain = sumrain
                                        +
                                        (float) (rfinterp[k] /
                                                 pow (distance, idwexponent));

                                    //compute IDW gross rainfall intensity for cell i,j
                                    //for current time step
                                    grossrainrate[i][j] =
                                        sumrain / sumdistance;

                                    //Compute the cumulative depth of rainfall for each cell
                                    grossraindepth[i][j] =
                                        grossraindepth[i][j] +
                                        grossrainrate[i][j] * dt[idt];

                                }       //end if distance < w

                            }   //end if distance <= idwradius

                        }       //end loop over number of rain gages

                        //if distance flag = 1, no gages were within the radius
                        if (distanceflag == 1)  //distance flag is raised
                        {
                            //set gross rainfall intensity to zero
                            grossrainrate[i][j] = 0.0;

                            //If the gross rainfall rate is zero, we don't need to
                            //do this but we do for the sake of completeness...
                            //
                            //Compute the cumulative depth of rainfall for each cell (m)
                            grossraindepth[i][j] = grossraindepth[i][j]
                                + grossrainrate[i][j] * dt[idt];

                        }       //end if distance flag
                    }
                    else        //else rain option not yet implemented
                    {
                        //Write error message to file
                        fprintf (errorfile_fp,
                                 "\n\nRainopt 2-4 or >5 not implemented...\n");

                        //Report invalid rainopt
                        fprintf (errorfile_fp, "User selected rainopt = %d\n",
                                 rainopt);

                        //Advise for valid rainopt
                        fprintf (errorfile_fp,
                                 "\n\nSelect rainopt = 0, 1, or 5");

                        exit (EXIT_FAILURE);    //abort

                    }           //end if rainopt = 0...

                    //Save the minimum rainfall intensity value (m/s)
                    minrainintensity =
                        Min (minrainintensity, grossrainrate[i][j]);

                    //Save the maximum rainfall intensity value (m/s)
                    maxrainintensity =
                        Max (maxrainintensity, grossrainrate[i][j]);

                    //Compute the cumulative gross rainfall volume entering a cell (m3)
                    grossrainvol[i][j] = grossrainvol[i][j]
                        + grossrainrate[i][j] * w * w * dt[idt];

                }               //end if cell is in domain (imask != nodata value)

            }                   //end loop over columns

        }                       //end loop over rows
    }
    //else the rainfall option does not require spatial interpolation (rainopt = 5)
    else if (rainopt == 5)      //gridded rainfall option 
    {
        //Note:  This rainfall option assumes that the user specifies a
        //       grid of rainfall intensity values for each cell in the
        //       active model domain.  The rainfall valies in each grid
        //       can vary cell-by-cell in space but are not interpolated
        //       in space and are used as a step function in time (no 
        //       temporal interpolation).
        //
        //if simulation time >= time to read a new grid of rainfall intensities
        if (simtime >= timeraingrid)
        {
            //Note:  Gross rainfall intensities are specificed for
            //       each cell in ReadRainGridFile
            //
            //Read rainfall grid from file
            ReadRainGridFile ();

            //increment sequential count of rainfall grids read events for file extension
            raingridcount = raingridcount + 1;

            //Set the new rainfall grid read time
            timeraingrid = timeraingrid + raingridfreq;

        }                       //end if simtime >= timeraingrid

        //Loop over rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is in the domain
                if (imask[i][j] != nodatavalue)
                {
                    //Compute the cumulative gross rainfall volume entering a cell (m3)
                    grossrainvol[i][j] = grossrainvol[i][j]
                        + grossrainrate[i][j] * w * w * dt[idt];

                }               //end if cell is in domain (imask != nodata value)

            }                   //end loop over columns

        }                       //end loop over rows
    }
    else                        //else rain option not yet implemented
    {
        //Write error message to file
        fprintf (errorfile_fp, "\n\nRainopt 2-4 or >5 not implemented...\n");

        //Report invalid rainopt
        fprintf (errorfile_fp, "User selected rainopt = %d\n", rainopt);

        //Advise for valid rainopt
        fprintf (errorfile_fp, "\n\nSelect rainopt = 0, 1, or 5");

        exit (EXIT_FAILURE);    //abort

    }                           //end if rainopt <= 4...

//End of Function: Return to WaterTransport
}
