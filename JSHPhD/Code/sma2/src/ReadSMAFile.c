/*---------------------------------------------------------------------
C-  Function:   ReadSMAFile
C-  
C-  Purpose/    Reads the SMA file that defines
C-  Methods:    the parameters for the Soil Moisture Accounting Procedure
C-
C-  Inputs:     None
C-
C-  Outputs:    None
C-
C-  Controls:   infopt
C-
C-  Calls:      imask
C-
C-  Called by:  ReadDataGroupB
C-
C-  Created:    James Halgren
C-
C-  Date:       20 JAN 2009
C-
C-  Revised:  
C-
C----------------------------------------------------------------------*/

//trex global variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for water transport
#include "trex_water_declarations.h"

//variable declarations and definitions specific to the 
//soil moisture accounting procedure.
#include "trex_SMA_declarations.h"

void ReadSMAFile(void)
{
    //Local Variable Declarations
    //int nuz, nlz, nio;          //number of upper and lower zones, interflow outlets.

    //write message to screen
    printf ("\n\n**************************\n");
    printf ("*                        *\n");
    printf ("*    Reading SMA File    *\n");
    printf ("*                        *\n");
    printf ("**************************\n\n\n");

    //Set the SMA file pointer to null
    SMAfile_fp = NULL;

    //Open the SMA file for reading
    SMAfile_fp = fopen (SMAfile, "r");

    //if the SMA file pointer is null, abort execution
    if (SMAfile_fp == NULL)
    {
        printf ("Can't open SMA File: %s\n", SMAfile);  //Write message to screen
        exit (EXIT_FAILURE);    //abort
    }

    //Write label for SMA file to file
    fprintf (echofile_fp,
             "\n\n\n  SMA File: Soil Moisture Accounting for infopt == 2\n");
    fprintf (echofile_fp, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    //Record 1 Zone Numbers
    fgets (header, MAXHEADERSIZE, SMAfile_fp);  //read header
    //Echo header to file
    fprintf (echofile_fp, "\n%s\n", header);
    // Read NUZ
    fscanf (SMAfile_fp, "%s %d", varname,       //dummy
            &nuz);              //number of upper zones
    // Read NLZ
    fscanf (SMAfile_fp, "%s %d", varname,       //dummy
            &nlz);              //number of lower zones
    // Read NIO
    fscanf (SMAfile_fp, "%s %d", varname,       //dummy
            &nio);              //number of interflow outlets

    //Record 2 Interflow Outlets
    fgets (header, MAXHEADERSIZE, SMAfile_fp);  //read interflow outlet header
    fgets (header, MAXHEADERSIZE, SMAfile_fp);  //read lower zone header

    //Echo interflow outlet header to file
    fprintf (echofile_fp, "\n%s\n", header);

    //Read interflow outlet links and nodes
    ioSMA =
        (interflowoutlet_t **) malloc ((nio + 1) *
                                       sizeof (interflowoutlet_t *));
    for (m = 1; m <= nio; m++)
    {
        ioSMA[m] =
            (interflowoutlet_t *) malloc ((m) * sizeof (interflowoutlet_t));
        fscanf (SMAfile_fp, "%d %d %d", varname, &(ioSMA[m]->intfout_i),
                &(ioSMA[m]->intfout_j));
    }

    //Read lower zone initial values
    lzSMA = (lowerzone_t **) malloc ((nlz + 1) * sizeof (lowerzone_t *));
    for (k = 1; k <= nlz; k++)
    {
        //Record 3 Lower Zones
        fgets (header, MAXHEADERSIZE, SMAfile_fp);      //read lower zone header
        fgets (header, MAXHEADERSIZE, SMAfile_fp);      //read lower zone header

        //Echo lower zone header to file
        fprintf (echofile_fp, "\n%s\n", header);

        lzSMA[k] = (lowerzone_t *) malloc (sizeof (lowerzone_t));
        fscanf (SMAfile_fp, "%d %lf %lf %lf %lf %lf %lf %lf %d",
                varname,
                &(lzSMA[k]->twc),
                &(lzSMA[k]->twm),
                &(lzSMA[k]->pbase),
                &(lzSMA[k]->zperc),
                &(lzSMA[k]->rexp),
                &(lzSMA[k]->pfree), &(lzSMA[k]->saved), &(lzSMA[k]->nparts));
        lzSMA[k]->kpart =
            (double *) malloc ((lzSMA[k]->nparts + 1) * sizeof (double));
        lzSMA[k]->part =
            (lowerzonepart_t **) malloc ((lzSMA[k]->nparts + 1) *
                                         sizeof (lowerzonepart_t *));

        for (h = 1; h <= lzSMA[k]->nparts; h++)
        {
            //Record 4 Lower Zone Parts
            fgets (header, MAXHEADERSIZE, SMAfile_fp);  //read lower zone part header
            fgets (header, MAXHEADERSIZE, SMAfile_fp);  //read lower zone part header

            //Echo lower zone part header to file
            fprintf (echofile_fp, "\n%s\n", header);
            lzSMA[k]->part[h] =
                (lowerzonepart_t *) malloc (sizeof (lowerzonepart_t));
            fscanf (SMAfile_fp, "%d %d %lf %lf %lf %lf",
                    varname,
                    varname,
                    &(lzSMA[k]->kpart[h]),
                    &(lzSMA[k]->part[h]->fwc),
                    &(lzSMA[k]->part[h]->fwm), &(lzSMA[k]->part[h]->k));
            lzSMA[k]->part[h]->kbasf =
                (double *) malloc ((nio + 1) * sizeof (double));

            //Record 5 Lower Zone Part Baseflow
            fgets (header, MAXHEADERSIZE, SMAfile_fp);  //read lower zone part baseflow header
            fgets (header, MAXHEADERSIZE, SMAfile_fp);  //read lower zone part baseflow header

            //Echo lower zone part baseflow header to file
            fprintf (echofile_fp, "\n%s\n", header);

            for (m = 1; m <= nio; m++)
            {
                fscanf (SMAfile_fp, "%d %d %d %lf",
                        varname,
                        varname, varname, &(lzSMA[k]->part[h]->kbasf[m]));
            }
        }
    }

    //Read upper zone initial values
    uzSMA = (upperzone_t **) malloc ((nuz + 1) * sizeof (upperzone_t *));
    for (k = 1; k <= nuz; k++)
    {
        //Record 6 Upper Zones
        fgets (header, MAXHEADERSIZE, SMAfile_fp);      //read upper zone header
        fgets (header, MAXHEADERSIZE, SMAfile_fp);      //read upper zone header

        //Echo upper zone header to file
        fprintf (echofile_fp, "\n%s\n", header);

        uzSMA[k] = (upperzone_t *) malloc (sizeof (upperzone_t));
        fscanf (SMAfile_fp, "%d %lf %lf %d",
                varname,
                &(uzSMA[k]->twc), &(uzSMA[k]->twm), &(uzSMA[k]->nparts));
        uzSMA[k]->kpart =
            (double *) malloc ((uzSMA[k]->nparts + 1) * sizeof (double));
        uzSMA[k]->part =
            (upperzonepart_t **) malloc ((uzSMA[k]->nparts + 1) *
                                         sizeof (upperzonepart_t *));
        for (h = 1; h <= uzSMA[k]->nparts; h++)
        {
            //Record 7 Upper Zone Parts
            fgets (header, MAXHEADERSIZE, SMAfile_fp);  //read upper zone part header
            fgets (header, MAXHEADERSIZE, SMAfile_fp);  //read upper zone part header

            //Echo upper zone part header to file
            fprintf (echofile_fp, "\n%s\n", header);

            uzSMA[k]->part[h] =
                (upperzonepart_t *) malloc (sizeof (upperzonepart_t));
            fscanf (SMAfile_fp, "%d %d %lf %lf %lf %lf",
                    varname,
                    varname,
                    &(uzSMA[k]->kpart[h]), &(uzSMA[k]->part[h]->fwc),
                    &(uzSMA[k]->part[h]->fwm), &(uzSMA[k]->part[h]->k));
            uzSMA[k]->part[h]->kintf =
                (double *) malloc ((nio + 1) * sizeof (double));

            //Record 8 Upper Zone Part Interflow
            fgets (header, MAXHEADERSIZE, SMAfile_fp);  //read upper zone part interflow header
            fgets (header, MAXHEADERSIZE, SMAfile_fp);  //read upper zone part interflow header

            //Echo upper zone part interflow header to file
            fprintf (echofile_fp, "\n%s\n", header);

            for (m = 1; m <= nio; m++)
            {
                fscanf (SMAfile_fp, "%d %d %lf",
                        varname, varname, &(uzSMA[k]->part[h]->kintf[m]));
            }
        }

        //Record 9 Upper Zone Percolation Outlets
        fgets (header, MAXHEADERSIZE, SMAfile_fp);      //read upper zone percolation outlet header
        fgets (header, MAXHEADERSIZE, SMAfile_fp);      //read upper zone percolation outlet header

        //Echo upper zone percolation outlet header to file
        fprintf (echofile_fp, "\n%s\n", header);

        uzSMA[k]->kperc = (double *) malloc ((nlz + 1) * sizeof (double));
        for (m = 1; m <= nlz; m++)
        {
            fscanf (SMAfile_fp, "%d %d %lf",
                    varname, varname, &(uzSMA[k]->kperc[m]));
        }
    }

    //Read SMA upper zone distribution coefficient header
    fgets (header, MAXHEADERSIZE, SMAfile_fp);  //read upper zone header
    fgets (header, MAXHEADERSIZE, SMAfile_fp);  //read upper zone header
    //Echo SMA upper zone distribution coefficient to file
    fprintf (echofile_fp, "\n%s\n", header);

    for (k = 1; k <= nuz; k++)
    {
        //Record 10 SMA upper zone distribution coefficient
        fscanf (SMAfile_fp, "%s", varname);     //read dummy

        //Allocate memory for SMA upper zone distribution coefficient file name
        SMAUZKfile = (char *) malloc (MAXNAMESIZE * sizeof (char));

        //Record 10 (Part 2)
        fgets (SMAUZKfile, MAXNAMESIZE, SMAfile_fp);    //read SMA file name

        //strip leading blanks, trailing blanks, final carriage return from string
        SMAUZKfile = StripString (SMAUZKfile);

        //Echo SMA File name to file
        fprintf (echofile_fp, "\n\nSMA File: %s\n", SMAUZKfile);

        //ReadSMAUZKFile();
        //ReadSMAKUZFile(); // Grid of partition coefficients to determine 
        //which Upper zone receives flow from each cell
    }

    //Record 11 SMA States file name
    fscanf (SMAfile_fp, "%s", varname); //read dummy

    //Allocate memory for SMA states file name
    SMAstatefile = (char *) malloc (MAXNAMESIZE * sizeof (char));

    //Record 11 (Part 2)
    fgets (SMAstatefile, MAXNAMESIZE, SMAfile_fp);      //read SMA file name

    //strip leading blanks, trailing blanks, final carriage return from string
    SMAstatefile = StripString (SMAstatefile);

    //Echo SMA states File name to file
    fprintf (echofile_fp, "\n\nSMA File: %s\n", SMAstatefile);

    //Record 12 SMA printout interval
    fscanf (SMAfile_fp, "%s", varname); //read dummy

    //Record 12 (Part 2)
    fscanf (SMAfile_fp, "%lf", &printSMAinterval);      //read SMA printout interval

    //JSH ADD This code should read an array of printout intervals
    //JSH ADD but for now brings in a single value for use
    //JSH ADD during the entire simulation
    /*
       //Allocate memory for printout interval array
       SMAstatefile = (char *) malloc (MAXNAMESIZE * sizeof (char));
     */

    //JSH ADD error checking

    //JSH ADD Need to incorporate error checking 
    //JSH ADD and echo file output into the SMA read routine 
    //JSH ADD as stubbed out below.
    /*

       //If number of grid rows, grid columns, or cell size do not equal global values, abort...
       if(gridrows != nrows || gridcols != ncols || cellsize != dx)
       {
       //Write error message to file
       fprintf(echofile_fp, "\n\n\nSoil File Error:\n");
       fprintf(echofile_fp,       "  nrows = %5d   grid rows = %5d\n", nrows, gridrows);
       fprintf(echofile_fp,       "  ncols = %5d   grid cols = %5d\n", ncols, gridcols);
       fprintf(echofile_fp,       "  dx = %12.4f   dy = %12.4f   cell size = %12.4f\n", dx, dy, cellsize);

       //Write error message to screen
       printf("Soil File Error:\n");
       printf("  nrows = %5d   grid rows = %5d\n", nrows, gridrows);
       printf("  ncols = %5d   grid cols = %5d\n", ncols, gridcols);
       printf("  dx = %12.4f   dy = %12.4f   cell size = %12.4f\n", dx, dy, cellsize);

       exit(EXIT_FAILURE);                                  //abort
       }    //end check of grid dimensions


     */
    //Close SMA File
    fclose (SMAfile_fp);
//End of function: Return to ReadDataGroupB
}
