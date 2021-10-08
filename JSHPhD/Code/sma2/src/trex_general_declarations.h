/*----------------------------------------------------------------------
C-  Header:     trex_general_declarations.h
C-
C-	Purpose/	Global declarations header file for trex.  Used in
C-	Methods:	concert with trex_general_definitions.h
C-
C-	Created:	Mark Velleux
C-              Department of Civil Engineering
C-              Colorado State University
C-              Fort Collins, CO  80523
C-
C-				John F. England, Jr.
C-				Bureau of Reclamation
C-				Flood Hydrology Group, D-8530
C-				Bldg. 67, Denver Federal Center,
C-              Denver, CO  80225
C-
C-	Date:		16-MAR-2004
C-
C-	Revised:
C-
C-	Date:
C-
C-	Revisions:
C-
C---------------------------------------------------------------------*/

    //JSH ADD This file has been slightly modified in the
    //JSH ADD sma2optimize branch of this code. 

//general order of header:
//	1. include files
//	2. global constant definitions
//	3. file declarations (organized by DataGroups A-E)
//	4. function prototypes (organized by order of appearance/DataGroups A-E)
//	5. variable declarations by state variables, general controls, then ksim
//		(1=water, 2=solids, 3=chemicals)

/******************************************/
/*      INCLUDED LIBRARY HEADER FILES     */
/******************************************/

#include <stdio.h>		//C library for standard I/O functions
#include <stdlib.h>		//C library for utility functions
#include <string.h>		//C library for string functions
#include <math.h>		//C library for math functions
#include <float.h>		//C library for floating point arithmetic
#include <time.h>		//C library for date and time functions

/***********************************/
/*   GLOBAL CONSTANT DEFINITIONS   */
/***********************************/

#define MAXHEADERSIZE 132	//Maximum size of character string for headers in input file
#define MAXNAMESIZE 100		//Maximum size of character string for names in input file and path name
#define TOLERANCE 1.0e-7	//Error tolerance for single precision math
#define DISPLAYDATESIZE 20  //Length of output from dateshift.c

/*************************/
/*   FILE DECLARATIONS   */
/*************************/

extern FILE *inputfile_fp;	//input file pointer

//Data Group A
extern FILE *echofile_fp;	//echo file pointer

//Data Group F
extern FILE *dmpfile_fp;		//dump file pointer
extern FILE *msbfile_fp;		//mass balance file pointer
extern FILE *statsfile_fp;		//statistics file pointer
extern FILE *errorfile_fp;		//simulation error file pointer

/***************************/
/*   FUNCTION PROTOTYPES   */
/***************************/

extern void ReadInputFile(void);		//Overall control for read of input file

extern void ReadDataGroupA(void);		//Reads Data Group A of input file

extern void ReadDataGroupF(void);		//Reads Data Group F of input file

extern void Initialize(void);			//Initializes variables (sets initial values)

extern void TimeFunctionInit(void);		//Initializes piecewise linear time functions (Loads, BCs)

extern void ComputeInitialState(void);	//Computes initial states of variables at beginning of simulation

extern void UpdateTimeFunction(void);	//Updates piecewise linear time functions (Loads, BCs)

extern void WriteTimeSeries(void);		//Writes time series (export) files

extern void WriteGrids(int);		//Writes grid files

extern void WriteEndGrids(int);		//Writes end of simulation grid files

extern void Grid(char *, float **, float, int);		//Writes a particular grid based on WriteGrids

extern void WriteDumpFile(void);		//Writes (dumps) details of model computations to dump file

extern void NewState(void);	//Set new state (conditions) (flow depths, concentrations) for next time step

extern void ComputeFinalState(void);	//Computes final states of variables at end of simulation

extern void WriteMassBalance(void);		//Writes end of simulation mass balance file

extern void WriteSummary(void);			//Writes summary files (water, solids and chemical) statistics at end of simulation

extern void FreeMemory(void);			//Frees all allocated memory at end of simulation

extern void	RunTime(time_t, time_t);		//Computes total run time of simulation

extern void SimulationError(int, int, int, int);	//Reports errors and terminates simulation

extern double Max(double, double);	//Maximum of some value

extern double Min(double, double);	//Minimum of some value

extern void dateshift (char *out_ptr,       //ouput of dateshift is a variable containing
           // the offset time in the format YYYY-MM-DD HH:MM:SS
           double offset,       //number of hours forward or backward from start date
           long startyear,      //year of time 0 in simulation
           long startmonth,     //month of time 0 in simulation
           long startday,       //day of time 0 in simulation
           long starthour,      //hour of time 0 in simulation
           long startminute,    //minute of time 0 in simulation
           long startsecond,    //second of time 0 in simulation
           long gmt_offset,     //difference in hours of start time zone from Greenwich mean time (WITHOUT DAYLIGHT SAVINGS)
           long daylightsavingstoggle);        //if given start time is in daylight savings timei (USUALLY the summer time)

extern char *StripString(char *);	//strip blanks/carriage returns from char strings read by fgets

/************************************/
/*   GLOBAL VARIABLE DECLARATIONS   */
/************************************/

	/*****************************/
	//  general controls and i/o
	/*****************************/
extern char
	header1[MAXHEADERSIZE],		//header1 in input file
	header2[MAXHEADERSIZE],		//header2 in input file
	header[MAXHEADERSIZE],		//general header line in input file
	varname[MAXNAMESIZE];		//dummy variable name in input file

extern char
	*inputfile,			//input file name (specified by the argv function)
	*echofile,			//echo (summary) file name
	*dmpfile,			//dump (detailed results) file
	*msbfile,			//mass balance file
	*statsfile,			//statistics file
	*errorfile;			//simulation error file

extern int
	i,		//loop index
	j,		//loop index
	k,		//loop index
	ksim,	//simulation type index:
				//	1 = hydrology,
				//	2 = sediment transport,
				//	3 = chemical transport
	idt,	//timestep index (counter). Used in main code.
	pdt,	//tabular output print interval index (counter)
	gdt,	//grid output print interval index (counter)
	nrows,	//Number of Rows
	ncols,	//Number of Columns
	ndt,	//Number of Time Steps
	nprintout,		//Number of print intervals for tabular output
	nprintgrid,		//Number of print intervals for grid output
    inputgridheaderopt;	//toggle reading of extra line on top of ASCII input grids

extern float
	w,			//width of square cell in overland plane (grid cell size) (m) (to be eventually replaced by dx, dy)
	dx,			//width of cell (x-direction) in overland plane (grid cell width) (m) (currently fixed at w)
	dy,			//width of cell (y-direction) in overland plane (grid cell width) (m) (currently fixed at w)
	*dt,				//numerical integration time step
	*dttime,			//Time up to which the time step is used
	*printout,			//Print interval for tabular output
	*printouttime,		//Time up to which the output print interval is used
	*printgrid,			//Print interval for grid output
	*printgridtime;		//Time up to which the grid print interval is used

extern double
	simtime,		// current time of simulation
	tstart,			// simulation start time
	tend;			// simulation end time

extern char *      displaydate[DISPLAYDATESIZE]; //A string to contain the output from dateshift.c

extern long
            startyear, //year of time 0 in simulation
            startmonth, //month of time 0 in simulation
            startday, //day of time 0 in simulation
            starthour, //hour of time 0 in simulation
            startminute, //minute of time 0 in simulation
            startsecond, //second of time 0 in simulation
            gmt_offset,  //difference in hours of start time zone from Greenwich mean time (WITHOUT DAYLIGHT SAVINGS)
            daylightsavingstoggle; //if given start time is in daylight savings timei (USUALLY the summer time)
