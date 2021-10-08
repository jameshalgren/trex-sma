/*---------------------------------------------------------------------
C-  Function:   ReadSedimentPropertiesFile.c
C-
C-  Purpose/    ReadSedimentProperiesFile is called to read the proprties
C-  Methods:    (thickness, bottom width, and grain size distribution
C-              (GSD) for each vertical layer in the sediment stack for 
C-              each node of each link in the channel network
C-
C-
C-  Inputs:     None
C-
C-  Outputs:    nstackch0[i][j]
C-              porositych[i][j][ilayer]
C-              hlayerch0[i][j][layer]
C-              bwlayerch0[i][j][layer]
C-
C-              aych[i][j]
C-              mexpch[i][j]
C-
C-              gsdch[solid][link][node][layer]
C-
C-  Controls:   erschopt, ctlopt
C-
C-  Calls:      None
C-
C-  Called by:  ReadDataGroupC
C-
C-	Created:	Mark Velleux
C-				Department of Civil Engineering
C-				Colorado State University
C-				Fort Collins, CO 80523
C-
C-	Date:		26-MAR-2004
C-
C-  Revised:
C-
C----------------------------------------------------------------------*/

//trex global variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for general controls and water transport
#include "trex_water_declarations.h"

//trex global variable declarations for sediment transport
#include "trex_solids_declarations.h"

void ReadSedimentPropertiesFile(void)
{
    //local variable declarations
    int isolid;                 //local loop index for number of solids

    int chanlinks,              //number of channel links (dummy)
      chansolids,               //number of solids types (dummy)
      chanersopt,               //channel erosion option (dummy)
      chanctlopt,               //channel transmission loss option (dummy)
      linknum,                  //link number (dummy)
      linknodes,                //number of nodes in a link (dummy)
      nodenum,                  //node number (dummy)
      layernum;                 //layer number (dummy)

    float gsdchtot;             //Channel grain size distribution total

    //write message to screen
    printf ("\n\n***************************************\n");
    printf ("*                                     *\n");
    printf ("*   Reading Sediment Properties File  *\n");
    printf ("*                                     *\n");
    printf ("***************************************\n\n\n");

    //Set the sediment properties file pointer to null
    sedimentpropertiesfile_fp = NULL;

    //Open the sediment properties file for reading
    sedimentpropertiesfile_fp = fopen (sedimentpropertiesfile, "r");

    //if the sediment properties file pointer is null, abort execution
    if (sedimentpropertiesfile_fp == NULL)
    {
        printf ("Can't open Channel File: %s \n", sedimentpropertiesfile);      //Write message to screen

        exit (EXIT_FAILURE);    //abort
    }

    //Allocate memory for global sediment properties
    //
    //Allocate initial memory for sediment grain size distribution (gsd)
    gsdch = (float ****) malloc ((nsolids + 1) * sizeof (float ***));   //GSD for sediments in channels

    //loop over number of solids
    for (isolid = 1; isolid <= nsolids; isolid++)
    {
        //Allocate additional memory
        gsdch[isolid] = (float ***) malloc ((nlinks + 1) * sizeof (float **));  //GSD for sediments in channels

        //loop over links
        for (i = 1; i <= nlinks; i++)
        {
            //Allocate additional memory
            gsdch[isolid][i] = (float **) malloc ((nnodes[i] + 1) * sizeof (float *));  //GSD for sediments in channels

            //loop over nodes
            for (j = 1; j <= nnodes[i]; j++)
            {
                //Allocate remaining memory
                gsdch[isolid][i][j] = (float *) malloc ((maxstackch + 1) * sizeof (float));     //GSD for sediments in channels

            }                   //end loop over nodes

        }                       //end loop over links

    }                           //end loop over solids

    //Allocate initial memory for sediment layer porosity, thickness, and width
    nstackch0 = (int **) malloc ((nlinks + 1) * sizeof (int *));        //initial number of layers in sediment stack
    porositych = (float ***) malloc ((nlinks + 1) * sizeof (float **)); //layer porosity (dimensionless)
    hlayerch0 = (float ***) malloc ((nlinks + 1) * sizeof (float **));  //initial layer thickness (m)
    bwlayerch0 = (float ***) malloc ((nlinks + 1) * sizeof (float **)); //initial layer bottom width (m)

    //if the channel erosion option > 1
    if (erschopt > 1)
    {
        //Allocate initial memory for sediment erosion properties
        aych = (float **) malloc ((nlinks + 1) * sizeof (float *));     //initial layer thickness (m)
        mexpch = (float **) malloc ((nlinks + 1) * sizeof (float *));   //initial layer bottom width (m)

    }                           //end if erschopt > 1

    //if the channel transmission loss option > 0
    if (ctlopt > 0)
    {
        //Allocate initial memory for channel transmsiion loss properties
        khsed = (float **) malloc ((nlinks + 1) * sizeof (float *));    //sediment hydraulic conductivity (m/s)
        capshsed = (float **) malloc ((nlinks + 1) * sizeof (float *)); //sediment capillary suction head (m)
        sedmd = (float **) malloc ((nlinks + 1) * sizeof (float *));    //sediment moisture deficit (dimensionless)

    }                           //end if ctlopt > 0

    //loop over links
    for (i = 1; i <= nlinks; i++)
    {
        //Allocate additional memory
        nstackch0[i] = (int *) malloc ((nnodes[i] + 1) * sizeof (int)); //initial number of layers in sediment stack
        porositych[i] = (float **) malloc ((nnodes[i] + 1) * sizeof (float *)); //layer porosity (dimensionless)
        hlayerch0[i] = (float **) malloc ((nnodes[i] + 1) * sizeof (float *));  //initial layer thickness (m)
        bwlayerch0[i] = (float **) malloc ((nnodes[i] + 1) * sizeof (float *)); //initial layer bottom width (m)

        //if the channel erosion option > 1
        if (erschopt > 1)
        {
            //Allocate additional memory for sediment erosion properties
            aych[i] = (float *) malloc ((nnodes[i] + 1) * sizeof (float));      //initial layer thickness (m)
            mexpch[i] = (float *) malloc ((nnodes[i] + 1) * sizeof (float));    //initial layer bottom width (m)

        }                       //end if erschopt > 1

        //if the channel transmission loss option > 0
        if (ctlopt > 0)
        {
            //Allocate initial memory for channel transmsiion loss properties
            khsed[i] = (float *) malloc ((nnodes[i] + 1) * sizeof (float));     //sediment hydraulic conductivity (m/s)
            capshsed[i] = (float *) malloc ((nnodes[i] + 1) * sizeof (float));  //sediment capillary suction head (m)
            sedmd[i] = (float *) malloc ((nnodes[i] + 1) * sizeof (float));     //sediment moisture deficit (dimensionless)

        }                       //end if ctlopt > 0

        //loop over nodes
        for (j = 1; j <= nnodes[i]; j++)
        {
            //Allocate remaining memory for sediment properties
            porositych[i][j] = (float *) malloc ((maxstackch + 1) * sizeof (float));    //layer porosity (dimensionless)
            hlayerch0[i][j] = (float *) malloc ((maxstackch + 1) * sizeof (float));     //initial layer thickness (m)
            bwlayerch0[i][j] = (float *) malloc ((maxstackch + 1) * sizeof (float));    //initial layer bottom width (m)

        }                       //end loop over nodes

    }                           //end loop over links

    //Write label for sediment properties file to file
    fprintf (echofile_fp, "\n\n\n  Sediment Properties File  \n");
    fprintf (echofile_fp, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    //Record 1
    fgets (header, MAXHEADERSIZE, sedimentpropertiesfile_fp);   //read header

    //Echo header to file
    fprintf (echofile_fp, "\n%s\n", header);

    //Record 2
    fscanf (sedimentpropertiesfile_fp, "%s %d %s %d %s %d %s %d",       //read
            varname,            //dummy
            &chanlinks,         //number of channel links (local)
            varname,            //dummy
            &chansolids,        //number of channel solids types (local)
            varname,            //dummy
            &chanersopt,        //channel erosion option (local)
            varname,            //dummy
            &chanctlopt);       //channel transmission loss option (local)

    //Error check: chanlinks must equal nlinks...
    //
    //If number of links does not equal the global value from the link file, abort...
    if (chanlinks != nlinks)
    {
        //Write error message to file
        fprintf (echofile_fp, "\n\n\nSediment Properties File Error:\n");
        fprintf (echofile_fp, "  chanlinks = %5d   nlinks = %5d\n", chanlinks,
                 nlinks);

        //Write error message to screen
        printf ("Sediment Properties File Error:\n");
        printf ("  chanlinks = %5d   nlinks = %5d\n", chanlinks, nlinks);

        exit (EXIT_FAILURE);    //abort

    }                           //end check of number of channel links

    //Error check: chansolids must equal nsolids...
    //
    //If number of solids does not equal the global value from the input file, abort...
    if (chansolids != nsolids)
    {
        //Write error message to file
        fprintf (echofile_fp, "\n\n\nSediment Properties File Error:\n");
        fprintf (echofile_fp, "  chansolids = %5d   nsolids = %5d\n",
                 chansolids, nsolids);

        //Write error message to screen
        printf ("Sediment Properties File Error:\n");
        printf ("  chansolids = %5d   nsolids = %5d\n", chansolids, nsolids);

        exit (EXIT_FAILURE);    //abort

    }                           //end check of number of channel solids

    //Error check: chanersopt must equal erschopt...
    //
    //If the channel erosion option does not equal the global value from the input file, abort...
    if (chanersopt != erschopt)
    {
        //Write error message to file
        fprintf (echofile_fp, "\n\n\nSediment Properties File Error:\n");
        fprintf (echofile_fp, "  chanersopt = %5d   erschopt = %5d\n",
                 chanersopt, erschopt);

        //Write error message to screen
        printf ("Sediment Properties File Error:\n");
        printf ("  chanersopt = %5d   erschopt = %5d\n", chanersopt,
                erschopt);

        exit (EXIT_FAILURE);    //abort

    }                           //end check of channel erosion option

    //Error check: chanctlopt must equal ctlopt...
    //
    //If the channel transmission loss option does not equal the global value from the input file, abort...
    if (chanctlopt != ctlopt)
    {
        //Write error message to file
        fprintf (echofile_fp, "\n\n\nSediment Properties File Error:\n");
        fprintf (echofile_fp, "  chanctlopt = %5d   ctlopt = %5d\n",
                 chanctlopt, ctlopt);

        //Write error message to screen
        printf ("Sediment Properties File Error:\n");
        printf ("  chanctlopt = %5d   ctlopt = %5d\n", chanctlopt, ctlopt);

        exit (EXIT_FAILURE);    //abort

    }                           //end check of channel transmission loss option

    //loop over links
    for (i = 1; i <= nlinks; i++)
    {
        //Record 3
        fscanf (sedimentpropertiesfile_fp, "%s %d %s %d",       //read
                varname,        //dummy
                &linknum,       //link number (local) (known from loop counter, read as dummy)
                varname,        //dummy
                &linknodes);    //number of nodes (local)

        //Two more error checks...
        //  1. data must be in link order (1 to nlinks); and
        //  2. channodes must equal nnodes for each link.
        //
        //The sediment properties  file data must be in
        //sequential order from 1 to nlinks.
        //
        //If link number read from file does not match the link index (i), abort...
        if (linknum != i)
        {
            //Write error message to file
            fprintf (echofile_fp, "\n\n\nSediment Properties File Error:\n");
            fprintf (echofile_fp, "  link read = %d   link expected = %d\n",
                     linknum, i);

            //Write error message to screen
            printf ("Sediment Properties File Error:\n");
            printf ("  link read = %d   link expected = %d\n", chanlinks, i);

            exit (EXIT_FAILURE);        //abort

        }                       //end check of number of channel nodes

        //If number of nodes does not equal the global value from the node file, abort...
        if (linknodes != nnodes[i])
        {
            //Write error message to file
            fprintf (echofile_fp, "\n\n\nSediment Properties File Error:\n");
            fprintf (echofile_fp,
                     "  link = %d   linknodes = %d   nnodes = %d\n", i,
                     linknodes, nnodes[i]);

            //Write error message to screen
            printf ("Sediment Properties File Error:\n");
            printf ("  link = %d   linknodes = %d   nnodes = %d\n",
                    i, linknodes, nnodes[i]);

            exit (EXIT_FAILURE);        //abort

        }                       //end check of number of channel nodes

        //Loop of number of nodes in current link
        for (j = 1; j <= nnodes[i]; j++)
        {
            //Write first part of label for sediment properties to file
            fprintf (echofile_fp, "\nLink  Node  NSTACK");

            //Write next part of label...
            //
            //if the channel erosion option > 1
            if (erschopt > 1)
            {
                //Write label for sediment properties to file
                fprintf (echofile_fp, "  aych (g/m2)  mexpch");

            }                   //end if erschopt > 1

            //if the channel transmission loss option > 0
            if (ctlopt > 0)
            {
                //Write label for tranmission loss properties to file
                fprintf (echofile_fp,
                         "  Khsed (m/s)  CapSHsed (m)  SedMD (-)");

            }                   //end if ctlopt > 0

            //Write last part of label...
            //
            fprintf (echofile_fp, "\n----  ----  ------");

            //if the channel erosion option > 1
            if (erschopt > 1)
            {
                fprintf (echofile_fp, "  -----------  ------");

            }                   //end if erschopt > 1

            //if the channel transmission loss option > 0
            if (ctlopt > 0)
            {
                fprintf (echofile_fp,
                         "  -----------  ------------  ---------");

            }                   //end if ctlopt > 0

            //Write concluding carriage return for label to file
            fprintf (echofile_fp, "\n");

            //Read option independent properties...
            //
            //Record 4a
            fscanf (sedimentpropertiesfile_fp, "%s %d %s %d",   //read
                    varname,    //dummy
                    &nodenum,   //node number (local) (read as dummy)
                    varname,    //dummy
                    &nstackch0[i][j]);  //number of sediment stack elements (global)

            //Echo properties
            fprintf (echofile_fp, "%4d  %4d  %5d", i, j, nstackch0[i][j]);

            //Read option dependent properties...
            //
            //if the channel erosion option > 1
            if (chanersopt > 1)
            {
                //Record 4b
                fscanf (sedimentpropertiesfile_fp, "%s %f %s %f",       //read
                        varname,        //dummy
                        &aych[i][j],    //sediment erosion yield (global)
                        varname,        //dummy
                        &mexpch[i][j]); //sediment erosion exponent (global)

                //Echo properties
                fprintf (echofile_fp, "  %11.3f  %6.3f",
                         aych[i][j], mexpch[i][j]);

            }                   //end if erschopt <= 1

            //if channel transmission losses are simulated
            if (chanctlopt > 0)
            {
                //Record 4c
                fscanf (sedimentpropertiesfile_fp, "%s %f %s %f %s %f", //read
                        varname,        //dummy
                        &khsed[i][j],   //sediment hydraulic conductivity (m/s) (global)
                        varname,        //dummy
                        &capshsed[i][j],        //sediment capilary suction head (m) (global)
                        varname,        //dummy
                        &sedmd[i][j]);  //sediment moisture deficit (dimensionless) (global)

                //Echo properties
                fprintf (echofile_fp, "  %11.3e  %12.3e  %9.4f",
                         khsed[i][j], capshsed[i][j], sedmd[i][j]);

            }                   //end if chanctlopt > 0

            //Write concluding carriage return for data echo to file
            fprintf (echofile_fp, "\n");

            //loop over number of layers in reverse order
            for (k = nstackch0[i][j]; k >= 1; k--)
            {
                //Write label to file
                fprintf (echofile_fp,
                         "\n  Layer  Thickness (m)  Width (m)  Porosity");
                fprintf (echofile_fp,
                         "\n  -----  -------------  ---------  --------\n");

                //Record 5
                fscanf (sedimentpropertiesfile_fp, "%s %d %s %f %s %f %s %f",   //read
                        varname,        //dummy
                        &layernum,      //layer number (local) (read as dummy)
                        varname,        //dummy
                        &hlayerch0[i][j][k],    //initial sediment layer thickness (m)
                        varname,        //dummy
                        &bwlayerch0[i][j][k],   //initial sediment layer bottom width (m)
                        varname,        //dummy
                        &porositych[i][j][k]);  //sediment layer porosity (dimensionless)

                //Echo sediment properties to file
                fprintf (echofile_fp, "  %5d  %13.4f  %9.3f  %8.4f\n",
                         k, hlayerch0[i][j][k], bwlayerch0[i][j][k],
                         porositych[i][j][k]);

                //Write label to file
                fprintf (echofile_fp, "\n  Grain Size Distribution:\n");
                fprintf (echofile_fp, "\n  Solid  GSD Fraction");
                fprintf (echofile_fp, "\n  -----  ------------");

                //Record 6a
                fscanf (sedimentpropertiesfile_fp, "%s",        //read
                        varname);       //dummy

                //Sum of grain size distribution components (channels)
                gsdchtot = 0.0;

                //Loop of number of solids (particle types)
                for (isolid = 1; isolid <= nsolids; isolid++)
                {
                    //Record 6b
                    fscanf (sedimentpropertiesfile_fp, "%f",    //read
                            &gsdch[isolid][i][j][k]);   //grain size distribution

                    //Echo sediment layer gsd to file
                    fprintf (echofile_fp, "\n  %5d  %12.7f",
                             isolid, gsdch[isolid][i][j][k]);

                    //Compute total (sum) of individual grain size distribution components
                    gsdchtot = gsdchtot + gsdch[isolid][i][j][k];

                }               //end loop over number of solids

                //Echo sum of grain size distribution to file (blank space for formatting)
                fprintf (echofile_fp, "\n  Total  %12.7f\n\n", gsdchtot);

                //Error Checking
                if ((gsdchtot < 0.99999) || (gsdchtot > 1.00001))
                {
                    //Write error message to file
                    fprintf (echofile_fp,
                             "Error:  grain size distribution does not sum to 1.0!\n");

                    exit (EXIT_FAILURE);        //exit the program

                }               //end check of gsdchtot

            }                   //end loop over number of layers

        }                       //end loop over number of nodes

        //Start a new line for the next row of data in the echo file 
        fprintf (echofile_fp, "\n");

    }                           //end loop over number of links

    //Close Sediment Properties File
    fclose (sedimentpropertiesfile_fp);

//End of Function: Return to ReadDataGroupC
}
