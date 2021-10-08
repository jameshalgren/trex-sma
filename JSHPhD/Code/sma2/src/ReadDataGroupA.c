/*---------------------------------------------------------------------
C-  Function:	ReadDataGroupA.c
C-
C-  Purpose/    ReadDataGroupA is called at the start of the simulation
C-  Methods:    to read Data Group A (general controls) from the model
C-              input file.
C-
C-
C-  Inputs:     None
C-
C-  Outputs:    None
C-
C-  Controls:   None
C-
C-  Calls:      None
C-
C-  Called by:  ReadInputFile
C-
C-  Created:    Mark Velleux
C-              Aditya Maroo
C-
C-  Date:       15-JUL-2003
C-
C-  Revised:
C-
C----------------------------------------------------------------------*/

//trex general variable declarations
#include "trex_general_declarations.h"

void ReadDataGroupA(void)
{
    //write message to screen
    printf ("\n\n***************************\n");
    printf ("*                         *\n");
    printf ("*   Reading Data Group A  *\n");
    printf ("*                         *\n");
    printf ("***************************\n\n\n");

    //Record 1
    fgets (header1, MAXHEADERSIZE, inputfile_fp);       //read header1 from the input file

    //Record 2
    fgets (header2, MAXHEADERSIZE, inputfile_fp);       //read header2 from the input file

    //Record 3
    fscanf (inputfile_fp, "%s %d %s %d %s %d %s %f %s %f	%s %lf",        //read
            varname,            //dummy 
            &ksim,              //simulation type
            varname,            //dummy
            &nrows,             //number of rows
            varname,            //dummy
            &ncols,             //number of columns
            varname,            //dummy
            &dx,                //grid cell length in east-west direction
            varname,            //dummy
            &dy,                //grid cell length in north-south direction
            varname,            //dummy
            &tstart);           //simulation start time

    /* JSH Comment: 2009-17-12 16:24:20 GMT-0700 */
    //Record 3.5
    fscanf (inputfile_fp, "%s %d %d %d %d %d %d %d %d",
            varname,            //dummy for TZero label
            &startyear,
            &startmonth,
            &startday,
            &starthour,
            &startminute, &startsecond, &gmt_offset, &daylightsavingstoggle);

    dateshift (displaydate, 0, startyear, startmonth, startday, starthour,
               startminute, startsecond, gmt_offset, daylightsavingstoggle);

    fprintf (stderr, "Simulation Starting Date has been read as %s",
             displaydate);
    /* JSH Comment: 2009-17-12 11:36:56 GMT-0700 */

    //Record 4
    fscanf (inputfile_fp, "%s %d",      //read
            varname,            //dummy
            &ndt);              //number of timesteps

    //Allocate memory for numerical integration time steps
    dt = (float *) malloc ((ndt + 2) * sizeof (float)); //allocate memory
    dttime = (float *) malloc ((ndt + 2) * sizeof (float));     //allocate memory

    for (i = 1; i <= ndt; i++)  //loop over number of time steps
    {
        //Record 5
        fscanf (inputfile_fp, "%f %f",  //read
                &dt[i],         //timestep value (seconds)
                &dttime[i]);    //time break for timestep (hours)
    }                           //end loop over timestep series

    //Record 6
    fscanf (inputfile_fp, "%s %d", varname, &nprintout);        //read number of output print intervals

    //Allocate memory for output print intervals
    printout = (float *) malloc ((nprintout + 1) * sizeof (float));     //allocate memory
    printouttime = (float *) malloc ((nprintout + 1) * sizeof (float)); //allocate memory

    //Loop over number of output print intervals
    for (i = 1; i <= nprintout; i++)
    {
        //Record 7
        fscanf (inputfile_fp, "%f %f",  //read
                &printout[i],   //output print interval
                &printouttime[i]);      //time break for output print interval
    }                           //end loop over print interval series

    //Record 8
    fscanf (inputfile_fp, "%s %d", varname, &nprintgrid);       //read number of grid print intervals

    //Allocate memory for grid print intervals
    printgrid = (float *) malloc ((nprintgrid + 1) * sizeof (float));   //allocate memory
    printgridtime = (float *) malloc ((nprintgrid + 1) * sizeof (float));       //allocate memory

    //Loop over number of grid print intervals
    for (i = 1; i <= nprintgrid; i++)
    {
        //Record 9
        fscanf (inputfile_fp, "%f %f",  //read
                &printgrid[i],  //grid print interval
                &printgridtime[i]);     //time break for grid print interval
    }                           //end loop over print interval series

/*  JSH ADD There should be a simple flag to toggle whether or not the input
	JSH ADD files require the additional header on the ASCII Grid. 
	JSH ADD OR, there could be some options here about using a Geodatabase as
	JSH ADD the input source.
	//Record 9.5 
	fscanf(inputfile_fp, "%s %d",	//read
			varname,				//dummy
			&inputgridheaderopt);	//Toggle reading of extra line on top of ASCII input grids

*/

    //Record 10 (Part 1)
    fscanf (inputfile_fp, "%s", //read
            varname);           //dummy

    //Allocate memory for echo file name
    echofile = (char *) malloc (MAXNAMESIZE * sizeof (char));   //allocate memory

    //Record 10 (Part 2)
    fgets (echofile, MAXNAMESIZE, inputfile_fp);        //read name of echo file (including any path)

    //strip leading blanks, trailing blanks, and final carriage return from string
    echofile = StripString (echofile);

    //set the echo file pointer to null
    echofile_fp = NULL;

    //open the echo file
    echofile_fp = fopen (echofile, "w");

    //if file pointer is still null, then abort
    if (echofile_fp == NULL)
    {
        printf ("Error! Can't open Echo file %s\n", echofile);  //write error message to screen
        exit (EXIT_FAILURE);    //exit with error
    }

    //write welcome banner to echo file
    fprintf (echofile_fp, "\n\n   Welcome to TREX");
    fprintf (echofile_fp, "\n~~~~~~~~~~~~~~~~~~~~~\n\n\n\n");

    //echo input file name
    fprintf (echofile_fp, "\n\nInput File Name:  %s\n\n\n", inputfile);

    //echo header lines to file
    fprintf (echofile_fp, "%s%s\n\n", header1, header2);

    //Echo Simulation Type, Number of Rows and Number of Columns to file
    fprintf (echofile_fp, "\nSimulation Type = %d\n", ksim);
    fprintf (echofile_fp, "\nSize of Spatial Domain:\n");
    fprintf (echofile_fp, "   Number of Rows = %d\n", nrows);
    fprintf (echofile_fp, "   Number of Columns = %d\n", ncols);
    fprintf (echofile_fp, "\nGrid Cell Size:\n");
    fprintf (echofile_fp, "   dx (east-west) = %f (m)\n", dx);
    fprintf (echofile_fp, "   dy (north-south) = %f (m)\n\n", dy);

    //Developer's Note:
    //Use of different values for dx and dy is for future development
    //and is not currently supported.  At present, the code assumes
    //that all grid cells are square so dx must equal dy.  The error
    //check below will need to be deleted if code development to
    //support different dx and dy values is implemented...  mlv 7/03
    if (dy != dx)               //delete when the code supports different dx and dy values...
    {
        //write error message to screen
        printf
            ("Error! Can't have different values for dx and dy at present");

        //write error message to file to file
        fprintf (echofile_fp, "\nGrid must be square (dy = dx):\n");
        fprintf (echofile_fp, "   dx (east-west) = %f (m)\n", dx);
        fprintf (echofile_fp, "   dy (north-south) = %f (m)\n\n", dy);

        exit (EXIT_FAILURE);    //exit with error
    }
    else                        //dx = dy, so set cell size w = dx (= dy)
    {
        //set cell size "w"
        w = dx;
    }                           //end if(dy != dx)

    //Echo simulation start and end times to file
    fprintf (echofile_fp, "\nSimulation start and end times:");
    fprintf (echofile_fp, "\n   Start = %f (hours)", tstart);
    fprintf (echofile_fp, "\n   End = %f (hours)\n\n", dttime[ndt]);

    //Echo number of time steps to file
    fprintf (echofile_fp, "\nNumber of Time Steps = %d\n", ndt);

    //Print label for time step series to file
    fprintf (echofile_fp, "\n    dt          time   ");
    fprintf (echofile_fp, "\n~~~~~~~~~~   ~~~~~~~~~~\n");

    //Loop over number of time steps
    for (i = 1; i <= ndt; i++)
    {
        //Echo dt, dttime series to file
        fprintf (echofile_fp, "%10.4f   %10.4f\n", dt[i], dttime[i]);
    }

    //Echo number of output print intervals to file
    fprintf (echofile_fp,
             "\n\nNumber of Print Intervals for Tabular Output = %d\n",
             nprintout);

    //Echo label for output print intervals to file
    fprintf (echofile_fp, "\n Interval (hours)    Time (hours) ");
    fprintf (echofile_fp, "\n~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~\n");

    //Echo output print interval and time breaks to file
    for (i = 1; i <= nprintout; i++)
    {
        fprintf (echofile_fp, "%18.5f  %14.4f\n", printout[i],
                 printouttime[i]);
    }

    //Echo number of grid print intervals to file
    fprintf (echofile_fp,
             "\n\nNumber of Print Intervals for Grid Output = %d\n",
             nprintgrid);

    //Echo label for grid print intervals to file
    fprintf (echofile_fp, "\n Interval (hours)    Time (hours) ");
    fprintf (echofile_fp, "\n~~~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~\n");

    //Echo grid print interval and time breaks to file
    for (i = 1; i <= nprintgrid; i++)
    {
        fprintf (echofile_fp, "%18.5f  %14.4f\n", printgrid[i],
                 printgridtime[i]);
    }

    //Echo the name of the echo file name to file
    fprintf (echofile_fp, "\n\nEcho File Name: %s\n\n\n", echofile);

    //Close the Echo File
    fclose (echofile_fp);

//End of function: Return to ReadInputFile
}
