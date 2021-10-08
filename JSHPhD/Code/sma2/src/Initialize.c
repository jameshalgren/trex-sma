/*----------------------------------------------------------------------
C-  Function:	Initialize.c
C-
C-	Purpose/	Initialize is called at the start of the simulation to
C-  Methods:    allocate memory and initialize values for variables used
C-              in computations but not read from input files.
C-
C-	Inputs:		None
C-
C-	Outputs:	None
C-
C-	Controls:   ksim
C-
C-	Calls:		InitializeWater. InitializeSolids, InitializeChemical
C-
C-	Called by:	trex
C-
C-	Created:	Mark Velleux
C-              Department of Civil Engineering
C-              Colorado State University
C-              Fort Collins, CO  80523
C-				
C-				John F. England, Jr.
C-				Bureau of Reclamation
C-				Flood Hydrology Group, D-8530
C-				Bldg. 67, Denver Federal Center, Denver, CO  80225
C-
C-	Date:		18-MAR-2004
C-
C-	Revisions:
C-
C-  Date:
C-
C---------------------------------------------------------------------*/

//trex general variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for water transport
#include "trex_water_declarations.h"

//trex global variable declarations for sediment transport
#include "trex_solids_declarations.h"

//trex global variable declarations for chemical transport
#include "trex_chemical_declarations.h"

//trex global variable declarations for environmental conditions
#include "trex_environmental_declarations.h"

void Initialize(void)
{
        /***************************************************/
    /*        Initialize Simulation Error File         */
        /***************************************************/

    //Allocate memory for simulation error file name
    errorfile = (char *) malloc (MAXNAMESIZE * sizeof (char));  //allocate memory

    //Set simulation error file name = inputfile name
    strcpy (errorfile, inputfile);

    //Add .err extension to simulation error file name
    errorfile = strcat (errorfile, ".err");

    //Set the simulation error file pointer to null
    errorfile_fp = NULL;

    //Open simulation error file and write header line
    errorfile_fp = fopen (errorfile, "w");

    //if the simulation error file pointer is null, abort execution
    if (errorfile_fp == NULL)
    {
        printf ("Can't create Simulation Error File:  %s\n", errorfile);        //Write message to screen
        exit (EXIT_FAILURE);    //abort

    }                           //End if simulation error file is null

    //Echo simulation error file name to error file
    fprintf (errorfile_fp, "\n\nSimulation Error File:  %s\n", errorfile);

    //Close simulation error file
    fclose (errorfile_fp);

    //Open simulation echo file in append mode
    echofile_fp = fopen (echofile, "a");

    //Echo simulation error file name to echo file
    fprintf (echofile_fp, "\n\nSimulation Error File:  %s\n", errorfile);

    //Close echo file
    fclose (echofile_fp);

        /***********************************************************************/
    /*        Initialize Summary Statistics and Mass Balance Files         */
        /***********************************************************************/

    //The summary statistics file is opened here and then closed
    //as a check to make sure it can be created to minimize the
    //chance that the program aborts at the end of a simulation
    //when the statistics file is in used in the WriteSummary*
    //routines...
    //
    //set the summary statistics file pointer to null
    statsfile_fp = NULL;

    //open the summary statistics file
    statsfile_fp = fopen (statsfile, "w");

    //if file pointer is still null, then abort
    if (statsfile_fp == NULL)
    {
        printf ("Error! Can't open Summary Statistics file %s\n", statsfile);   //write error message to screen
        exit (EXIT_FAILURE);    //exit with error
    }

    //Close the Summary Statistics File
    fclose (statsfile_fp);

    //The mass balance file is opened here and then closed as a
    //check to make sure it can be created to minimize the chance
    //that the program aborts at the end of a simulation when the
    //mass balance file is in used in the WriteMassBalance routine.
    //
    //set the summary statistics file pointer to null
    msbfile_fp = NULL;

    //open the mass balance file
    msbfile_fp = fopen (msbfile, "w");

    //if file pointer is still null, then abort
    if (msbfile_fp == NULL)
    {
        printf ("Error! Can't open Mass Balance file %s\n", msbfile);   //write error message to screen
        exit (EXIT_FAILURE);    //exit with error
    }

    //Close the Mass Balance File
    fclose (msbfile_fp);

    //Initialize water transport variables
    InitializeWater ();

    //if sediment transport is simulated
    if (ksim > 1)
    {
        //Initialize sediment transport variables
        InitializeSolids ();

        //if chemical transport is simulated
        if (ksim > 2)
        {
            //Initialize chemical transport variables
            InitializeChemical ();

        }                       //end if ksim > 2

        //Initialize environmental condition variables
        InitializeEnvironment ();

    }                           //end if ksim > 1

//End of function: Return to trex_main
}
