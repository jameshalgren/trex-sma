/*----------------------------------------------------------------------
C-  Header:     trex_SMA_declarationsSMA.h
C-
C-	Purpose/	Declarations of Soil Moisture Accounting methods.
C-	Methods:	
C-
C-	Created:	James Halgren
C-              Department of Civil Engineering
C-              Colorado State University
C-              Fort Collins, CO  80523
C-
C-	Date:		27-Feb-2007
C-
C-	Revised:
C-
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
//  5. variable declatations by state variables, general controls, then ksim
//     (1=water, 2=solids, 3=chemicals)

/*************************/
/*   FILE DECLARATIONS   */
/*************************/

extern FILE *SMAfile_fp;        //SMA file pointer
extern FILE *SMAUZKfile_fp;     //Upper zone distribution coefficients pointer
extern FILE *SMAstatefile_fp;   //reporting file for SMA states pointer

/***************************/
/*   FUNCTION PROTOTYPES   */
/***************************/

extern void percolationSMA (void);  //Infiltration and Soil Moisture Accounting for Groundwater Procedures
extern void ReadSMAFile (void);
extern void WriteTimeSeriesSMA (void);

/************************************/
/*   GLOBAL VARIABLE DECLARATIONS   */
/************************************/

extern char *SMAfile;           //SMA file
extern char *SMAUZKfile;        //Upper zone distribution coefficients 
extern char *SMAstatefile;      //reporting file for SMA states

extern double printSMAinterval; //interval for producing printout of SMA state
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

extern int h, m;                //Indices for looping through zones. 

extern int nuz;                 //number of upper zones 
extern int nlz;                 //number of lower zones 
extern int nlz3;                //number of third tier lower zones
extern int nio;                 //number of interflow outlets

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
//uz[1]->nparts = 1 -- one free water storage in the one upperzone
//lz[1]->nparts = 2 -- two free water storages in the one lowerzone
//uz[1]->part[1]->npo = 1 -- one percolation outlet for the upper zone: the one lower zone
//uz[1]->part[1]->nif = 1 -- one interflow outlet
//lz[1]->part[1]->nif = 1 -- one interflow outlet from the lower zone

extern double ***kuzSMA;        //array defining the portion of infiltrated water 
        //allocated to each upper zone
        //sum of all kuz's for each cell must be 1.#
extern double **etdemand;       //evaporation demand in each time step

//DEL The role of uznewinfiltration is assumed by 'wnew' in each upper zone.
//extern double *uznewinfiltration; //array defining additional infiltration
        //volume to be passed to the SMA routine.
extern double **newsoilmoisture;        //array to be returned to the TREX code
        //with the updated soil moisture state after computing the 
        //SMA percolation, interflow, and storage depletion. 


extern double fwcuzSMA;         //total actual storage = sum of nuz upperzone fwctotals
extern double twcuzSMA;         //total actual storage = sum of nuz upperzone twctotals
extern double fwmuzSMA;         //total capacity = sum of nuz upperzone fwmtotals
extern double twmuzSMA;         //total capacity = sum of nuz upperzone twmtotals
extern double fwclzSMA;         //total actual storage = sum of nlz lowerzone fwctotals
extern double twclzSMA;         //total actual storage = sum of nlz lowerzone twctotals
extern double fwmlzSMA;         //total capacity = sum of nlz lowerzone fwmtotals
extern double twmlzSMA;         //total capacity = sum of nlz lowerzone twmtotals

/* Declarations associated with 09_ and 10_ in percolationSMA.c*/
extern long SMArejectionMethod;
extern double smaRejectvolume;

extern long SMAinfReinitMethod;
extern double smaReinitializationStormEpsilon;    // JSH ADD When the wnew drops below this threshold, the storm is considered to have ended. 
extern long smaReinitializationStormToggle;   // JSH ADD 1 = Storming (wnew > epsilon); 0 = not storming;
extern long smaReinitializationPrintMessageToggle;   // JSH ADD 1 = Storming (wnew > epsilon); 0 = not storming;
extern long smaReinitializationStormCount;  //Number of independent storms above the threshold; 

extern double SMASummaryState;
extern double SMASummaryDivisor;
/* Declarations associated with 09_ and 10_ in percolationSMA.c*/

// SMA interflow outlet generic type
typedef struct
{
    double wnew;                //new water tossed into this interflow outlet
    int intfout_i;              //row of outlet cell
    int intfout_j;              //column of outlet cell
    //ADD additional parameters should be added to define 
    //ADD outlets of a different type than surface cells. 
} interflowoutlet_t;

// SMA lower zone free water part type 
typedef struct
{
    double fwc;                 //lower zone free water actual storage
    double fwm;                 //lower zone free water maximum storage
    int includeSummaryState;   //Include this lower zone free volume in summary state calculations
    double basf;                //volume draining to surface cells from this lowerzone part
    double k;                   //lower zone free water storage depletion coefficient
    double *kbasf;              //partition of outgoing water into each of nbo interflow outlets
    double percolation;         //volume draining to any subsequent lower zones
} lowerzonepart_t;

// SMA lower zone type 
typedef struct
{
    double twc;                 //lower zone tension water actual storage
    double twm;                 //lower zone tension water maximum storage
    int includeSummaryState;   //Include this lower zone tension volume in summary state calculations
    double fwctotal;            //total actual storage = sum of nparts fwc
    double fwmtotal;            //total capacity = sum of nparts fwm
    double basftotal;           //draining from this lower zone
    double perctotal;           //total percolation draining from this lower zone
    double fwdeficitratio;      //Use to compute the distribution of percolated water from the upper zone
    double wnew;                //new volume of water percolated from connected upper zones in this time step
    double et;                  //evapotranspiration from lower zone tension water
    double saved;               // lowerzone free water not available to resupply tension
    double areafactor;          //area of cells contributing to lower zone water
    double pbase;               //primary base flow array:  pbase = Sum ( lzfwm * lzk )
    double percdemand;          // percolation demand 
    // percdemand = pbase * [ 1 + [ zperc * (( twdef + fwdef ) / ( twm + fwm )) ^ rexp]]
    double zperc;               //maximum percolation capacity factor
    double rexp;                //curvature exponent for drainage of lower zone free water
    double pfree;               //percent allocated directly to free water while tension water still filling
    int nparts;                 //number of fw storages in this lower zone -- default is two (primary and secondary)
    double *kpart;              //partition of outgoing water into each of the free water storage volumes
    lowerzonepart_t **part;     //array of lowerzone parts (each with a free water storage volumes)
    double *kperc;              //Partitioning of outgoing water into each of npo percolation outlets
} lowerzone_t;

// SMA upper zone free water part type 
typedef struct
{
    double fwc;                 //upper zone free water actual storage
    double fwm;                 //upper zone free water maximum storage
    int includeSummaryState;   //Include this upper zone free volume in summary state calculations
    double et;                  //et removed during the time step
    double intf;                //volume draining to surface cells from this upper zone part
    double k;                   //upper zone free water storage depletion coefficient
    double percolation;         //volume draining to any subsequent lower zones
    double *kintf;              //partition of outgoing water into each of nif interflow outlets
} upperzonepart_t;

// SMA upper zone type 
typedef struct
{
    double twc;                 //upper zone tension water actual storage
    double twm;                 //upper zone tension water maximum storage
    int includeSummaryState;   //Include this upper zone tension volume in summary state calculations
    double fwctotal;            //total actual storage = sum of nparts fwc
    double fwmtotal;            //total capacity = sum of nparts fwm
    double intftotal;           //total interflow draining from this upper zone
    double perctotal;           //total percolation draining from this upper zone
    double fwdeficitratio;      //Used to compute the distribution of infiltrated water from the land surface
    double *kperc;              //Partitioning of outgoing water into each of npo percolation outlets
    // Partitioning of percolation is according to demand hence there is 
    // no need for a scaling factor -- kpercoutlet is intended to be a switch
    // to turn on or off the demand influence from any of the various lower
    // zones.
    double precip;              //new precipitation -- only used to trigger re-initialization of infiltration parameters
    double wnew;                //new volume of water in each time step
	double pcp;                 //new volume of water converted to depth for SMA calculations
	double pav;                 //water depth available after initial tension water requirements are satisfied
    double wreject;             //volume of water rejected as saturation excess flow
    double et;                  //evapotranspiration from upper zone tension water
    double et_deep;             //evapotranspiration from lower zones passing through this zone
    double etdemand;            //evapotranspiration from upper zone tension water
    double areafactor;          //area of cells contributing to upper zone water
    int nparts;                 //number of free water storages in this upper zone -- default is one
    double *kpart;              //partition of outgoing water into each of the free water storages
    upperzonepart_t **part;     //array of upperzone parts (each with a tw/fw pair);
} upperzone_t;

extern lowerzone_t **lzSMA;
extern upperzone_t **uzSMA;
extern interflowoutlet_t **ioSMA;
