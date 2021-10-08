/*----------------------------------------------------------------------
C-  Function:	WriteGridsChemical.c                                              
C-
C-	Purpose/	Write optional output at each grid cell at time t.
C-	Methods:	Specification details are in Data Group F
C-
C-
C-	Inputs:		simtime
C-
C-	Outputs:	none
C-
C-	Controls:   timeprintgrid
C-
C-	Calls:		ComputeChemicalGroupConcSums
C-				ComputeChemicalGroupPhaseAve
C-				Grid
C-
C-	Called by:	WriteGrids
C-
C-
C-  Created:    Mark Velleux
C-              Department of Civil Engineering
C-              Colorado State University
C-              Fort Collins, CO  80523
C-
C-				John F. England, Jr.
C-				Bureau of Reclamation
C-				Flood Hydrology Group, D-8530
C-				Bldg. 67, Denver Federal Center, Denver, CO 80225
C-
C-	Date:		12-SEP-2004
C-
C-	Revised:
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

//trex global variable declarations for sediment transport
#include "trex_solids_declarations.h"

//trex global variable declarations for chemical transport
#include "trex_chemical_declarations.h"

void WriteGridsChemical(int gridcount)
{
    //Local variable declarations/definitions
    int igroup,                 //local counter for reporting group
      iphase,                   //index for chemical phase (total, dissolved, etc.)
      ipath;                    //index for transport path (infiltration, erosion, deposition, net accumulation)

    float *cgroupsum,           //location group sum array for concentrations (g/m3)
    ***concentration,           //local concentration array (g/m3)
     *cgroupave,                //location group sum array for phase fractions (dimensionless)
    ***fraction,                //local phase distribution fraction array (dimensionless)
    ***transport;               //local transport flux array (g/s)

    //Allocate memory for local group, concentration, and fraction arrays
    cgroupsum = (float *) malloc ((ncgroups + 1) * sizeof (float));
    concentration = (float ***) malloc ((ncgroups + 1) * sizeof (float **));
    cgroupave = (float *) malloc ((ncgroups + 1) * sizeof (float));
    fraction = (float ***) malloc ((ncgroups + 1) * sizeof (float **));
    transport = (float ***) malloc ((ncgroups + 1) * sizeof (float **));

    //Loop over number of reporting groups
    for (igroup = 1; igroup <= ncgroups; igroup++)
    {
        //Allocate additional memory local concentration and fraction arrays
        concentration[igroup] =
            (float **) malloc ((nrows + 1) * sizeof (float *));
        fraction[igroup] = (float **) malloc ((nrows + 1) * sizeof (float *));
        transport[igroup] =
            (float **) malloc ((nrows + 1) * sizeof (float *));

        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Allocate remaining memory local concentration and fraction arrays
            concentration[igroup][i] =
                (float *) malloc ((ncols + 1) * sizeof (float));
            fraction[igroup][i] =
                (float *) malloc ((ncols + 1) * sizeof (float));
            transport[igroup][i] =
                (float *) malloc ((ncols + 1) * sizeof (float));

        }                       //end loop over rows

    }                           //end loop over groups

    //if the water column total chemical concentration grid file (root) name is not null
    if (*totchemconcwatergridroot != '\0')
    {
        //phase is the total chemical (iphase = 0)
        iphase = 0;

        //set values for concentration: chemical concentration in overland or channel cells
        //
        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over number of columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is not null (it is in the active area)
                if (imask[i][j] != nodatavalue)
                {
                    //Compute chemical reporting groups sums (water column)
                    ComputeChemicalGroupConcSums (cgroupsum, i, j, 0, iphase);

                    //loop over chemical reporting groups
                    for (igroup = 1; igroup <= ncgroups; igroup++)
                    {
                        //assign cell concentrations
                        concentration[igroup][i][j] = cgroupsum[igroup];

                    }           //end loop over chemical reporting groups

                }               //end if imask != nodatatavalue

            }                   //end loop over columns

        }                       //end loop over rows

        //loop over chemical reporting groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //Write grid of water column chemical concentrations
            Grid (totchemconcwatergrid[igroup], concentration[igroup],
                  1.0, gridcount);

        }                       //end loop over chemical reporting groups

    }                           //end if the water column total chemical concentration grid file root name is not null

    //if the water column dissolved chemical concentration grid file (root) name is not null
    if (*dischemconcwatergridroot != '\0')
    {
        //phase is the dissolved chemical (iphase = 1)
        iphase = 1;

        //set values for concentration: chemical concentration in overland or channel cells
        //
        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over number of columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is not null (it is in the active area)
                if (imask[i][j] != nodatavalue)
                {
                    //Compute chemical reporting groups sums (water column)
                    ComputeChemicalGroupConcSums (cgroupsum, i, j, 0, iphase);

                    //loop over chemical reporting groups
                    for (igroup = 1; igroup <= ncgroups; igroup++)
                    {
                        //assign cell concentrations
                        concentration[igroup][i][j] = cgroupsum[igroup];

                    }           //end loop over chemical reporting groups

                }               //end if imask != nodatatavalue

            }                   //end loop over columns

        }                       //end loop over rows

        //loop over chemical reporting groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //Write grid of water column chemical concentrations
            Grid (dischemconcwatergrid[igroup], concentration[igroup],
                  1.0, gridcount);

        }                       //end loop over chemical reporting groups

    }                           //end if the water column dissolved chemical grid file root name is not null

    //if the water column bound chemical concentration grid file (root) name is not null
    if (*bndchemconcwatergridroot != '\0')
    {
        //phase is the bound chemical (iphase = 2)
        iphase = 2;

        //set values for concentration: chemical concentration in overland or channel cells
        //
        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over number of columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is not null (it is in the active area)
                if (imask[i][j] != nodatavalue)
                {
                    //Compute chemical reporting groups sums (water column)
                    ComputeChemicalGroupConcSums (cgroupsum, i, j, 0, iphase);

                    //loop over chemical reporting groups
                    for (igroup = 1; igroup <= ncgroups; igroup++)
                    {
                        //assign cell concentrations
                        concentration[igroup][i][j] = cgroupsum[igroup];

                    }           //end loop over chemical reporting groups

                }               //end if imask != nodatatavalue

            }                   //end loop over columns

        }                       //end loop over rows

        //loop over chemical reporting groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //Write grid of water column chemical concentrations
            Grid (bndchemconcwatergrid[igroup], concentration[igroup],
                  1.0, gridcount);

        }                       //end loop over chemical reporting groups

    }                           //end if the water column bound chemical concentration grid file root name is not null

    //if the water column particulate chemical concentration grid file (root) name is not null
    if (*prtchemconcwatergridroot != '\0')
    {
        //phase is the particulate chemical (iphase = 3)
        iphase = 3;

        //set values for concentration: chemical concentration in overland or channel cells
        //
        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over number of columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is not null (it is in the active area)
                if (imask[i][j] != nodatavalue)
                {
                    //Compute chemical reporting groups sums (water column)
                    ComputeChemicalGroupConcSums (cgroupsum, i, j, 0, iphase);

                    //loop over chemical reporting groups
                    for (igroup = 1; igroup <= ncgroups; igroup++)
                    {
                        //assign cell concentrations
                        concentration[igroup][i][j] = cgroupsum[igroup];

                    }           //end loop over chemical reporting groups

                }               //end if imask != nodatatavalue

            }                   //end loop over columns

        }                       //end loop over rows

        //loop over chemical reporting groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //Write grid of water column chemical concentrations
            Grid (prtchemconcwatergrid[igroup], concentration[igroup],
                  1.0, gridcount);

        }                       //end loop over chemical reporting groups

    }                           //end if the water column particulate chemical concentration grid file root name is not null

    //if the surface sediment layer total chemical concentration grid file (root) name is not null
    if (*totchemconcsurfgridroot != '\0')
    {
        //phase is the total chemical (iphase = 0)
        iphase = 0;

        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over number of columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is not null (it is in the active area)
                if (imask[i][j] != nodatavalue)
                {
                    //Compute chemical reporting groups sums (surface layer)
                    ComputeChemicalGroupConcSums (cgroupsum, i, j, 1, iphase);

                    //loop over chemical reporting groups
                    for (igroup = 1; igroup <= ncgroups; igroup++)
                    {
                        //assign cell concentrations
                        concentration[igroup][i][j] = cgroupsum[igroup];

                    }           //end loop over chemical reporting groups

                }               //end if imask != nodatatavalue

            }                   //end loop over columns

        }                       //end loop over rows

        //loop over chemical reporting groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //Write grid of surface sediment chemical concentrations
            Grid (totchemconcsurfgrid[igroup], concentration[igroup],
                  1.0, gridcount);

        }                       //end loop over chemical reporting groups

    }                           //end if the surface sediment total chemical grid file root name is not null

    //if the surface sediment layer dissolved chemical concentration grid file (root) name is not null
    if (*dischemconcsurfgridroot != '\0')
    {
        //phase is the dissolved chemical (iphase = 1)
        iphase = 1;

        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over number of columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is not null (it is in the active area)
                if (imask[i][j] != nodatavalue)
                {
                    //Compute chemical reporting groups sums (surface layer)
                    ComputeChemicalGroupConcSums (cgroupsum, i, j, 1, iphase);

                    //loop over chemical reporting groups
                    for (igroup = 1; igroup <= ncgroups; igroup++)
                    {
                        //assign cell concentrations
                        concentration[igroup][i][j] = cgroupsum[igroup];

                    }           //end loop over chemical reporting groups

                }               //end if imask != nodatatavalue

            }                   //end loop over columns

        }                       //end loop over rows

        //loop over chemical reporting groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //Write grid of surface sediment chemical concentrations
            Grid (dischemconcsurfgrid[igroup], concentration[igroup],
                  1.0, gridcount);

        }                       //end loop over chemical reporting groups

    }                           //end if the surface sediment total chemical concentration grid file root name is not null

    //if the surface sediment layer bound chemical concentration grid file (root) name is not null
    if (*bndchemconcsurfgridroot != '\0')
    {
        //phase is the bound chemical (iphase = 2)
        iphase = 2;

        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over number of columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is not null (it is in the active area)
                if (imask[i][j] != nodatavalue)
                {
                    //Compute chemical reporting groups sums (surface layer)
                    ComputeChemicalGroupConcSums (cgroupsum, i, j, 1, iphase);

                    //loop over chemical reporting groups
                    for (igroup = 1; igroup <= ncgroups; igroup++)
                    {
                        //assign cell concentrations
                        concentration[igroup][i][j] = cgroupsum[igroup];

                    }           //end loop over chemical reporting groups

                }               //end if imask != nodatatavalue

            }                   //end loop over columns

        }                       //end loop over rows

        //loop over chemical reporting groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //Write grid of surface sediment chemical concentrations
            Grid (bndchemconcsurfgrid[igroup], concentration[igroup],
                  1.0, gridcount);

        }                       //end loop over chemical reporting groups

    }                           //end if the surface sediment bound chemical concentration grid file root name is not null

    //if the surface sediment layer particulate chemical concentration grid file (root) name is not null
    if (*prtchemconcsurfgridroot != '\0')
    {
        //phase is the particulate chemical (iphase = 3)
        iphase = 3;

        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over number of columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is not null (it is in the active area)
                if (imask[i][j] != nodatavalue)
                {
                    //Compute chemical reporting groups sums (surface layer)
                    ComputeChemicalGroupConcSums (cgroupsum, i, j, 1, iphase);

                    //loop over chemical reporting groups
                    for (igroup = 1; igroup <= ncgroups; igroup++)
                    {
                        //assign cell concentrations
                        concentration[igroup][i][j] = cgroupsum[igroup];

                    }           //end loop over chemical reporting groups

                }               //end if imask != nodatatavalue

            }                   //end loop over columns

        }                       //end loop over rows

        //loop over chemical reporting groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //Write grid of surface sediment chemical concentrations
            Grid (prtchemconcsurfgrid[igroup], concentration[igroup],
                  1.0, gridcount);

        }                       //end loop over chemical reporting groups

    }                           //end if the surface sediment particulate chemical concentration grid file root name is not null

    //if the water column dissolved fraction grid file (root) name is not null
    if (*dischemfracwatergridroot != '\0')
    {
        //phase is the dissolved fraction (iphase = 1)
        iphase = 1;

        //set values for fraction: chemical fraction in overland or channel cells
        //
        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over number of columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is not null (it is in the active area)
                if (imask[i][j] != nodatavalue)
                {
                    //Compute chemical reporting groups sums (water column)
                    ComputeChemicalGroupPhaseAve (cgroupave, i, j, 0, iphase);

                    //loop over chemical reporting groups
                    for (igroup = 1; igroup <= ncgroups; igroup++)
                    {
                        //assign cell fractions
                        fraction[igroup][i][j] = cgroupave[igroup];

                    }           //end loop over chemical reporting groups

                }               //end if imask != nodatatavalue

            }                   //end loop over columns

        }                       //end loop over rows

        //loop over chemical reporting groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //Write grid of water column chemical fractions
            Grid (dischemfracwatergrid[igroup], fraction[igroup],
                  1.0, gridcount);

        }                       //end loop over chemical reporting groups

    }                           //end if the water column dissolved fraction grid file root name is not null

    //if the water column bound fraction grid file (root) name is not null
    if (*bndchemfracwatergridroot != '\0')
    {
        //phase is the bound fraction (iphase = 2)
        iphase = 2;

        //set values for fraction: chemical fraction in overland or channel cells
        //
        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over number of columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is not null (it is in the active area)
                if (imask[i][j] != nodatavalue)
                {
                    //Compute chemical reporting groups sums (water column)
                    ComputeChemicalGroupPhaseAve (cgroupave, i, j, 0, iphase);

                    //loop over chemical reporting groups
                    for (igroup = 1; igroup <= ncgroups; igroup++)
                    {
                        //assign cell fractions
                        fraction[igroup][i][j] = cgroupave[igroup];

                    }           //end loop over chemical reporting groups

                }               //end if imask != nodatatavalue

            }                   //end loop over columns

        }                       //end loop over rows

        //loop over chemical reporting groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //Write grid of water column chemical fractions
            Grid (bndchemfracwatergrid[igroup], fraction[igroup],
                  1.0, gridcount);

        }                       //end loop over chemical reporting groups

    }                           //end if the water column bound fraction grid file root name is not null

    //if the water column mobile chemical fraction grid file (root) name is not null
    if (*mblchemfracwatergridroot != '\0')
    {
        //phase is the mobile fraction (iphase = 3)
        iphase = 3;

        //set values for fraction: chemical fraction in overland or channel cells
        //
        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over number of columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is not null (it is in the active area)
                if (imask[i][j] != nodatavalue)
                {
                    //Compute chemical reporting groups sums (water column)
                    ComputeChemicalGroupPhaseAve (cgroupave, i, j, 0, iphase);

                    //loop over chemical reporting groups
                    for (igroup = 1; igroup <= ncgroups; igroup++)
                    {
                        //assign cell fractions
                        fraction[igroup][i][j] = cgroupave[igroup];

                    }           //end loop over chemical reporting groups

                }               //end if imask != nodatatavalue

            }                   //end loop over columns

        }                       //end loop over rows

        //loop over chemical reporting groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //Write grid of water column chemical fractions
            Grid (mblchemfracwatergrid[igroup], fraction[igroup],
                  1.0, gridcount);

        }                       //end loop over chemical reporting groups

    }                           //end if the water column mobile fraction grid file root name is not null

    //if the water column particulate fraction grid file (root) name is not null
    if (*prtchemfracwatergridroot != '\0')
    {
        //phase is the particulate fraction (iphase = 4)
        iphase = 4;

        //set values for fraction: chemical fraction in overland or channel cells
        //
        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over number of columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is not null (it is in the active area)
                if (imask[i][j] != nodatavalue)
                {
                    //Compute chemical reporting groups sums (water column)
                    ComputeChemicalGroupPhaseAve (cgroupave, i, j, 0, iphase);

                    //loop over chemical reporting groups
                    for (igroup = 1; igroup <= ncgroups; igroup++)
                    {
                        //assign cell fractions
                        fraction[igroup][i][j] = cgroupave[igroup];

                    }           //end loop over chemical reporting groups

                }               //end if imask != nodatatavalue

            }                   //end loop over columns

        }                       //end loop over rows

        //loop over chemical reporting groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //Write grid of water column chemical fractions
            Grid (prtchemfracwatergrid[igroup], fraction[igroup],
                  1.0, gridcount);

        }                       //end loop over chemical reporting groups

    }                           //end if the water column particulate fraction grid file root name is not null

    //if the surface sediment layer dissolved fraction grid file (root) name is not null
    if (*dischemfracsurfgridroot != '\0')
    {
        //phase is the dissolved fraction (iphase = 1)
        iphase = 1;

        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over number of columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is not null (it is in the active area)
                if (imask[i][j] != nodatavalue)
                {
                    //Compute chemical reporting groups sums (surface layer)
                    ComputeChemicalGroupPhaseAve (cgroupave, i, j, 1, iphase);

                    //loop over chemical reporting groups
                    for (igroup = 1; igroup <= ncgroups; igroup++)
                    {
                        //assign cell fractions
                        fraction[igroup][i][j] = cgroupave[igroup];

                    }           //end loop over chemical reporting groups

                }               //end if imask != nodatatavalue

            }                   //end loop over columns

        }                       //end loop over rows

        //loop over chemical reporting groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //Write grid of surface sediment chemical fractions
            Grid (dischemfracsurfgrid[igroup], fraction[igroup],
                  1.0, gridcount);

        }                       //end loop over chemical reporting groups

    }                           //end if the surface sediment dissolved fraction grid file root name is not null

    //if the surface sediment layer bound fraction grid file (root) name is not null
    if (*bndchemfracsurfgridroot != '\0')
    {
        //phase is the bound fraction (iphase = 2)
        iphase = 2;

        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over number of columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is not null (it is in the active area)
                if (imask[i][j] != nodatavalue)
                {
                    //Compute chemical reporting groups sums (surface layer)
                    ComputeChemicalGroupPhaseAve (cgroupave, i, j, 1, iphase);

                    //loop over chemical reporting groups
                    for (igroup = 1; igroup <= ncgroups; igroup++)
                    {
                        //assign cell fractions
                        fraction[igroup][i][j] = cgroupave[igroup];

                    }           //end loop over chemical reporting groups

                }               //end if imask != nodatatavalue

            }                   //end loop over columns

        }                       //end loop over rows

        //loop over chemical reporting groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //Write grid of surface sediment chemical fractions
            Grid (bndchemfracsurfgrid[igroup], fraction[igroup],
                  1.0, gridcount);

        }                       //end loop over chemical reporting groups

    }                           //end if the surface sediment bound fraction grid file root name is not null

    //if the surface sediment layer mobile chemical fraction grid file (root) name is not null
    if (*mblchemfracsurfgridroot != '\0')
    {
        //phase is the mobile fraction (iphase = 3)
        iphase = 3;

        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over number of columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is not null (it is in the active area)
                if (imask[i][j] != nodatavalue)
                {
                    //Compute chemical reporting groups sums (surface layer)
                    ComputeChemicalGroupPhaseAve (cgroupave, i, j, 1, iphase);

                    //loop over chemical reporting groups
                    for (igroup = 1; igroup <= ncgroups; igroup++)
                    {
                        //assign cell fractions
                        fraction[igroup][i][j] = cgroupave[igroup];

                    }           //end loop over chemical reporting groups

                }               //end if imask != nodatatavalue

            }                   //end loop over columns

        }                       //end loop over rows

        //loop over chemical reporting groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //Write grid of surface sediment chemical fractions
            Grid (mblchemfracsurfgrid[igroup], fraction[igroup],
                  1.0, gridcount);

        }                       //end loop over chemical reporting groups

    }                           //end if the surface sediment mobile fraction grid file root name is not null

    //if the surface sediment layer particulate fraction grid file (root) name is not null
    if (*prtchemfracsurfgridroot != '\0')
    {
        //phase is the particulate fraction (iphase = 4)
        iphase = 4;

        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over number of columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is not null (it is in the active area)
                if (imask[i][j] != nodatavalue)
                {
                    //Compute chemical reporting groups sums (surface layer)
                    ComputeChemicalGroupPhaseAve (cgroupave, i, j, 1, iphase);

                    //loop over chemical reporting groups
                    for (igroup = 1; igroup <= ncgroups; igroup++)
                    {
                        //assign cell fractions
                        fraction[igroup][i][j] = cgroupave[igroup];

                    }           //end loop over chemical reporting groups

                }               //end if imask != nodatatavalue

            }                   //end loop over columns

        }                       //end loop over rows

        //loop over chemical reporting groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //Write grid of surface sediment chemical fractions
            Grid (prtchemfracsurfgrid[igroup], fraction[igroup],
                  1.0, gridcount);

        }                       //end loop over chemical reporting groups

    }                           //end if the surface sediment particulate fraction grid file root name is not null

    //if the chemical infiltration flux grid file (root) name is not null
    if (*infchemfluxgridroot != '\0')
    {
        //transport path is infiltration (ipath = 0)
        ipath = 0;

        //set values for flux: chemical flux in overland or channel cells
        //
        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Loop over number of columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is not null (it is in the active area)
                if (imask[i][j] != nodatavalue)
                {
                    //Compute chemical reporting groups sums
                    ComputeChemicalGroupTransportSums (cgroupsum, i, j,
                                                       ipath);

                    //loop over chemical reporting groups
                    for (igroup = 1; igroup <= ncgroups; igroup++)
                    {
                        //assign cell chemical infiltration fluxes
                        transport[igroup][i][j] = cgroupsum[igroup];

                    }           //end loop over chemical reporting groups

                }               //end if imask != nodatatavalue

            }                   //end loop over columns

        }                       //end loop over rows

        //loop over chemical reporting groups
        for (igroup = 1; igroup <= ncgroups; igroup++)
        {
            //Write grid of chemical infiltration flux
            Grid (infchemfluxgrid[igroup], transport[igroup], 1.0, gridcount);

        }                       //end loop over chemical reporting groups

    }                           //end if the chemical infiltration flux grid file root name is not null

    //free memory for local arrays
    //
    //loop over reporting groups
    for (igroup = 1; igroup <= ncgroups; igroup++)
    {
        //loop over rows

        for (i = 1; i <= nrows; i++)
        {
            //deallocate memory for local variables 
            free (concentration[igroup][i]);
            free (fraction[igroup][i]);
            free (transport[igroup][i]);

        }                       //end loop over rows

        //deallocate memory for local variables 
        free (concentration[igroup]);
        free (fraction[igroup]);
        free (transport[igroup]);

    }                           //end loop over groups

    //deallocate memory for local variables 
    free (concentration);
    free (fraction);
    free (transport);
    free (cgroupsum);
    free (cgroupave);

//End of function: Return to WriteGrids
}
