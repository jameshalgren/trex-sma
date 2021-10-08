/*----------------------------------------------------------------------
C-  Function:   trex.c (main module)
C-
C-  Purpose/    trex is the main module of the program.  It
C-  Methods:    obtains the input file name from the initial
C-              argument (argv), calls ReadInputFile to read
C-              all program inputs, calls Initialize to set
C-              initial values for variables.  Euler's method
C-              is used to generate solutions over space and
C-              time.  During integration, derivative terms
C-              are computed for transport processes for the
C-              current time step, output written, and mass
C-              balances computed to get water depths and the
C-              concentrations of solids and chemicals for
C-              the next time step.
C-
C-  Inputs:     argv
C-
C-  Outputs:    argc
C-
C-  Controls:   ksim, simtime, tend, timeprintout, timeprintgrid
C-
C-  Calls:      ReadInputFile, Initialize,
C-              TimeFunctionInit, ComputeInitialState,
C-              UpdateTimeFunction, UpdateEnvironment,
C-              WaterTransport, SolidsTransport,
C-              ChemicalTransport, WaterBalance,
C-              SolidsBalance, ChemicalBalance,
C-              WriteTimeSeries, WriteDumpFile,
C-              WriteGrids, NewState,
C-              WriteEndGrids, ComputeFinalState,
C-              WriteMassBalance, WriteSummary,
C-              FreeMemory, RunTime
C-
C-  Called by:  None
C-
C-  Created:    Mark Velleux
C-              Department of Civil Engineering
C-              Colorado State University
C-              Fort Collins, CO  80523
C-
C-              John F. England, Jr.
C-              Bureau of Reclamation
C-              Flood Hydrology Group, D-8530
C-              Bldg. 67, Denver Federal Center, Denver, CO  80225
C-
C-  Date:       17-MAR-2004
C-
C-  Revised:    James Halgren
C-
C-  Date:				17-DEC-2009
C-
C-  Revisions:  Formalized SMA-2 additions
C-
C---------------------------------------------------------------------*/
//trex global general variable declarations/definitions
#include "trex_general_declarations.h"
#include "trex_general_definitions.h"

//trex global variable declarations/definitions for Groundwater and Soil Moisture Accounting
#include "trex_SMA_declarations.h"
#include "trex_SMA_definitions.h"

//trex global variable declarations/definitions for water transport
#include "trex_water_declarations.h"
#include "trex_water_definitions.h"

//trex global variable declarations/definitions for sediment transport
#include "trex_solids_declarations.h"
#include "trex_solids_definitions.h"

//trex global variable declarations/definitions for chemical transport
#include "trex_chemical_declarations.h"
#include "trex_chemical_definitions.h"

//trex global variable declarations/definitions for environmental conditions
#include "trex_environmental_declarations.h"
#include "trex_environmental_definitions.h"

int main (int argc, char *argv[])
{
    //Local variable declarations/definitions
    char *prog;                 //name of program (defined from argv[0])

    int gridcount;              //running count of the number of time grids are printed

    double timeprintout,        //tabular output print time
      timeprintSMA,             //SMA states output print time
      timeprintgrid;            //grid output print time

    time_t clockstart;
    time_t clockstop;

    //CPU clock time (in seconds since epoch) at start of simulation
    time (&clockstart);

    //Assign program name to define stderr file for error reporting
    prog = argv[0];

    //Assign program name to define stderr file for error reporting
    strcpy (prog, argv[0]);

    //Allocate memory for input file name
    inputfile = (char *) malloc (MAXNAMESIZE * sizeof (char));  //allocate memory

    //Assign inputfile name from argv initial argument
    strcpy (inputfile, argv[1]);

        /*********************************************/
    /*         Read and initialize data          */
        /*********************************************/

    //Read input file
    ReadInputFile ();

    //Initialize Variables
    Initialize ();

    //Set simulation start time: tstart specified in Data Group A
    simtime = tstart;           //simulation time (hours)

    //Set simulation end time: end time = time of last break in dt time series
    tend = dttime[ndt];         //simulation end time (hours)

    //Determine starting index of all time functions for any simulation start time
    TimeFunctionInit ();

    //Developer's Note:  The counter to track the number of grid files output
    //                   during a simulation is always initialized to zero
    //                   regardless of the value for simulation start time
    //                   (tstart).  So the first grid file output will always
    //                   have a ".0" extention.  This is done because the
    //                   post processing tools used to animate model outputs
    //                   require that the output be in sequence, presumably
    //                   starting from zero.
    //
    //                   Eventually, this initialization should be moved into
    //                   the initization routine and adjusted for the value of
    //                   tstart so the first grid output might be in sequence
    //                   as needed to represent a tstart > 0.0.  The adjustment
    //                   would also be useful for a model "hot start" feature.
    //
    //Set index to track grid count print statements
    gridcount = 0;              //

    //set time series and grid print output start times to simulation start time
    timeprintout = simtime;
    timeprintSMA = simtime;
    timeprintgrid = simtime;

    //Compute initial volumes and masses for overland plane and channels
    ComputeInitialState ();

        /*********************************************/
    /*        Numerical Integration Loop         */
        /*********************************************/

    //write simulation start message to screen
    printf ("\n\n*********************************\n");
    printf ("*                               *\n");
    printf ("*   Beginning TREX Simulation   *\n");
    printf ("*                               *\n");
    printf ("*********************************\n\n\n");

    //Main loop over time...
    //
    //while the current simulation time <= the simulation end time
    while (simtime <= tend)
    {
        //if it is time to use a new dt, increment the timestep index (idt)
        if ((simtime > dttime[idt]) && (idt < ndt))
        {
            //increment timestep index
            idt = idt + 1;      //increment timestep index
        }

        //Update time functions
        UpdateTimeFunction ();

        //Update environmental conditions
        UpdateEnvironment ();

        //Compute water transport/forcing functions (rainfall, infiltration, flows)
        WaterTransport ();

        //Compute water mass balance (obtain flow depths and floodplain transfers)
        WaterBalance ();

        //if sediment transport is simulated
        if (ksim > 1)
        {
            //Compute sediment transport/forcing functions (transport, erosion, deposition, loads)
            SolidsTransport ();

            //Compute solids mass balance (obtain particle concentrations)
            SolidsBalance ();

            //if chemical transport is simulated
            if (ksim > 2)
            {
                //Compute chemical transport/forcing functions (mass transfer and transformations etc.)
                ChemicalTransport ();

                //Compute chemical mass balance (obtain chemical concentrations)
                ChemicalBalance ();

            }                   //end if ksim > 2

        }                       //end if ksim > 1

        //if simulation time >= time to report time series output
        if (simtime >= timeprintout)
        {
            //Write simtime and time series printout status to screen

            //jfe need to use stderr here as in K&R p. 163 to insure msg goes to screen....
            //jfe????? or should we allow user to redirect all to file??????? if so, which one???

            fprintf (stderr,
                     "  Time Series printout time = %9.6f\tSimulation Time (hours) = %9.6f\tDate and Time (local) %s\n",
                     timeprintout, simtime, displaydate);
            if (infopt == 2) // JSH DEL Find a better place to print the SMA summary state. 
            {
                fprintf (stderr, "SMASummaryState:\t%g\n", SMASummaryState);
            } // JSH DEL Find a better place to print the SMA summary state. 
//jfe                   fprintf("  Time Series printout time = %f\tSimulation Time (hours) = %f \n"
//jfe                                           ,timeprintout,simtime);

            //Write times series output to text files
            WriteTimeSeries ();

            //if the dump file name is not blank
            if (*dmpfile != '\0')
            {
                //Write detailed model results to dump file (*.dmp)
                WriteDumpFile ();

            }                   //end if dmpfile is not blank

            //if it is time to use a new output print interval
            if ((timeprintout >= printouttime[pdt]) && (pdt < nprintout))
            {
                //increment the printout index (pdt)
                pdt = pdt + 1;
            }

            //Set the new output print time
            timeprintout = timeprintout + printout[pdt];

        }                       //end if simtime >= timeprintout

        if (infopt == 2)
        {
            if (simtime >= timeprintSMA) //JSH ADD these two checks could be one -- if the 
				// JSH ADD initialization sets timeprintSMA to an arbitrarily huge number
				// JSH ADD unless an input value is assigned. 
            {
                WriteTimeSeriesSMA ();
                //JSH ADD, for now, only one print interval for SMA
                /*
                   //if the dump file name is not blank
                   if(*dmpfile != '\0')
                   {
                   //Write detailed model results to dump file (*.dmp)
                   WriteDumpFileSMA();

                   }    //end if dmpfile is not blank
                   //if it is time to use a new output print interval
                   if((timeprintSMA >= printSMAtime[SMApdt]) && (SMApdt < nprintSMA))
                   {
                   //increment the printSMA index (pdt)
                   pdt = pdt + 1;
                   }
                   //Set the new SMAput print time
                   timeprintSMA = timeprintSMA + printSMA[SMApdt];
                 */

                //Set the new SMAput print time
                timeprintSMA += printSMAinterval;
            }
        }

        //if simulation time >= time to report grid output
        if (simtime >= timeprintgrid)
        {
            //Write simtime and grid printout status to screen via stderr

//mlv                   fprintf(stderr, "  Grid printout time = %9.6f\tSimulation Time (hours) = %9.6f \n"
//mlv                                           ,prog, timeprintgrid,simtime);
//jfe                   printf(stderr, "  Grid printout time = %f\tSimulation Time (hours) = %f \n"
//jfe                                           ,timeprintgrid,simtime);

            //Write output grids to files
            WriteGrids (gridcount);

            //increment sequential count of grids print events for file extension
            gridcount = gridcount + 1;

            //if it is time to use a new grid print interval
            if ((timeprintgrid >= printgridtime[gdt]) && (gdt < nprintgrid))
            {
                //increment the printgrid index (gdt)
                gdt = gdt + 1;
            }

            //Set the new grid print time
            timeprintgrid = timeprintgrid + printgrid[pdt];

        }                       //end if simtime >= timeprintgrid

        //Assign new state (flow depths, concentrations) for next time step
        NewState ();

        //Increment simulation time (t = t + dt) (for upcoming iteration)
        simtime = simtime + dt[idt] / 3600.0;   //simtime (hours), dt (seconds)
        dateshift (displaydate, simtime, startyear, startmonth, startday,
                   starthour, startminute, startsecond, gmt_offset,
                   daylightsavingstoggle); // Returns "displaydate" in human readable form. 


    }                           // End Numerical Integration Loop

    /*******************************************/
    /* Compute final volumes & write results   */
    /*******************************************/

    //Write times series output to text files (ensure output of final iteration)
    WriteTimeSeries ();

    //if the dump file name is not blank
    if (*dmpfile != '\0')
    {
        //Write detailed model results to dump file (*.dmp) (ensure output of final iteration)
        WriteDumpFile ();

    }                           //end if dmpfile is not blank

    if (infopt == 2)
    {
        WriteTimeSeriesSMA ();
    }

    //Write output grids to files (ensure output of final iteration)
    WriteGrids (gridcount);

    //Write end of run (single) grids to files (net elevation change, gross erosion, etc.)
    WriteEndGrids (gridcount);

    //Compute final volumes and masses for the overland and channels
    ComputeFinalState ();

    //Write summary mass balance file
    WriteMassBalance ();

    //Write summary file
    WriteSummary ();

    //Deallocate memory
    FreeMemory ();

    //CPU clock time (in seconds since epoch) at end of simulation
    time (&clockstop);

    //Compute trex elapsed running time for this simulation
    RunTime (clockstart, clockstop);

    //End of Main Program
    return 0;
}
