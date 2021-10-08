/*----------------------------------------------------------------------
C-  Header:     trex_general_definitions.h
C-
C-	Purpose/	Global definitions header file for trex.  Used in
C-	Methods:	concert with trex_general_declarations.h
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

//trex main routine
FILE *inputfile_fp = NULL;	//input file pointer

//Data Group A
FILE *echofile_fp = NULL;	//echo file pointer

//Data Group E

FILE *dmpfile_fp = NULL;		//dump file pointer
FILE *msbfile_fp = NULL;		//mass balance file pointer
FILE *statsfile_fp = NULL;		//statistics file pointer
FILE *errorfile_fp = NULL;		//simulation error file pointer

/************************************/
/*   GLOBAL VARIABLE DEFINITIONS    */
/************************************/


	/*****************************/
	//  general controls and i/o
	/*****************************/
char 
	header1[MAXHEADERSIZE],		//header1 in input file
	header2[MAXHEADERSIZE],		//header2 in input file
	header[MAXHEADERSIZE],		//general header line in input file
	varname[MAXNAMESIZE],		//dummy variable name in input file
	loadname[MAXNAMESIZE],		//load name
	bcname[MAXNAMESIZE];		//boundary condition name



char
	*inputfile,			//input file name (specified by the argv function)
	*echofile,			//echo (summary) file name
	*dmpfile,			//dump (detailed results) file
	*msbfile,			//mass balance file
	*statsfile,			//statistics file
	*errorfile;			//simulation error file

int 
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
    inputgridheaderopt;	//Toggle reading of extra line on top of ASCII input grids
	
float
	w,			//width of square cell in overland plane (grid cell size) (m) (to be eventually replaced by dx, dy)
	dx,			//width of cell (x-direction) in overland plane (grid cell width) (m) (currently fixed at w)
	dy,			//width of cell (y-direction) in overland plane (grid cell width) (m) (currently fixed at w)
	*dt,				//numerical integration time step
	*dttime,			//Time up to which the time step is used
	*printout,			//Print interval for tabular output
	*printouttime,		//Time up to which the output print interval is used
	*printgrid,			//Print interval for grid output
	*printgridtime;		//Time up to which the grid print interval is used

double	
	simtime,		// current time of simulation
	tstart,			// simulation start time
	tend;			// simulation end time

char *      displaydate[DISPLAYDATESIZE]; //A string to contain the output from dateshift.c

long
            startyear, //year of time 0 in simulation
            startmonth, //month of time 0 in simulation
            startday, //day of time 0 in simulation
            starthour, //hour of time 0 in simulation
            startminute, //minute of time 0 in simulation
            startsecond, //second of time 0 in simulation
            gmt_offset,  //difference in hours of start time zone from Greenwich mean time (WITHOUT DAYLIGHT SAVINGS)
            daylightsavingstoggle; //if given start time is in daylight savings timei (USUALLY the summer time)
