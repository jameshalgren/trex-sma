/*----------------------------------------------------------------------
C-  Header:     trex_SMA_definitionsSMA.h
C-
C-	Purpose/	Global definitions header file for Soil Moisture
C-	Methods:	Accounting methods.
C-
C-	Created:	James Halgren
C-              Department of Civil Engineering
C-              Colorado State University
C-              Fort Collins, CO  80523
C-				
C-	Date:		05-MAR-2007
C-
C-  Revised:

C-	Date:
C-
C-	Revisions:
C-
C---------------------------------------------------------------------*/

//general order of header:
//  1. include files
//  2. global constant definitions
//  3. file declarations (organized by DataGroups A-E)
//  4. function prototypes (organized by order of appearance/DataGroups A-E)
//  5. variable declatations by state cariables, general controls, then ksim
//     (1=water, 2=solids, 3=chemicals)

/*************************/
/*   FILE DECLARATIONS   */
/*************************/

FILE *SMAfile_fp;               //SMA file pointer
FILE *SMAUZKfile_fp;            //Upper zone distribution coefficients pointer
FILE *SMAstatefile_fp;          //reporting file for SMA states pointer

/************************************/
/*   GLOBAL VARIABLE DEFINITIONS    */
/************************************/

char *SMAfile;                  //SMA file
char *SMAUZKfile;               //Upper zone distribution coefficients 
char *SMAstatefile;             //reporting file for SMA states

double printSMAinterval;        //interval for producing printout of SMA state
//Instead of primary and secondary
//lower zones and a single upper zone, as in the 
//traditional SAC-SMA implementation, here
//we define an array which can allow for a 
//theoretically infinite number of upper and  lower
//zone divisions to conceptually represent the 
//various drainage pathways. 
//Of course, this is "over-parameterization" and 
//almost guarantees a non-unique model solution
//but it also allows for (under skillful manipulation)
//simulation of complex conceptual subsurface hydrology
//such as in areas influenced by macro/mesopores.
//Future coding might include thresholds or other
//more complex functions to control the filling and
//emptying of the various upper zones. 
//For the time, the filling of different upper zones 
//partitioned with a simple coefficient.
//Thanks to Dr. Stephanie Kampf for the suggestion
//to pursue this type of modification.

int h = 0, m = 0;

int nuz = 0;                    //number of upper zones
int nlz = 0;                    //number of lower zones
int nlz3 = 0;                   //number of third tier lower zones
int nio = 0;                    //number of interflow outlets

//In the standard SAC-SMA model, the upperzone is a single
//unit consisting of a tension water component and a free
//water component. The lower zone partitions percolation 
//from the upper zone into a Primary and Secondary Lower 
//Zone, each with a free water and tension water component. 
//All interflow and base flow released from the upper and 
//lower zones appears at the outlet of the watershed. 
//From this the default values of the indicies are:
//nuz = 1 -- one upper zone
//nlz = 1 -- one lower zone
//uz[1]->nparts = 1 -- one tw/fw pair in the one upperzone
//lz[1]->nparts = 2 -- two tw/fw pairs in the one lowerzone
//uz[1]->part[1]->npo = 1 -- one percolation outlet for the upper zone: the one lower zone
//uz[1]->part[1]->nif = 1 -- one interflow outlet
//lz[1]->part[1]->nif = 1 -- one interflow outlet from the lower zone

double ***kuzSMA;               //array defining the portion of infiltrated water 
    //allocated to each upper zone
    //sum of all kuz's for each cell must be 1.#
double **etdemand;              //evaporation demand in each time step
double **newsoilmoisture;       //array to be returned to the TREX code
    //with the updated soil moisture state after computing the 
    //SMA percolation, interflow, and storage depletion. 


double fwcuzSMA;                //total actual storage = sum of nuz upperzone fwctotals
double twcuzSMA;                //total actual storage = sum of nuz upperzone twctotals
double fwmuzSMA;                //total capacity = sum of nuz upperzone fwmtotals
double twmuzSMA;                //total capacity = sum of nuz upperzone twmtotals
double fwclzSMA;                //total actual storage = sum of nlz lowerzone fwctotals
double twclzSMA;                //total actual storage = sum of nlz lowerzone twctotals
double fwmlzSMA;                //total capacity = sum of nlz lowerzone fwmtotals
double twmlzSMA;                //total capacity = sum of nlz lowerzone twmtotals

/* Definitions associated with 09_ and 10_ in percolationSMA.c*/
long SMArejectionMethod = 1;
double smaRejectvolume = 0.0;

long SMAinfReinitMethod = 1;
double smaReinitializationStormEpsilon = 0.0001;    // JSH ADD When the wnew drops below this threshold, the storm is considered to have ended. 
long smaReinitializationStormToggle = -1;   // JSH ADD 1 = Storming (wnew > epsilon); 0 = not storming;
long smaReinitializationPrintMessageToggle = -1;   // JSH ADD 1 = Storming (wnew > epsilon); 0 = not storming;
long smaReinitializationStormCount = -1;   // JSH ADD 1 = Storming (wnew > epsilon); 0 = not storming;

double SMASummaryState = 0.0;
double SMASummaryDivisor = 0.0;
/* Definitions associated with 09_ and 10_ in percolationSMA.c*/

lowerzone_t **lzSMA;
upperzone_t **uzSMA;
interflowoutlet_t **ioSMA;
