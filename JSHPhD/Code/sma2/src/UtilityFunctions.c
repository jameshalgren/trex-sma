/*----------------------------------------------------------------------
C-  File:		UtilityFunctions.c
C-
C-	Purpose/	
C-	Methods:	Concatenated group of general utility functions for
C-              statistics
C-
C-  Function
C-	 Listing:   Min, Max,
C-
C-	Inputs:		None
C-
C-	Outputs:	None
C-
C-	Controls:   None
C-
C-	Calls:		None
C-
C-	Called by:	None
C-
C-	Created:	John F. England, Jr.
C-				Bureau of Reclamation
C-				Flood Hydrology Group, D-8530
C-				Bldg. 67, Denver Federal Center, Denver, CO  80225
C-
C-				Mark Velleux
C-              Department of Civil Engineering
C-              Colorado State University
C-				Fort Collins, CO 80523
C-
C-	Date:		31-OCT-2003
C-
C-	Revised:  James Halgren	
C-				
C-	Date:		07-Jul-2010
C-
C-	Revisions:  added dateshift function	
C-
C-	Revised:  
C-				
C-	Date:
C-
C-	Revisions:
C-
C---------------------------------------------------------------------*/

//trex general variable declarations
#include "trex_general_declarations.h"
#include <time.h>
#define  ONEDAY         60*60*24
#define  ONEHOUR        60*60

//C
//C
			/*************************************/ 
			/*               FUNCTION: Max       */ 
			/*************************************/ 

/* Finds the maximum between two floating numbers */ 

double Max(double one,double theOther)
{
    double maximum;

    if (one >= theOther)
        maximum = one;
    else
        maximum = theOther;

    return (maximum);

//end of function Max
}

//C
//C
                        /*************************************/
                        /*               FUNCTION: Min       */
                        /*************************************/

/* Finds the minimum between two floating numbers */

double Min (double one, double theOther)
{
    double minimum;

    if (one <= theOther)
        minimum = one;
    else
        minimum = theOther;

    return (minimum);

//end of function Min
}

//C
//C
                        /*************************************/
                        /*               FUNCTION: Dateshift */
                        /*************************************/


/* JSH Comment: 2009-12-18 12:10:36 GMT-0700 
 * The following function was developed based on lots of information from
 * the internet -- but principally a piece of work by Dave Taylor 
 * found at http://www.askdavetaylor.com/date_math_in_linux_shell_script.html
 * I think that we qualify for fair use and so forth
 * but just to be sure, I have included his original copyright here.
 * If we ever get around to selling this code, there will be 
 * a small legal detail of making sure we honor this copyright
 * appropriately. 
 *
 * JSH Comment: 2009-12-18 12:13:00 GMT-0700 */
/** DATEMATH - Demonstration program that shows how to
do date mathematics by utilizing Unix procedures.
This is missing some error checking, etc., but will
show how to do "+n" and "-n" date math well enough.
(C) Copyright 2005 by Dave Taylor. Free to redistribute
if this copyright is left intact. Thanks.
***/
void dateshift (char *out_ptr,       //ouput of dateshift is a variable containing
           // the offset time in the format YYYY-MM-DD HH:MM:SS
                double offset,                  //number of hours forward or backward from start date
                long startyear,                 //year of time 0 in simulation
                long startmonth,                //month of time 0 in simulation
                long startday,                  //day of time 0 in simulation
                long starthour,                 //hour of time 0 in simulation
                long startminute,               //minute of time 0 in simulation
                long startsecond,               //second of time 0 in simulation
                long gmt_offset,                //difference in hours of start time zone from Greenwich mean time (WITHOUT DAYLIGHT SAVINGS)
                long daylightsavings)           //=1 if given start time is in daylight savings (USUALLY the summer time), =0 otherwise
{
    struct tm time_struct;
    time_t theTime;


    time_struct.tm_year = startyear;
    time_struct.tm_year -= 1900;
    time_struct.tm_mon = startmonth - 1; //Months are indexed from 0=January 
										//But the input file expects human readable 1=January
    time_struct.tm_mday = startday;
    time_struct.tm_hour = starthour;
    time_struct.tm_min = startminute;
    time_struct.tm_sec = startsecond;
    time_struct.tm_isdst = daylightsavings;

    theTime = mktime (&time_struct);
    if (mktime (&time_struct) == -1)
    {
        printf ("Error getting time.\n");
    }

    theTime += (double) (offset * ONEHOUR);
    theTime -= (double) (gmt_offset * ONEHOUR);
    strftime (out_ptr, 20, "%Y-%m-%d %H:%M:%S", gmtime (&theTime));

}
