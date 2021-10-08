/*---------------------------------------------------------------------
C-  Function:   ReadDataGroupB.c
C-
C-  Purpose/    ReadDataGroupB is called at the start
C-  Methods:    of the simulation to read Data Group B
C-              (hydrologic and hydraulic simulation parameters) from
C-              the model input file.
C-
C-  Inputs:     None
C-
C-  Outputs:    None
C-
C-  Controls:   ksim, infopt, chnopt, tplgyopt, rainopt, dbcopt[i]
C-
C-  Calls:      (in order of appearance/call in function)
C-              StripString, ReadMaskFile, ReadElevationFile,
C-              ReadSoilTypeFile, ReadLandUseFile, ReadStorageDepthFile,
C-              ReadLinkFile, ReadNodeFile, ReadChannelFile,
C-              ComputeChannelTopology,ComputeChannelLength,ComputeChannelElevation,
C-              ReadTopologyFile (dummy),
C-              ReadInitialWaterOverlandFile, ReadInitialWaterChannelFile,
C-              ReadDesignRainGrid, ReadRadarRainLocations, ReadRadarRainRates,
C-              ReadSpaceTimeStorm
C-
C-  Called by:  ReadInputFile
C-
C-  Created:    Mark Velleux
C-              Department of Civil Engineering
C-              Colorado State University
C-              Fort Collins, CO  80523
C-
C-              Aditya Maroo (CSU)
C-
C-  Date:       24-JUN-2004
C-
C-  Revised:    John F. England, Jr.
C-              Bureau of Reclamation
C-              Flood Hydrology Group, D-8530
C-              Bldg. 67, Denver Federal Center
C-              Denver, CO 80225
C-
C-  Date:       18-JUL-2004
C-
C-  Revisions:  Additional rain and snow options, Emacs formatting
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
C-  Revisions:	Added SMA options
C-
C-  Revised:    James Halgren
C-              Colorado State University
C-              james.halgren@colostate.edu
C-
C-  Date:       26-Jan-2009
C-
C----------------------------------------------------------------------*/

//trex global variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for water transport
#include "trex_water_declarations.h"

//variable declarations and definitions specific to the 
//soil moisture accounting procedure.
#include "trex_SMA_declarations.h"

void ReadDataGroupB(void)
{
    //local variable declarations/definitions
    float convunits,            //conversion factors for length, area, volume or mass units
      convtime,                 //conversion factor for time units
      scale;                    //scale factor

    //open the echo file in append mode (append to existing file)
    echofile_fp = fopen (echofile, "a");

    //write message to screen
    printf ("\n\n***************************\n");
    printf ("*                         *\n");
    printf ("*   Reading Data Group B  *\n");
    printf ("*                         *\n");
    printf ("***************************\n\n\n");

    //Record 1
    fgets (header, MAXHEADERSIZE, inputfile_fp);        //read header line from input file

    //Echo header to file
    fprintf (echofile_fp, "\n\n\n%s\n\n", header);

    //Record 2 (Part 1)
    fscanf (inputfile_fp, "%s", varname);       //read dummy

    //Allocate memory for mask file name
    maskfile = (char *) malloc (MAXNAMESIZE * sizeof (char));

    //Record 2 (Part 2)
    fgets (maskfile, MAXNAMESIZE, inputfile_fp);        //read mask file name

    //strip leading blanks, trailing blanks, final carriage return from string
    maskfile = StripString (maskfile);

    //Echo mask file name to file
    fprintf (echofile_fp, "\nMask File: %s\n\n", maskfile);

    //Read Mask File
    ReadMaskFile ();

    //Record 3 (Part 1)
    fscanf (inputfile_fp, "%s", varname);       //read dummy

    //Allocate memory for elevation file name
    elevationfile = (char *) malloc (MAXNAMESIZE * sizeof (char));

    //Record 3 (Part 2)
    fgets (elevationfile, MAXNAMESIZE, inputfile_fp);   //read elevation file name

    //strip leading blanks, trailing blanks, final carriage return from string
    elevationfile = StripString (elevationfile);

    //Echo elevation file name to file
    fprintf (echofile_fp, "\nElevation File: %s\n\n", elevationfile);

    //Read Elevation File
    ReadElevationFile ();

    //Record 4
    fscanf (inputfile_fp, "%s %d",      //read
            varname,            //dummy
            &infopt);           //inftration option

    //Echo Infiltration Option to file
    fprintf (echofile_fp, "\n\n\nInfiltration Option = %d\n", infopt);

    //Check value of ksim (if simulation type is hydrology only...)
    if (ksim == 1)
    {
        //Check value of infopt
        if (infopt > 0)
        {
            //Check if infopt == 2 -> Soil Moisture Accounting
            if (infopt == 2)
            {
                //Record SMA (Part 1)
                fscanf (inputfile_fp, "%s", varname);   //read dummy

                //Allocate memory for SMA file name
                SMAfile = (char *) malloc (MAXNAMESIZE * sizeof (char));

                //Record SMA (Part 2)
                fgets (SMAfile, MAXNAMESIZE, inputfile_fp);     //read SMA file name

                //strip leading blanks, trailing blanks, final carriage return from string
                SMAfile = StripString (SMAfile);

                //Echo SMA File name to file
                fprintf (echofile_fp, "\n\nSMA File: %s\n", SMAfile);

                ReadSMAFile ();

            }
            //Record 5
            fscanf (inputfile_fp, "%s %d",      //read
                    varname,    //dummy
                    &nsoils);   //Number of soil types

            //Echo Number of soil types to file
            fprintf (echofile_fp, "\n\nNumber of Soil types = %d\n", nsoils);

            //Allocate memory for infitration parameters
            khsoil = (float *) malloc ((nsoils + 1) * sizeof (float));  //allocate memory
            capshsoil = (float *) malloc ((nsoils + 1) * sizeof (float));       //allocate memory
            soilmd = (float *) malloc ((nsoils + 1) * sizeof (float));  //allocate memory
            soilname = (char **) malloc ((nsoils + 1) * sizeof (char *));       //allocate memory

            //Write label for land use parameters to file
            fprintf (echofile_fp, "\n  Hydraulic Conductivity (m/s)    ");
            fprintf (echofile_fp, "  Capillary Suction Head (m)    ");
            fprintf (echofile_fp, "  Soil Moisture Deficit (-)    ");
            fprintf (echofile_fp, "  Soil Type Description  \n");
            fprintf (echofile_fp, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ");
            fprintf (echofile_fp, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ");
            fprintf (echofile_fp, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ");
            fprintf (echofile_fp, "~~~~~~~~~~~~~~~~~~~~~~~~~\n");

            //Loop over the number of soil types
            for (i = 1; i <= nsoils; i++)
            {
                //Allocate remaining memory for soilname...
                soilname[i] = (char *) malloc (MAXNAMESIZE * sizeof (char));

                //Record 6 (Part 1)
                fscanf (inputfile_fp, "%f %f %f",       //read
                        &khsoil[i],     //Soil Hydraulic Conductivity
                        &capshsoil[i],  //Soil Capillary Suction Head
                        &soilmd[i]);    //Soil Moisture Deficit

                //Record 6 (Part 2)
                fgets (soilname[i], MAXNAMESIZE, inputfile_fp); //Read Soil Name

                //Echo Hydraulic Conductivity, Capillary Suction Head
                //& Soil Moisture Deficit to file
                fprintf (echofile_fp, "%32.8f  %29.3f  %29.3f  %s",
                         khsoil[i], capshsoil[i], soilmd[i], soilname[i]);

            }                   //End loop over soil types

            //Record 7 (Part 1)
            fscanf (inputfile_fp, "%s", varname);       //read dummy

            //Allocate memory for soil file name
            soiltypefile = (char *) malloc (MAXNAMESIZE * sizeof (char));

            //Record 7 (Part 2)
            fgets (soiltypefile, MAXNAMESIZE, inputfile_fp);    //read soil file name

            //strip leading blanks, trailing blanks, final carriage return from string
            soiltypefile = StripString (soiltypefile);

            //Echo Soil Type Classification File name to file
            fprintf (echofile_fp, "\n\nSoil Type Classification File: %s\n",
                     soiltypefile);

            //Allocate memory for soil types
            soiltype = (int ***) malloc ((nrows + 1) * sizeof (int **));        //allocate memory

            //Loop over number of rows
            for (i = 1; i <= nrows; i++)
            {
                //Allocate additional memory for soil types
                soiltype[i] = (int **) malloc ((ncols + 1) * sizeof (int *));   //allocate memory

                //Loop over number of columns
                for (j = 1; j <= ncols; j++)
                {
                    //Allocate remaining memory for soil types
                    soiltype[i][j] = (int *) malloc (2 * sizeof (int)); //allocate memory

                }               //end loop over columns

            }                   //end loop over rows

            //Read Soil Type Classification File (for ksim = 1, soil layer = 1)
            ReadSoilTypeFile (1);

        }                       //End check of infopt

        //Record 8
        fscanf (inputfile_fp, "%s %d", varname, //dummy
                &nlands);       //Number of Land Use Classes

        //Echo Number of Land Use Classes to file
        fprintf (echofile_fp, "\n\nNumber of Land Use Classes %d\n", nlands);

        //Allocate memory to land use class parameters
        nmanningov = (float *) malloc ((nlands + 1) * sizeof (float));  //allocate memory
        interceptionclass = (float *) malloc ((nlands + 1) * sizeof (float));   //allocate memory
        landname = (char **) malloc ((nlands + 1) * sizeof (char *));   //allocate memory

        //Loop over number of land use classes to allocate remaining memory
        for (i = 1; i <= nlands; i++)
        {
            landname[i] = (char *) malloc (MAXNAMESIZE * sizeof (char));
        }

        //Print label for land use parameters to file
        fprintf (echofile_fp, "\n  Manning n     ");
        fprintf (echofile_fp, "  Interception Depth (mm)     ");
        fprintf (echofile_fp, "  Land Use Classification  \n");
        fprintf (echofile_fp, "~~~~~~~~~~~~~   ");
        fprintf (echofile_fp, "~~~~~~~~~~~~~~~~~~~~~~~~~~~   ");
        fprintf (echofile_fp, "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

        //Loop over number of land use classes to read land use parameters
        for (i = 1; i <= nlands; i++)
        {
            //Record 9 (Part 1)
            fscanf (inputfile_fp, "%f %f",      //read
                    &nmanningov[i],     //Manning n
                    &interceptionclass[i]);     //interception depth (mm) for this land use

            //Record 9 (Part 2)
            fgets (landname[i], MAXNAMESIZE, inputfile_fp);     //Read Land Use Class Name

            //Echo Manning n, interception depth for current land use, and
            //  land use classification name to echo file
            fprintf (echofile_fp, "%13.4f   %27.4f   %s",
                     nmanningov[i], interceptionclass[i], landname[i]);

        }                       //End loop over land use classes

        //Record 10 (Part 1)
        fscanf (inputfile_fp, "%s", varname);   //read dummy

        //Allocate memory for land use file name
        landusefile = (char *) malloc (MAXNAMESIZE * sizeof (char));

        //Record 10 (Part 2)
        fgets (landusefile, MAXNAMESIZE, inputfile_fp); //read land use file name

        //strip leading blanks, trailing blanks, final carriage return from string
        landusefile = StripString (landusefile);

        //Echo land use classification file name to file
        fprintf (echofile_fp, "\n\nLand Use Classification File %s\n\n",
                 landusefile);

        //Read Land Use Classification File
        ReadLandUseFile ();

    }                           //End check of ksim = 1

    //Record 11 (Part 1) (depression storage depth for overland plane)
    fscanf (inputfile_fp, "%s", varname);       //read dummy

    //Allocate memory for storage depth file name
    storagedepthfile = (char *) malloc (MAXNAMESIZE * sizeof (char));

    //Record 11 (Part 2)
    fgets (storagedepthfile, MAXNAMESIZE, inputfile_fp);        //read storage depth file name

    //strip leading blanks, trailing blanks, final carriage return from string
    storagedepthfile = StripString (storagedepthfile);

    //Echo Storage Depth File name to file
    fprintf (echofile_fp, "\n\nStorage Depth File: %s\n", storagedepthfile);

    //Read Storage Depth File (storage depth for each grid cell in overland plane)
    ReadStorageDepthFile ();

    //Record 12
    fscanf (inputfile_fp, "%s %d",      //read
            varname,            //dummy
            &chnopt);           //Channel Option

    //Echo Channel Option to file
    fprintf (echofile_fp, "\n\n\nChannel Option = %d\n", chnopt);

    //Check value of chnopt (if chnopt = 1 then read channel network info)
    if (chnopt == 1)
    {
        //Record 13
        fscanf (inputfile_fp, "%s %d %s %d %s %d %s %d",        //read
                varname,        //dummy
                &tplgyopt,      //topology option
                varname,        //dummy
                &ctlopt,        //channel transmission loss option
                varname,        //dummy
                &fldopt,        //floodplain transfer option
                varname,        //dummy
                &outopt);       //outlet routing option

        //Echo Topology Option and others to file
        fprintf (echofile_fp, "\n\nTopology Option = %d\n", tplgyopt);
        fprintf (echofile_fp, "Channel Transmission Loss Option = %d\n",
                 ctlopt);
        fprintf (echofile_fp, "Floodplain Transfer Option = %d\n", fldopt);
        fprintf (echofile_fp, "Outlet Routing Option = %d\n", outopt);

        //Check value of tplgyopt
        if (tplgyopt == 0)      //trex computes channel network topology
        {
            //Record 14 (Part 1)
            fscanf (inputfile_fp, "%s", varname);       //read dummy

            //Allocate memory for link file name
            linkfile = (char *) malloc (MAXNAMESIZE * sizeof (char));

            //Record 14 (Part 2)
            fgets (linkfile, MAXNAMESIZE, inputfile_fp);        //read link file name

            //strip leading blanks, trailing blanks, final carriage return from string
            linkfile = StripString (linkfile);

            //Echo Link File name to file
            fprintf (echofile_fp, "\n\nLink File: %s\n", linkfile);

            //Read Link File
            ReadLinkFile ();

            //Record 15 (Part 1)
            fscanf (inputfile_fp, "%s", varname);       //read dummy

            //Allocate memory for node file name
            nodefile = (char *) malloc (MAXNAMESIZE * sizeof (char));

            //Record 15 (Part 2)
            fgets (nodefile, MAXNAMESIZE, inputfile_fp);        //read node file name

            //strip leading blanks, trailing blanks, final carriage return from string
            nodefile = StripString (nodefile);

            //Echo Node File name to file
            fprintf (echofile_fp, "\n\nNode File: %s\n", nodefile);

            //Read Node File
            ReadNodeFile ();

            //Record 16 (Part 1) (channel properties including storage depth/dead storage)
            fscanf (inputfile_fp, "%s", varname);       //read dummy

            //Allocate memory for channel file name
            channelfile = (char *) malloc (MAXNAMESIZE * sizeof (char));

            //Record 16 (Part 2)
            fgets (channelfile, MAXNAMESIZE, inputfile_fp);     //read channel property file name

            //strip leading blanks, trailing blanks, final carriage return from string
            channelfile = StripString (channelfile);

            //Echo Channel Property File name to file
            fprintf (echofile_fp, "\n\nChannel File: %s\n", channelfile);

            //Read Channel Properties File
            ReadChannelFile ();

            //Compute channel topology from link, node, and channel property files
            ComputeChannelTopology ();

            //Compute stream length of each channel element (link/node)
            ComputeChannelLength ();

            //Compute initial bed elevation of each channel element (link/node)
            ComputeChannelElevation ();
        }
        else                    //trex reads channel network topology directly from file
        {
            //Record 17 (Part 1) (external channel network topology/property file)
            fscanf (inputfile_fp, "%s", varname);       //read dummy

            //Allocate memory for topology file name
            topologyfile = (char *) malloc (MAXNAMESIZE * sizeof (char));       //allocate memory

            //Record 17 (Part 2)
            fgets (topologyfile, MAXNAMESIZE, inputfile_fp);    //read topology file name

            //strip leading blanks, trailing blanks, and final carriage return from string
            topologyfile = StripString (topologyfile);

            //echo topology file name to file
            fprintf (echofile_fp, "\n\nTopology File %s\n\n", topologyfile);

            //Read topology file (for future development)
            //mlv  ReadChannelTopologyFile();

        }                       //end if tplgyopt = 1

        //Note:  If ksim = 1, transmission loss parameters are read
        //       here.  Otherwise, they are read with other sediment
        //       properties in Data Group C.
        //
        //Check value of ksim (if simulation type is hydrology only...)
        if (ksim == 1)
        {
            //if transmission loss is simulated (ctlopt > 0)
            if (ctlopt > 0)
            {
                //Record 18 (Part 1) (transmission loss parameter file)
                fscanf (inputfile_fp, "%s", varname);   //read dummy

                //Allocate memory for channel transmission loss property file name
                channeltlossfile = (char *) malloc (MAXNAMESIZE * sizeof (char));       //allocate memory

                //Record 18 (Part 2)
                fgets (channeltlossfile, MAXNAMESIZE, inputfile_fp);    //read channel transmission loss property file name

                //strip leading blanks, trailing blanks, and final carriage return from string
                channeltlossfile = StripString (channeltlossfile);

                //echo channel transmission loss property file name to file
                fprintf (echofile_fp,
                         "\n\nChannel Transmission Loss Property File %s\n\n",
                         channeltlossfile);

                //Read channel transmission loss property file
                ReadChannelTransmissionFile ();

            }                   //end if ctlopt > 0

        }                       //end if ksim == 1

    }                           //end if chnopt = 1

    //Record 19 (Part 1)
    fscanf (inputfile_fp, "%s", varname);       //read dummy

    //Allocate memory for initial waterdepth overland file name
    wateroverlandfile = (char *) malloc (MAXNAMESIZE * sizeof (char));

    //Record 19 (Part 2)
    //read initial water depth overland file name
    fgets (wateroverlandfile, MAXNAMESIZE, inputfile_fp);

    //strip leading blanks, trailing blanks, and final carriage return from string
    wateroverlandfile = StripString (wateroverlandfile);

    //Echo Initial Water Overland to file
    fprintf (echofile_fp, "\n\nInitial Water Depth Overland: %s\n",
             wateroverlandfile);

    //Read initial water in storage overland file
    ReadInitialWaterOverlandFile ();

    //if infopt > 0, read initial infiltration depths (this is really initial soil moisture)
    if (infopt > 0)
    {
        //Record 20 (Part 1)
        fscanf (inputfile_fp, "%s", varname);   //read dummy

        //Allocate memory for initial infiltration depth file name
        infiltrationfile = (char *) malloc (MAXNAMESIZE * sizeof (char));

        //Record 20 (Part 2)
        //read initial infiltration depth file name
        fgets (infiltrationfile, MAXNAMESIZE, inputfile_fp);

        //strip leading blanks, trailing blanks, final carriage return from string
        infiltrationfile = StripString (infiltrationfile);

        //Echo initial infiltration depth (depth to wetting front) file name to file
        fprintf (echofile_fp, "\n\nInitial Infiltration Depth: %s\n",
                 infiltrationfile);

        //Read initial infiltration depth file
        ReadInitialInfiltrationFile ();

    }                           //end if infopt > 0

    //Check value of channel option
    if (chnopt == 1)
    {
        //Record 21 (Part 1)
        fscanf (inputfile_fp, "%s", varname);   //read dummy

        //Allocate memory for initial water depth in channels file name
        waterchannelfile = (char *) malloc (MAXNAMESIZE * sizeof (char));

        //Record 21 (Part 2)
        //read initial water depth in channel network file name
        fgets (waterchannelfile, MAXNAMESIZE, inputfile_fp);

        //strip leading blanks, trailing blanks, final carriage return from string
        waterchannelfile = StripString (waterchannelfile);

        //Echo Initial Water in Channels to file
        fprintf (echofile_fp, "\n\nInitial Water Depth in Channels: %s\n",
                 waterchannelfile);

        //Read initial water depth in channel network file
        ReadInitialWaterChannelFile ();

        //if transmission loss is simulated (ctlopt > 0)
        if (ctlopt > 0)
        {
            //Record 22 (Part 1)
            fscanf (inputfile_fp, "%s", varname);       //read dummy

            //Allocate memory for initial transmission loss file name
            translossfile = (char *) malloc (MAXNAMESIZE * sizeof (char));

            //Record 22 (Part 2)
            //read initial transmission loss depth (depth to wetting front) file name
            fgets (translossfile, MAXNAMESIZE, inputfile_fp);

            //strip leading blanks, trailing blanks, final carriage return from string
            translossfile = StripString (translossfile);

            //Echo Initial Water in Channels to file
            fprintf (echofile_fp, "\n\nInitial Transmission Loss Depth: %s\n",
                     translossfile);

            //Read initial transmission loss depth file
            ReadInitialTransmissionLossFile ();

        }                       //end if ctlopt > 0

    }                           //end check of chnopt

    //Write label for hydrologic forcing functions/boundary conditions
    fprintf (echofile_fp,
             "\n\n  Hydrologic Forcing Functions/Boundary Conditions  \n");
    fprintf (echofile_fp,
             "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

    //Write label for rainfall functions to file
    fprintf (echofile_fp, "\n  Rainfall Functions  \n");
    fprintf (echofile_fp, "~~~~~~~~~~~~~~~~~~~~~~\n\n");

    //Record 23
    fscanf (inputfile_fp, "%s %d",      //read
            varname,            //dummy
            &rainopt);          //Rainfall Data Interpolation Option

    //Echo Rain Option to file
    fprintf (echofile_fp, "Rainfall Interpolation Option = %d\n\n", rainopt);

    //Check for valid rainfall interpolation/modeling option
    if (rainopt < 0 || rainopt > 5)
    {
        //Write error message to file
        fprintf (echofile_fp, "\n\n\nData Group Error:\n");
        fprintf (echofile_fp, "  Rainfall option > 5 not a valid option\n");
        fprintf (echofile_fp, "  User selected rainopt = %5d\n", rainopt);
        fprintf (echofile_fp, "  Select rainopt = 0,1,2,3,4 or 5");

        //Write error message to screen
        printf ("\n\n\nData Group Error:\n");
        printf ("  Rainfall option > 5 not a valid option!\n");
        printf ("  User selected rainopt = %5d\n", rainopt);
        printf ("  Select rainopt = 0,1,2,3,4 or 5");

        exit (EXIT_FAILURE);    //abort

    }                           //End check on rainopt > 5

    //jfe NOTE!! Temporary Radar Patch:
    //jfe  Use IDW for Radar Spatial Interpolation

    //if IDW spatial interpolation selected
    //jfe ideal  if(rainopt == 1)
    if ((rainopt == 1) || (rainopt == 3))       //alternate for both gage and radar
    {
        //Record 24 (read IDW parameters)
        fscanf (inputfile_fp, "%s %f %s %f",    //read
                varname,        //dummy
                &idwradius,     //IDW radius of influence
                varname,        //dummy
                &idwexponent);  //IDW weighting exponent

        //Echo IDW parameters to file
        fprintf (echofile_fp,
                 "\nInverse Distance Weighting (IDW) selected.\n");
        fprintf (echofile_fp, "\n  IDW Radius of Influence (m) = %f",
                 idwradius);
        fprintf (echofile_fp, "\n  IDW Weighting Exponent = %f\n",
                 idwexponent);

    }                           //end if rainopt = 1

    //Read rain gage data from this main file for options 0, 1, and 2
    //
    //if rainopt <= 2 (uniform or distributed gage data)
    if (rainopt <= 2)
    {
        //Record 25
        fscanf (inputfile_fp, "%s %d",  //read
                varname,        //dummy
                &nrg);          //Number of Rain Gages

        //Echo number of rain gages to file
        fprintf (echofile_fp, "Number of Rain Gages = %d\n\n", nrg);

        //error check for rainopt = 0 (uniform in space) and nrg > 1
        if (rainopt == 0 && nrg > 1)
        {
            //Write error message to file
            fprintf (echofile_fp, "\n\n\nData Group Error:\n");
            fprintf (echofile_fp, "  Rainfall option = %5d\n", rainopt);
            fprintf (echofile_fp, "  Number of Rain Gages = %5d\n", nrg);
            fprintf (echofile_fp, "  Only one rain gage can be specified");

            //Write error message to screen
            printf ("\n\n\nData Group Error:\n");
            printf ("  Rainfall option = %5d\n", rainopt);
            printf ("  Number of Rain Gages = %5d\n", nrg);
            printf ("  Only one rain gage can be specified");

            exit (EXIT_FAILURE);        //abort
        }                       //end rainopt = 0 and nrg > 1

        //if raingages are specified (nrg > 0)
        if (nrg > 0)
        {
            //Record 26
            fscanf (inputfile_fp, "%s %f %s %f %s %f",  //read
                    varname,    //dummy
                    &convunits, //units conversion factor
                    varname,    //dummy
                    &convtime,  //time conversion factor
                    varname,    //dummy
                    &scale);    //scale factor

            //Echo converson and scale factors to file
            fprintf (echofile_fp, "Units Conversion Factor = %f\n",
                     convunits);
            fprintf (echofile_fp, "Time Conversion Factor = %f\n", convtime);
            fprintf (echofile_fp, "Scale Factor = %f\n\n", scale);

            //Allocate memory for rain gage parameters
            //
            //rain gage id
            rgid = (int *) malloc ((nrg + 1) * sizeof (int));

            //Gage x coordinate (m)
            rgx = (float *) malloc ((nrg + 1) * sizeof (float));

            //Gage y coordinate (m)
            rgy = (float *) malloc ((nrg + 1) * sizeof (float));

            //number of pairs in rainfall time series
            nrpairs = (int *) malloc ((nrg + 1) * sizeof (int));

            //value of rainfall intensity (m/s)
            rfintensity = (float **) malloc ((nrg + 1) * sizeof (float *));

            //time break in rainfall intensity time series (days)
            rftime = (float **) malloc ((nrg + 1) * sizeof (float *));

            //Loop over number of rain gages (rainfall functions)
            for (i = 1; i <= nrg; i++)
            {
                //Record 27
                fscanf (inputfile_fp, "%s %d %f %f %d", //read
                        varname,        //dummy
                        &rgid[i],       //Gage number
                        &rgx[i],        //Gage x coordinate (m)
                        &rgy[i],        //Gage y coordinate (m)
                        &nrpairs[i]);   //Number of Pairs

                //Echo Gage number, Gage X-Coord, Gage Y-Coord and Number of Pairs to file
                fprintf (echofile_fp, "\n\nRain Gage Number = %d\n", rgid[i]);
                fprintf (echofile_fp, "\n   Location:\n");
                fprintf (echofile_fp, "      x coordinate = %f\n", rgx[i]);
                fprintf (echofile_fp, "      y coordinate = %f\n", rgy[i]);
                fprintf (echofile_fp,
                         "\n   Number of Pairs in Time Series = %d\n\n",
                         nrpairs[i]);

                //Allocate remaining memory for rainfall parameters
                rfintensity[i] =
                    (float *) malloc ((nrpairs[i] + 1) * sizeof (float));
                rftime[i] =
                    (float *) malloc ((nrpairs[i] + 1) * sizeof (float));

                //Write label for rainfall time series to file
                fprintf (echofile_fp, "  Rainfall Intensity (m/s)     ");       //Defacto units!!!
                fprintf (echofile_fp, "  Time (hours)  \n");
                fprintf (echofile_fp, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~   ");
                fprintf (echofile_fp, "~~~~~~~~~~~~~~~~\n");

                //Loop over rainfall intensity time series
                for (j = 1; j <= nrpairs[i]; j++)
                {
                    //Record 28
                    fscanf (inputfile_fp, "%f %f",      //read
                            &rfintensity[i][j], //Rainfall Intensity
                            &rftime[i][j]);     //Rainfall Intensity Time Break

                    //Apply conversion and scale factors
                    rfintensity[i][j] = rfintensity[i][j]
                        * convunits * convtime * scale;

                    //Echo Rainfall Intensity, Rainfall Intensity Time Step to file
                    fprintf (echofile_fp, "%28.8f %18.3f\n",
                             rfintensity[i][j], rftime[i][j]);

                }               //end loop over rainfall intensity time series

            }                   //end loop over rain gages (rainfall functions)

        }                       //end if nrg > 0
    }
    //else if rainopt = 5 (gridded radar rainfall)
    else if (rainopt == 5)
    {
        //Record 29
        fscanf (inputfile_fp, "%s %f",  //read
                varname,        //dummy
                &raingridfreq); //time (frequency) at which rainfall grids are read

        //Echo rainfall grid file read frequency to file
        fprintf (echofile_fp, "Rainfall Grid Read Frequency = %f (hours)\n\n",
                 raingridfreq);

        //Record 30
        fscanf (inputfile_fp, "%s %f %s %f %s %f",      //read
                varname,        //dummy
                &rainconvunits, //units conversion factor for rainfall grids
                varname,        //dummy
                &rainconvtime,  //time conversion factor for rainfall grids
                varname,        //dummy
                &rainscale);    //scale factor for rainfall grids

        //Echo converson and scale factors to file
        fprintf (echofile_fp, "Rainfall Grid Units Conversion Factor = %f\n",
                 rainconvunits);
        fprintf (echofile_fp, "Rainfall Grid Time Conversion Factor = %f\n",
                 rainconvtime);
        fprintf (echofile_fp, "Rainfall Grid Scale Factor = %f\n\n",
                 rainscale);

        //Record 31 (Part 1)
        fscanf (inputfile_fp, "%s",     //read
                varname);       //dummy

        //Allocate memory for rainfall grid root file name
        raingridfileroot = (char *) malloc (MAXNAMESIZE * sizeof (char));       //allocate memory

        //Record 31 (Part 2)
        fgets (raingridfileroot, MAXNAMESIZE, inputfile_fp);    //read name of rainfall grid root file (including any path)

        //strip leading blanks, trailing blanks, and final carriage return from string
        raingridfileroot = StripString (raingridfileroot);

        //echo rainfall grid root file name
        fprintf (echofile_fp, "\n\nRainfall Grid File Root Name:  %s\n",
                 raingridfileroot);

        //Note:  To suppress looping over rain gages, the number of
        //       rain gages in the simulation must be explicitly set
        //       to zero.
        //
        //set the number of rain gages to zero
        nrg = 0;

    }                           //end if rainopt <= 2

    //Record 32
    fscanf (inputfile_fp, "%s %d",      //read
            varname,            //dummy
            &nqwov);            //Number of External Flows

    //Echo Number of External Overland Flows to file
    fprintf (echofile_fp, "\n\nNumber of External Overland Flows = %d\n\n",
             nqwov);

    //if external overland flows are specified (nqwov > 0)
    if (nqwov > 0)
    {
        //Record 33
        fscanf (inputfile_fp, "%s %f %s %f %s %f",      //read
                varname,        //dummy
                &convunits,     //units conversion factor
                varname,        //dummy
                &convtime,      //time conversion factor
                varname,        //dummy
                &scale);        //scale factor

        //Echo converson and scale factors to file
        fprintf (echofile_fp, "Units Conversion Factor = %f\n", convunits);
        fprintf (echofile_fp, "Time Conversion Factor = %f\n", convtime);
        fprintf (echofile_fp, "Scale Factor = %f\n\n", scale);

        //Allocate memory for external overland flow parameters

        //flow source row number
        qwovrow = (int *) malloc ((nqwov + 1) * sizeof (int));
        //flow source column number
        qwovcol = (int *) malloc ((nqwov + 1) * sizeof (int));
        //number of pairs
        nqwovpairs = (int *) malloc ((nqwov + 1) * sizeof (int));
        //flow description
        qwovdescription = (char **) malloc ((nqwov + 1) * sizeof (char *));
        //external overland flow (m3/s)
        qwov = (float **) malloc ((nqwov + 1) * sizeof (float *));
        //time break in flow function (days)
        qwovtime = (float **) malloc ((nqwov + 1) * sizeof (float *));

        //Allocate remaining memory for overland flow parameters
        for (i = 1; i <= nqwov; i++)
        {
            //flow description
            qwovdescription[i] =
                (char *) malloc (MAXNAMESIZE * sizeof (char));
        }

        //Loop over number of overland flow functions
        for (i = 1; i <= nqwov; i++)
        {
            //Record 34 (Part 1)
            fscanf (inputfile_fp, "%d %d %d",   //read
                    &qwovrow[i],        //flow source row number
                    &qwovcol[i],        //flow source column number
                    &nqwovpairs[i]);    //number of pairs in time series

            //Record 34 (Part 2)
            fgets (qwovdescription[i], MAXNAMESIZE, inputfile_fp);      //Read Flow Description

            //Echo Overland Flow Source ID, Number Of Pairs, and Description to file
            fprintf (echofile_fp, "\nOverland Flow Source ID = %d", i);
            fprintf (echofile_fp, "\nRow Number = %d", qwovrow[i]);
            fprintf (echofile_fp, "\nColumn Number = %d", qwovcol[i]);
            fprintf (echofile_fp, "\nNumber Of Time Breaks = %d",
                     nqwovpairs[i]);
            fprintf (echofile_fp, "\nDescription: %s\n\n",
                     qwovdescription[i]);

            //Allocate remaining memory for flow parameters
            qwov[i] = (float *) malloc ((nqwovpairs[i] + 1) * sizeof (float));  //external flow (m3/s)
            qwovtime[i] = (float *) malloc ((nqwovpairs[i] + 1) * sizeof (float));      //time break (hrs)

            //Write label for external flow time series to file
            fprintf (echofile_fp, "  Flow (m3/s)     ");
            fprintf (echofile_fp, "  Time (hours)  \n");
            fprintf (echofile_fp, "~~~~~~~~~~~~~~~   ");
            fprintf (echofile_fp, "~~~~~~~~~~~~~~~~\n");

            //Loop over the number of pairs for this series
            for (j = 1; j <= nqwovpairs[i]; j++)
            {
                //Record 35
                fscanf (inputfile_fp, "%f %f",  //read
                        &qwov[i][j],    //External Overland Flow (m3/s)
                        &qwovtime[i][j]);       //time break

                //Apply conversion and scale factors
                qwov[i][j] = qwov[i][j] * convunits * convtime * scale;

                //Echo external flow and time break to file
                fprintf (echofile_fp, "%15.6f %18.3f\n", qwov[i][j],
                         qwovtime[i][j]);

            }                   //end loop over number of pairs in time series

        }                       //end loop over number of flow functions

    }                           //end if nqwov > 0

    //Check value of chnopt (channel option)
    if (chnopt == 1)
    {
        //Record 36
        fscanf (inputfile_fp, "%s %d",  //read
                varname,        //dummy
                &nqwch);        //Number of External Flows

        //Echo Number of External Channel Flows to file
        fprintf (echofile_fp, "\n\nNumber of External Flows = %d\n\n", nqwch);

        //if external flows are specified (nqwch > 0)
        if (nqwch > 0)
        {
            //Record 37
            fscanf (inputfile_fp, "%s %f %s %f %s %f",  //read
                    varname,    //dummy
                    &convunits, //units conversion factor
                    varname,    //dummy
                    &convtime,  //time conversion factor
                    varname,    //dummy
                    &scale);    //scale factor

            //Echo converson and scale factors to file
            fprintf (echofile_fp, "Units Conversion Factor = %f\n",
                     convunits);
            fprintf (echofile_fp, "Time Conversion Factor = %f\n", convtime);
            fprintf (echofile_fp, "Scale Factor = %f\n\n", scale);

            //Allocate memory for external channel flow parameters

            //flow source link number
            qwchlink = (int *) malloc ((nqwch + 1) * sizeof (int));
            //flow source node number
            qwchnode = (int *) malloc ((nqwch + 1) * sizeof (int));
            //number of pairs
            nqwchpairs = (int *) malloc ((nqwch + 1) * sizeof (int));
            //flow description
            qwchdescription =
                (char **) malloc ((nqwch + 1) * sizeof (char *));
            //external channel flow (m3/s)
            qwch = (float **) malloc ((nqwch + 1) * sizeof (float *));
            //time break in flow function (days)
            qwchtime = (float **) malloc ((nqwch + 1) * sizeof (float *));

            //Allocate remaining memory for flow description
            for (i = 1; i <= nqwch; i++)
            {
                qwchdescription[i] =
                    (char *) malloc (MAXNAMESIZE * sizeof (char));
            }

            //Loop over number of flow functions
            for (i = 1; i <= nqwch; i++)
            {
                //Record 38 (Part 1)
                fscanf (inputfile_fp, "%d %d %d",       //read
                        &qwchlink[i],   //flow source link number
                        &qwchnode[i],   //flow source node number
                        &nqwchpairs[i]);        //number of pairs in time series

                //Record 38 (Part 2)
                fgets (qwchdescription[i], MAXNAMESIZE, inputfile_fp);  //Read Flow Description

                //Echo Flow Source ID, Number Of Pairs, and Description  to file
                fprintf (echofile_fp, "\nFlow Source ID = %d", i);
                fprintf (echofile_fp, "\nLink Number = %d", qwchlink[i]);
                fprintf (echofile_fp, "\nNode Number = %d", qwchnode[i]);
                fprintf (echofile_fp, "\nNumber Of Time Breaks = %d",
                         nqwchpairs[i]);
                fprintf (echofile_fp, "\nDescription: %s\n\n",
                         qwchdescription[i]);

                //Allocate remaining memory for flow parameters

                //external channel flow (m3/s)
                qwch[i] =
                    (float *) malloc ((nqwchpairs[i] + 1) * sizeof (float));
                //time break (hrs)
                qwchtime[i] =
                    (float *) malloc ((nqwchpairs[i] + 1) * sizeof (float));

                //Write label for external channel flow time series to file
                fprintf (echofile_fp, "  Flow (m3/s)     ");
                fprintf (echofile_fp, "  Time (hours)  \n");
                fprintf (echofile_fp, "~~~~~~~~~~~~~~~   ");
                fprintf (echofile_fp, "~~~~~~~~~~~~~~~~\n");

                //Loop over the number of pairs for this series
                for (j = 1; j <= nqwchpairs[i]; j++)
                {
                    //Record 39
                    fscanf (inputfile_fp, "%f %f",      //read
                            &qwch[i][j],        //external channel flow (m3/s)
                            &qwchtime[i][j]);   //time break

                    //Apply conversion and scale factors
                    qwch[i][j] = qwch[i][j] * convunits * convtime * scale;

                    //Echo external flow and time break to file
                    fprintf (echofile_fp, "%15.6f %18.3f\n", qwch[i][j],
                             qwchtime[i][j]);

                }               //end loop over number of pairs in time series

            }                   //end loop over number of flow functions

        }                       //end if nqwch > 0

    }                           //end if chnopt = 1

    //Write label for outlets (boundaries) to file
    fprintf (echofile_fp,
             "\n\n\n  Domain Outlet/Boundary Characteristics  \n");
    fprintf (echofile_fp, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    //Record 40
    fscanf (inputfile_fp, "%s %d",      //read
            varname,            //dummy
            &noutlets);         //Number of Outlet Cells

    //Echo number of outlets to file
    fprintf (echofile_fp, "\nNumber of Domain Outlets: %d\n", noutlets);
    fprintf (echofile_fp,
             "\n\nOutlet     Row   Column  Overland Slope  dbcopt");
    fprintf (echofile_fp,
             "\n------    -----  ------  --------------  ------\n");

    //Allocate memory for outlet cell properties

    iout = (int *) malloc ((noutlets + 1) * sizeof (int));      //outlet cell row
    jout = (int *) malloc ((noutlets + 1) * sizeof (int));      //outlet cell column
    sovout = (float *) malloc ((noutlets + 1) * sizeof (float));        //outlet cell ground slope
    dbcopt = (int *) malloc ((noutlets + 1) * sizeof (int));    //domain boundary condition option

    //if channels are simulated (chnopt = 1)
    if (chnopt == 1)
    {
        //Allocate memory for channel boundary properties
        qchoutlet = (int *) malloc ((nlinks + 1) * sizeof (int));       //outlet for channel link

        //Initialize the channel link outlet vector...
        //
        //loop over links
        for (i = 1; i <= nlinks; i++)
        {
            //initialize the outlet value to zero
            qchoutlet[i] = 0;

        }                       //end loop over links

    }                           //end if chnopt = 1

    //Loop over number of outlets
    for (i = 1; i <= noutlets; i++)
    {
        //Record 41
        fscanf (inputfile_fp, "%s %d %d %f %d", //read
                varname,        //dummy
                &iout[i],       //Outlet cell row location
                &jout[i],       //Outlet cell column location
                &sovout[i],     //Outlet cell overland slope
                &dbcopt[i]);    //Domain boundary condition option

        //Echo Outlet Cell ROW, COLUMN and SLOPE to file
        fprintf (echofile_fp, "%6d  %5d  %6d  %14.6f  %6d",
                 i, iout[i], jout[i], sovout[i], dbcopt[i]);

        //The link number of the outlet is:
        //
        //  outlink = link[iout[i]][jout[i]];
        //
        //if channels are simulated (chnopt > 0) and the
        //outlet cell is a channel cell (imask > 1) and the
        //node is the last node of the link (node = nnodes)
        if (chnopt > 0 && imask[iout[i]][jout[i]] > 1
            && node[iout[i]][jout[i]] == nnodes[link[iout[i]][jout[i]]])
        {
            //store the outlet number corresponding to this link
            qchoutlet[link[iout[i]][jout[i]]] = i;

        }                       //end if chnopt > 0 and imask > 1...

        //if the domain boundary condition option = 1
        if (dbcopt[i] == 1)
        {
            //Record 42
            fscanf (inputfile_fp, "%s %f %s %f %s %f",  //read
                    varname,    //dummy
                    &convunits, //units conversion factor
                    varname,    //dummy
                    &convtime,  //time conversion factor
                    varname,    //dummy
                    &scale);    //scale factor

            //Echo converson and scale factors to file
            fprintf (echofile_fp, "\nUnits Conversion Factor = %f\n",
                     convunits);
            fprintf (echofile_fp, "Time Conversion Factor = %f\n", convtime);
            fprintf (echofile_fp, "Scale Factor = %f\n\n", scale);

            //Allocate memory for outlet boundary condition parameters
            //number of pairs
            nhbcpairs = (int *) malloc ((noutlets + 1) * sizeof (int));
            //bc description
            hbcdescription =
                (char **) malloc ((noutlets + 1) * sizeof (char *));
            //bc value (water depth) (m)
            hbc = (float **) malloc ((noutlets + 1) * sizeof (float *));
            //time break in bc function (days)
            hbctime = (float **) malloc ((noutlets + 1) * sizeof (float *));

            //Allocate remaining memory for overland bc parameters
            for (i = 1; i <= noutlets; i++)
            {
                hbcdescription[i] = (char *) malloc (MAXNAMESIZE * sizeof (char));      //bc description
            }

            //Record 43 (Part 1)
            fscanf (inputfile_fp, "%d", //read
                    &nhbcpairs[i]);     //number of pairs in time series

            //Record 43 (Part 2)
            fgets (hbcdescription[i], MAXNAMESIZE, inputfile_fp);       //Reads bc Description

            //Echo domain bc characteristics and description to file
            fprintf (echofile_fp, "\nDomain Boundary = %d", i);
            fprintf (echofile_fp, "\nRow Number = %d", iout[i]);
            fprintf (echofile_fp, "\nColumn Number = %d", jout[i]);
            fprintf (echofile_fp, "\nNumber Of Time Breaks = %d",
                     nhbcpairs[i]);
            fprintf (echofile_fp, "\nDescription: %s\n\n", hbcdescription[i]);

            //Allocate remaining memory for bc parameters
            hbc[i] = (float *) malloc ((nhbcpairs[i] + 1) * sizeof (float));    //bc (water depth) (m)
            hbctime[i] = (float *) malloc ((nhbcpairs[i] + 1) * sizeof (float));        //time break (hrs)

            //Write label for domain bc time series to file
            fprintf (echofile_fp, "  Depth (m)     ");
            fprintf (echofile_fp, "  Time (hours)  \n");
            fprintf (echofile_fp, "~~~~~~~~~~~~~   ");
            fprintf (echofile_fp, "~~~~~~~~~~~~~~~~\n");

            //Loop over the number of pairs for this series
            for (j = 1; j <= nhbcpairs[i]; j++)
            {
                //Record 44
                fscanf (inputfile_fp, "%f %f",  //read
                        &hbc[i][j],     //domain BC (m)
                        &hbctime[i][j]);        //time break (hrs)

                //Apply conversion and scale factors
                hbc[i][j] = hbc[i][j] * convunits * convtime * scale;

                //Echo bc and time break to file
                fprintf (echofile_fp, "%15.6f %18.3f\n", hbc[i][j],
                         hbctime[i][j]);

            }                   //end loop over number of pairs in time series

        }                       //end if dbcopt[i] = 1

    }                           //end loop over outlets

    //Record 45
    fscanf (inputfile_fp, "%s %d",      //read
            varname,            //dummy
            &nqreports);        //Number of Flow Reporting Stations

    //Echo Number of Flow Reporting Stations to file
    fprintf (echofile_fp, "\n\n\nNumber of Flow Reporting Stations = %d\n",
             nqreports);

    //Allocate memory for flow reporting stations
    qreprow = (int *) malloc ((nqreports + 1) * sizeof (int));  //reporting station cell row
    qrepcol = (int *) malloc ((nqreports + 1) * sizeof (int));  //reporting station cell column
    qarea = (float *) malloc ((nqreports + 1) * sizeof (float));        //reporting station cell drainage area
    qunitsopt = (int *) malloc ((nqreports + 1) * sizeof (int));        //flow report units option

    //Write label for flow reports to file
    fprintf (echofile_fp, "\n  Station     ");
    fprintf (echofile_fp, "  Cell Row     ");
    fprintf (echofile_fp, "  Cell Column     ");
    fprintf (echofile_fp, "  Drainage Area (km2)     ");
    fprintf (echofile_fp, "  Report Units Option  ");
    fprintf (echofile_fp, "  Station Name  \n");
    fprintf (echofile_fp, "~~~~~~~~~~~   ");
    fprintf (echofile_fp, "~~~~~~~~~~~~   ");
    fprintf (echofile_fp, "~~~~~~~~~~~~~~~   ");
    fprintf (echofile_fp, "~~~~~~~~~~~~~~~~~~~~~~   ");
    fprintf (echofile_fp, "~~~~~~~~~~~~~~~~~~~~~~~  ");
    fprintf (echofile_fp, "~~~~~~~~~~~~~~~~\n");

    //loop over number of flow reporting stations
    for (i = 1; i <= nqreports; i++)
    {
        //Record 46
        fscanf (inputfile_fp, "%d %d %f %d %s", //read
                &qreprow[i],    //reporting station cell row
                &qrepcol[i],    //reporting station cell column
                &qarea[i],      //reporting station cell (upstream) drainage Area
                &qunitsopt[i],  //flow report units option
                varname);       //station identifier

        //Echo Station (loop counter), Row, Column, Area, and Units Option to file
        fprintf (echofile_fp, "%11d   %12d   %15d   %22.3f   %23d %s\n",
                 i, qreprow[i], qrepcol[i], qarea[i], qunitsopt[i], varname);

    }                           //end loop over number of reports

    //Close the Echo File
    fclose (echofile_fp);

//End of function: Return to ReadInputFile
}
