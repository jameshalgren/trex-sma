/*----------------------------------------------------------------------
C-  Function:   InitializeWater.c
C-
C-  Purpose/    Allocate memory for and initialize water variables
C-  Methods:    used in computations but not read from input files.
C-
C-  Inputs:     None
C-
C-  Outputs:    None
C-
C-  Controls:   infopt, chnopt
C-
C-  Calls:      None
C-
C-  Called by:  Initialize
C-
C-  Created:    Original Coding: Rosalia Rojas-Sanchez (and others)
C-              Department of Civil Engineering
C-              Colorado State University
C-              Fort Collins, CO 80523
C-
C-  Date:       19-JUN-2003 (last version)
C-
C-  Revised:    Mark Velleux
C-              Department of Civil Engineering
C-              Colorado State University
C-              Fort Collins, CO  80523
C-
C-              John F. England, Jr.
C-              Bureau of Reclamation
C-              Flood Hydrology Group, D-8530
C-              Bldg. 67, Denver Federal Center, Denver, CO  80225
C-
C-  Date:       08-JUL-2004
C-
C-  Revisions:  Complete rewrite.
C-
C-  Revised:    James Halgren
C-              Colorado State University
C-              james.halgren@colostate.edu
C-
C-  Date:       2009
C-
C-  Revisions:  Added SMA options
C-
C---------------------------------------------------------------------*/

//trex global variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for water transport
#include "trex_water_declarations.h"

//trex global variable declarations for soil moisture accounting
#include "trex_SMA_declarations.h"

void InitializeWater (void)
{
    /*********************************************/
    /*        Initialize Global Counters         */
    /*********************************************/

    //Initialize iteration index used to control N-S or E-W routing in OverlandRoute
    iteration = 0;

    /**************************************************/
    /*        Initialize Total Volume Variables       */
    /**************************************************/

    initialwaterovvol = 0.0;    //initial water volume in overland plane
    finalwaterovvol = 0.0;      //final water volume in overland plane

    //Allocate memory for total volume by cell variables

    //initial water volume in an overland cell (m3)
    initialwaterov = (float **) malloc ((nrows + 1) * sizeof (float *));

    //final water volume in an overland cell (m3)
    finalwaterov = (float **) malloc ((nrows + 1) * sizeof (float *));

    //Loop over number of rows
    for (i = 1; i <= nrows; i++)
    {
        //Allocate remaining memory for overland volume parameters
        //(values are initialized in ComputeInitialVolume)
        initialwaterov[i] = (float *) malloc ((ncols + 1) * sizeof (float));
        finalwaterov[i] = (float *) malloc ((ncols + 1) * sizeof (float));

    }                           //end loop over rows

    //if channels are simulated
    if (chnopt == 1)
    {
        initialwaterchvol = 0.0;        //initial water volume in overland plane
        finalwaterchvol = 0.0;  //final water volume in overland plane

        //Allocate memory for total volume by node variables

        //initial water volume in a channel node (m3)
        initialwaterch = (float **) malloc ((nlinks + 1) * sizeof (float *));
        //final water volume in a channel node (m3)
        finalwaterch = (float **) malloc ((nlinks + 1) * sizeof (float *));

        //Loop over number of links
        for (i = 1; i <= nlinks; i++)
        {
            //Allocate remaining memory for overland volume parameters
            //(values are initialized in ComputeInitialVolume)
            initialwaterch[i] =
                (float *) malloc ((nnodes[i] + 1) * sizeof (float));
            finalwaterch[i] =
                (float *) malloc ((nnodes[i] + 1) * sizeof (float));
        }                       //end loop over links
    }                           //end if chnopt == 1

    /*******************************************************/
    /*        Initialize Global Rainfall Variables         */
    /*******************************************************/

    //Allocate memory for rainfall parameters
    grossrainrate = (float **) malloc ((nrows + 1) * sizeof (float *)); //gross rainfall rate (m/s)
    grossraindepth = (float **) malloc ((nrows + 1) * sizeof (float *));        //cumulative gross rainfall depth (m)
    grossrainvol = (float **) malloc ((nrows + 1) * sizeof (float *));  //gross rainfall volume (m3)
    netrainrate = (float **) malloc ((nrows + 1) * sizeof (float *));   //net rainfall rate (m/s)
    netrainvol = (float **) malloc ((nrows + 1) * sizeof (float *));    //net rainfall volume (m3)

    //Allocate memory for interception parameters
    //remaining (unstatisfied) interception depth (m)
    interceptiondepth = (float **) malloc ((nrows + 1) * sizeof (float *));
    //cumulative interception volume (m3)
    interceptionvol = (float **) malloc ((nrows + 1) * sizeof (float *));

    //Loop over number of rows
    for (i = 1; i <= nrows; i++)
    {
        //Allocate memory for rainfall parameters
        //(values are initialized in Rainfall and Interception)
        grossrainrate[i] = (float *) malloc ((ncols + 1) * sizeof (float));
        grossraindepth[i] = (float *) malloc ((ncols + 1) * sizeof (float));
        grossrainvol[i] = (float *) malloc ((ncols + 1) * sizeof (float));
        netrainrate[i] = (float *) malloc ((ncols + 1) * sizeof (float));
        netrainvol[i] = (float *) malloc ((ncols + 1) * sizeof (float));

        //Allocate remaining memory for interception parameters
        interceptiondepth[i] =
            (float *) malloc ((ncols + 1) * sizeof (float));
        interceptionvol[i] = (float *) malloc ((ncols + 1) * sizeof (float));

        //Loop over number of columns
        for (j = 1; j <= ncols; j++)
        {
            //if the cell is in the domain
            if (imask[i][j] != nodatavalue)
            {
                //Set initial values for cumulative gross rainfall depth
                //these are used on right hand side of equation in Rainfall routine
                grossraindepth[i][j] = 0.0;

                //Initialize cumulative gross rainfall volume entering a cell (m3)
                grossrainvol[i][j] = 0.0;

                //Initialize interception parameters...
                //
                //Initialize interception depth (m) (input values in mm, convert to m)
                interceptiondepth[i][j] =
                    (float) (interceptionclass[landuse[i][j]] * 0.001);

                //Initialize cumulative interception volume in a cell (m3)
                interceptionvol[i][j] = 0.0;

                //Initialize cumulative net rainfall volume entering a cell (m3)
                netrainvol[i][j] = 0.0;

            }                   //end if imask[i][j] is not null (cell is is domain)

        }                       //end loop over columns

    }                           //end loop over rows

    //If rainopt = 5 (rain grid files will be read)
    if (rainopt == 5)
    {
        //Allocate memory for rainfall grid file name
        raingridfile = (char *) malloc (MAXNAMESIZE * sizeof (char));   //allocate memory

        //Initialize the rain grid counter
        //
        //if the simulation start time is greater than zero
        if (tstart > 0.0)
        {
            raingridcount = (int) (tstart / raingridfreq);
        }
        else                    //tstart equals zero
        {
            raingridcount = 0;

        }                       //end if tstart > 0.0

    }                           //end if rainopt = 5

    /*****************************************************************************/
    /*        Initialize Global Infiltration/Transmission Loss Variables         */
    /*****************************************************************************/

    //if infiltration is simulated
    if (infopt > 0)
    {
        //Allocate memory for infiltration parameters
        //Note: infiltrationdepth is allocated in ReadInfiltrationFile....
        infiltrationrate = (float **) malloc ((nrows + 1) * sizeof (float *));  //infiltration rate (m/s)
        infiltrationvol = (float **) malloc ((nrows + 1) * sizeof (float *));   //infiltration volume (m3)

        //Allocate memory for location (layer) in the overland soil stack
        nstackov = (int **) malloc ((nrows + 1) * sizeof (int *));      //soil stack layer

        //Loop over number of rows
        for (i = 1; i <= nrows; i++)
        {
            //Allocate remaining memory for infiltration parameters
            infiltrationrate[i] =
                (float *) malloc ((ncols + 1) * sizeof (float));
            infiltrationvol[i] =
                (float *) malloc ((ncols + 1) * sizeof (float));

            //Allocate remaining memory for location (layer) in the overland soil stack
            nstackov[i] = (int *) malloc ((ncols + 1) * sizeof (int));  //soil stack layer

            //Loop over number of columns
            for (j = 1; j <= ncols; j++)
            {
                //if the cell is in the domain
                if (imask[i][j] != nodatavalue)
                {
                    //Initialize cumulative infiltration volume (m3)
                    infiltrationvol[i][j] = 0.0;

                    //Initialize overland soil stack layer (for ksim = 1, nstackov = 1)
                    nstackov[i][j] = 1;

                }               //end if imask[i][j] is not null (cell is is domain)

            }                   //end loop over columns

        }                       //end loop over rows

        //Initialize total sum of infiltration volumes
        totalinfiltvol = 0.0;

        //Initialize SMA parameters
        //BEGIN SMA initialization
        if (infopt == 2)
        {
            //Allocate memory for SMA specific infiltration parameters
            SMAinfiltrationvol = (float **) malloc ((nrows + 1) * sizeof (float *));    //infiltration volume (m3)
            SMAprecipvol = (float **) malloc ((nrows + 1) * sizeof (float *));  //precip volume (m3)

            //Loop over number of rows
            for (i = 1; i <= nrows; i++)
            {
                //Allocate remaining memory for SMA specific infiltration parameters
                SMAinfiltrationvol[i] = (float *)
                    malloc ((ncols + 1) * sizeof (float));
                SMAprecipvol[i] = (float *)
                    malloc ((ncols + 1) * sizeof (float));

                //Loop over number of columns
                for (j = 1; j <= ncols; j++)
                {
                    //if the cell is in the domain
                    if (imask[i][j] != nodatavalue)
                    {
                        //Initialize incremental infiltration volume (m3)
                        SMAinfiltrationvol[i][j] = 0.0;
                        SMAprecipvol[i][j] = 0.0;

                    }           //end if imask[i][j] is not null (cell is is domain)

                }               //end loop over columns

            }                   //end loop over rows

            //Set nlz and nuz if not yet initialized from input
            // JSH ADD the variables that need to be read from the input file are:
            // JSH ADD NUZ
            // JSH ADD NLZ
            // JSH ADD NIO
            // JSH ADD AND for Each of NUZ, NLZ and NIO, there will need to be a line such as:
            // JSH ADD NIO_NUMBER INTFOUT_I INTFOUT_J 
            // JSH ADD NLZ_NUMBER PBASE ZPERC REXP PFREE NPARTS:
            // JSH ADD NLZ_NUMBER NPART_NUMBER FWM TWM K
            // JSH ADD NUZ_NUMBER 
            // JSH ADD   NUZ_NUMBER FWM TWM K 
            // JSH ADD     NUZ_NUMBER NIO_NUMBER KINTFOUTLET
            // JSH ADD   ...
            if (nuz == 0)       //JSH ADD This value should come from an input file
            {
                nuz = 1;
            }
            if (nlz == 0)       //JSH ADD This value should come from an input file
            {
                nlz = 1;
            }
            if (nio == 0)       //JSH ADD This value should come from an input file
            {
                //JSH DEL four outlets are CG-6, CG-1, SD-03A, and CG-4
                nio = 1;
            }

            /*
               ioSMA =
               (interflowoutlet_t **) malloc ((nio + 1) *
               sizeof (interflowoutlet_t *));
             */
            for (m = 1; m <= nio; m++)
            {
                /*
                   ioSMA[m] =
                   (interflowoutlet_t *) malloc ((m) *
                   sizeof
                   (interflowoutlet_t));
                   ioSMA[m]->intfout_i = 9;    //JSH ADD This value should come from an input file
                   ioSMA[m]->intfout_j = 1;    //JSH ADD This value should come from an input file
                 */
                ioSMA[m]->wnew = 0.0;
            }
            /*
               lzSMA =
               (lowerzone_t **) malloc ((nlz + 1) * sizeof (lowerzone_t *));
               uzSMA =
               (upperzone_t **) malloc ((nuz + 1) * sizeof (upperzone_t *));
             */
            for (k = 1; k <= nlz; k++)
            {
                /*
                   lzSMA[k] = (lowerzone_t *) malloc (sizeof (lowerzone_t));
                   lzSMA[k]->twc = 37.173; //JSH ADD This value should come from an input file
                   lzSMA[k]->twm = 98.340; //JSH ADD This value should come from an input file
                   lzSMA[k]->pbase = 2.975;        //JSH ADD This value should come from an input file
                   lzSMA[k]->zperc = 22.00;        //JSH ADD This value should come from an input file
                   lzSMA[k]->rexp = 6.405; //JSH ADD This value should come from an input file
                   lzSMA[k]->pfree = 0.020000;     //JSH ADD This value should come from an input file
                   lzSMA[k]->saved = 0.010000;     //JSH ADD This value should come from an input file
                   //if nparts was not initialized, set it to the default
                   if (lzSMA[k]->nparts <= 0)      //JSH ADD This value should come from an input file
                   {
                   lzSMA[k]->nparts = 2;
                   }
                 */
                lzSMA[k]->percdemand = 1.0;     //JSH ADD This value should come from an input file
                lzSMA[k]->basftotal = 0;
                lzSMA[k]->fwctotal = 0;
                lzSMA[k]->fwmtotal = 0;
                lzSMA[k]->wnew = 0.0;
                lzSMA[k]->et = 0.0;
                lzSMA[k]->areafactor = 0.0;
                /*
                   lzSMA[k]->part =
                   (lowerzonepart_t **) malloc ((lzSMA[k]->nparts + 1) *
                   sizeof (lowerzonepart_t *));
                   lzSMA[k]->kpart =
                   (double *) malloc ((lzSMA[k]->nparts + 1) *
                   sizeof (double));
                 */
                for (h = 1; h <= lzSMA[k]->nparts; h++)
                {
                    /*
                       lzSMA[k]->kpart[h] = 1.0 / (double) lzSMA[k]->nparts;       // JSH ADD This value should come from an input file
                       lzSMA[k]->part[h] =
                       (lowerzonepart_t *) malloc (sizeof (lowerzonepart_t));
                       lzSMA[k]->part[h]->fwc = 2.6599964321674 * h - 2.52099543809184;    // JSH ADD This value should come from an input file
                       lzSMA[k]->part[h]->fwm = -60.2800171956733 * h + 167.209984069236;  // JSH ADD This value should come from an input file
                       lzSMA[k]->part[h]->k = 0.0269001291786439 * h - 0.0157002276757022; // JSH ADD This value should come from an input file
                     */
                    lzSMA[k]->fwmtotal += lzSMA[k]->part[h]->fwm;
                    lzSMA[k]->fwctotal += lzSMA[k]->part[h]->fwc;
                    lzSMA[k]->part[h]->basf = 0.0;
                    /*
                       lzSMA[k]->part[h]->kbasfoutlet =
                       (double *) malloc ((nio + 1) * sizeof (double));
                       for (m = 1; m <= nio; m++)
                       {
                       lzSMA[k]->part[h]->kbasfoutlet[m] = 1 / nio;    //JSH ADD This value should come from an input file
                       }
                     */
                }
            }

            kuzSMA = (double ***) malloc ((nuz + 1) * sizeof (double **));
            for (k = 1; k <= nuz; k++)
            {
                /*
                   uzSMA[k] = (upperzone_t *) malloc (sizeof (upperzone_t));
                   uzSMA[k]->twc = 15.139; //JSH ADD This value should come from an input file
                   uzSMA[k]->twm = 15.240; //JSH ADD This value should come from an input file
                 */
                uzSMA[k]->fwctotal = 0.0;
                uzSMA[k]->fwmtotal = 0.0;
                uzSMA[k]->intftotal = 0.0;
                uzSMA[k]->perctotal = 0.0;
                uzSMA[k]->wnew = 0.0;
                uzSMA[k]->et = 0.0;
                uzSMA[k]->etdemand = 0.0;
                uzSMA[k]->areafactor = 0.0;

                // if nparts was not initialized, set it to the default
                if (uzSMA[k]->nparts <= 0)      //JSH ADD This value should come from an input file
                {
                    uzSMA[k]->nparts = 1;
                }
                /*
                   uzSMA[k]->part = (upperzonepart_t **)
                   malloc ((uzSMA[k]->nparts +
                   1) * sizeof (upperzonepart_t));
                   uzSMA[k]->kpart =
                   (double *) malloc ((uzSMA[k]->nparts + 1) *
                   sizeof (double));
                 */
                for (h = 1; h <= uzSMA[k]->nparts; h++)
                {
                    /*
                       uzSMA[k]->kpart[h] = 1.0 / (double) uzSMA[k]->nparts;
                       uzSMA[k]->part[h] = (upperzonepart_t *)
                       malloc ((uzSMA[k]->nparts +
                       1) * sizeof (upperzonepart_t));
                       uzSMA[k]->part[h]->fwc = 4.451;     //JSH ADD This value should come from an input file
                       uzSMA[k]->part[h]->fwm = 48.280;    //JSH ADD This value should come from an input file
                       uzSMA[k]->part[h]->k = 0.0106;      //JSH ADD This value should come from an input file
                     */
                    uzSMA[k]->fwctotal += uzSMA[k]->part[h]->fwc;
                    uzSMA[k]->fwmtotal += uzSMA[k]->part[h]->fwm;
                    uzSMA[k]->part[h]->intf = 0.0;
                    /*
                       uzSMA[k]->part[h]->kintfoutlet =
                       (double *) malloc ((nio + 1) * sizeof (double));
                       for (m = 1; m <= nio; m++)
                       {
                       uzSMA[k]->part[h]->kintfoutlet[m] = 1 / nio;    //JSH ADD This value should come from an input file
                       }
                     */
                }
                /*
                   uzSMA[k]->kpercoutlet =
                   (double *) malloc ((nlz + 1) * sizeof (double));
                   for (m = 1; m <= nlz; m++)
                   {
                   uzSMA[k]->kperc[m] = 1 / nlz; //JSH ADD This value should come from an input file
                   }
                 */
                uzSMA[k]->perctotal = 0.0;

                //These only need initialzation once for all upper zones
                if (k == 1)
                {
                    etdemand = (double **)
                        malloc ((nrows + 1) * sizeof (double *));
                    newsoilmoisture = (double **)
                        malloc ((nrows + 1) * sizeof (double *));
                    //Loop over number of rows
                    for (i = 1; i <= nrows; i++)
                    {
                        etdemand[i] =
                            (double *) malloc ((ncols + 1) * sizeof (double));
                        newsoilmoisture[i] =
                            (double *) malloc ((ncols + 1) * sizeof (double));
                        //Loop over number of columns
                        for (j = 1; j <= ncols; j++)
                        {
                            etdemand[i][j] = 0.0;
                            newsoilmoisture[i][j] = 0;  //new soil moisture added in meters
                            if (imask[i][j] != nodatavalue)
                            {
                                //etdemand[i][j] = 0.0004939;     //depth of evaporation in meters //JSH ADD **This value should be initialized to zero and then modified by the internal processes of the model
                                //.4939 mm was the number inherited from the Sac-mini code.
                                etdemand[i][j] = 0.000; //depth of evaporation in meters //JSH ADD **This value should be initialized to zero and then modified by the internal processes of the model
                            }
                        }
                    }
                }

                kuzSMA[k] =
                    (double **) malloc ((nrows + 1) * sizeof (double *));
                //Loop over number of rows
                for (i = 1; i <= nrows; i++)
                {
                    kuzSMA[k][i] =
                        (double *) malloc ((ncols + 1) * sizeof (double));
                    //Loop over number of columns
                    for (j = 1; j <= ncols; j++)
                    {
                        if (imask[i][j] != nodatavalue)
                        {
                            kuzSMA[k][i][j] = 1.0;      //JSH ADD This value should come from an input file
                        }
                        else
                        {
                            kuzSMA[k][i][j] = 0.0;
                        }       //JSH ADD This value should come from an input file
                        uzSMA[k]->areafactor += kuzSMA[k][i][j];
                    }
                }
                //After initializing the upper zone areafactor,
                //Divide the factor into the lowerzones according to kpercoutlet.
                for (m = 1; m <= nlz; m++)
                {
                    lzSMA[m]->areafactor +=
                        uzSMA[k]->areafactor * uzSMA[k]->kperc[m];
                }
            }                   //end loop over upper zones for initialization
        }                       //end if (infopt == 2) END SMA initialization
    }                           //end if infopt > 0

    //if channels are simulated
    if (chnopt > 0)
    {
        //if channel transmission loss is simulated
        if (ctlopt > 0)
        {
            //Allocate memory for transmission loss parameters
            //Note: translossdepth is allocated in ReadTransmissionLossFile....
            translossrate = (float **) malloc ((nlinks + 1) * sizeof (float *));        //transmission loss rate (m/s)
            translossvol = (float **) malloc ((nlinks + 1) * sizeof (float *)); //transmission loss volume (m3)

            //Allocate memory for location (layer) in the channel sediment stack
            nstackch = (int **) malloc ((nlinks + 1) * sizeof (int *)); //sediment stack layer

            //Loop over     number of links
            for (i = 1; i <= nlinks; i++)
            {
                //Allocate remaining memory     for     transmission loss parameters
                translossrate[i] = (float *)
                    malloc ((nnodes[i] + 1) * sizeof (float));
                translossvol[i] = (float *)
                    malloc ((nnodes[i] + 1) * sizeof (float));

                //Allocate remaining memory     for     location (layer) in     the     channel sediment stack
                nstackch[i] = (int *) malloc ((nnodes[i] + 1) * sizeof (int));  //sediment stack layer

                //Loop over     number of nodes
                for (j = 1; j <= nnodes[i]; j++)
                {
                    //Initialize cumulative transmission loss volume (m3)
                    translossvol[i][j] = 0.0;

                    //Initialize channel sediment stack layer (for ksim = 1, nstackov = 1)
                    nstackch[i][j] = 1;

                }               //end loop over nodes

            }                   //end loop over links

            //Initialize total sum of transmission loss volumes
            totaltranslossvol = 0.0;

        }                       //end if     ctlopt > 0

    }                           //end if chnopt > 0

    /*****************************************************************/
    /*        Initialize Global Overland Transport Variables         */
    /*****************************************************************/

    //Allocate memory for overland flow parameters indexed by row and column (and direction)
    //new overland water depth (for next time t+dt) (m)
    hovnew = (float **) malloc ((nrows + 1) * sizeof (float *));
    //net overland flow (m3/s)
    dqov = (float **) malloc ((nrows + 1) * sizeof (float *));
    //gross overland flow into a cell (m3/s)
    dqovin = (float ***) malloc ((nrows + 1) * sizeof (float **));
    //gross overland flow out of a cell (m3/s)
    dqovout = (float ***) malloc ((nrows + 1) * sizeof (float **));
    //cumulative gross overland flow volume into a cell (m3)
    dqovinvol = (float ***) malloc ((nrows + 1) * sizeof (float **));
    //cumulative gross overland flow volume out of a cell (m3)
    dqovoutvol = (float ***) malloc ((nrows + 1) * sizeof (float **));
    //cumulative external flow volume entering a cell (m3)
    qwovvol = (float **) malloc ((nrows + 1) * sizeof (float *));
    //friction slope for overland flow from a cell (m3)
    sfov = (float ***) malloc ((nrows + 1) * sizeof (float **));

    //Loop over number of rows
    for (i = 1; i <= nrows; i++)
    {
        //Allocate additional/remaining memory for overland flow parameters
        hovnew[i] = (float *) malloc ((ncols + 1) * sizeof (float));
        dqov[i] = (float *) malloc ((ncols + 1) * sizeof (float));
        dqovin[i] = (float **) malloc ((ncols + 1) * sizeof (float *));
        dqovout[i] = (float **) malloc ((ncols + 1) * sizeof (float *));
        dqovinvol[i] = (float **) malloc ((ncols + 1) * sizeof (float *));
        dqovoutvol[i] = (float **) malloc ((ncols + 1) * sizeof (float *));
        qwovvol[i] = (float *) malloc ((ncols + 1) * sizeof (float));
        sfov[i] = (float **) malloc ((ncols + 1) * sizeof (float *));

        //Loop over number of columns
        for (j = 1; j <= ncols; j++)
        {
            //if the cell is in the domain
            if (imask[i][j] > 0)
            {
                //Initialize cumulative external overland flow volume array
                qwovvol[i][j] = 0.0;

                //Allocate remaining memory for gross overland flow parameters
                dqovin[i][j] = (float *) malloc (11 * sizeof (float));
                dqovout[i][j] = (float *) malloc (11 * sizeof (float));
                dqovinvol[i][j] = (float *) malloc (11 * sizeof (float));
                dqovoutvol[i][j] = (float *) malloc (11 * sizeof (float));
                sfov[i][j] = (float *) malloc (11 * sizeof (float));

                //loop over flow sources
                for (k = 0; k <= 10; k++)
                {
                    //Initialize cumulative overland flow volumes
                    dqovinvol[i][j][k] = 0.0;
                    dqovoutvol[i][j][k] = 0.0;

                    //initialize overland friction slope
                    sfov[i][j][k] = 0.0;

                }               //end loop over flow sources

            }                   //end if imask[i][j] is not nodatavalue (cell is is domain)

        }                       //end loop over columns

    }                           //end loop over rows

    //Initialize cumulative sum of external flow volume entering overland plane (m3)
    totalqwov = 0.0;

    /****************************************************************/
    /*        Initialize Global Channel Transport Variables         */
    /****************************************************************/

    //if channels are simulated
    if (chnopt == 1)
    {
        //Initialize scalars...
        //
        //Initialize maximum courant number for flow in channel network
        maxcourant = 0.0;

        //Initialize channel network arrays indexed by link and node (and direction)
        //
        //Allocate memory for channel flow parameters

        //new channel water depth (for next time t+dt) (m)
        hchnew = (float **) malloc ((nlinks + 1) * sizeof (float *));
        //net nodal flow (for next time t+dt) (m3/s)
        dqch = (float **) malloc ((nlinks + 1) * sizeof (float *));
        //gross channel flow into a node (m3/s)
        dqchin = (float ***) malloc ((nlinks + 1) * sizeof (float **));
        //gross channel flow out of a node (m3/s)
        dqchout = (float ***) malloc ((nlinks + 1) * sizeof (float **));
        //cumulative gross channel flow volume into a node (m3)
        dqchinvol = (float ***) malloc ((nlinks + 1) * sizeof (float **));
        //cumulative gross channel flow volume out of a node (m3)
        dqchoutvol = (float ***) malloc ((nlinks + 1) * sizeof (float **));
        //cumulative external flow volume entering a node (m3)
        qwchvol = (float **) malloc ((nlinks + 1) * sizeof (float *));
        //link inflow from downstream boundary (m3/s)
        qinch = (float *) malloc ((nlinks + 1) * sizeof (float));
        //cumulative flow volume entering domain from a link (m3)
        qinchvol = (float *) malloc ((nlinks + 1) * sizeof (float));
        //link outflow to downstream boundary (m3/s)
        qoutch = (float *) malloc ((nlinks + 1) * sizeof (float));
        //cumulative flow volume leaving domain from a link (m3)
        qoutchvol = (float *) malloc ((nlinks + 1) * sizeof (float));
        //friction slope for channel flow from a node (m3)
        sfch = (float **) malloc ((nlinks + 1) * sizeof (float *));

        //Loop over number of links
        for (i = 1; i <= nlinks; i++)
        {
            //Initialize link inflow volume (m3)
            qinchvol[i] = 0.0;

            //Initialize link outflow volume (m3)
            qoutchvol[i] = 0.0;

            //Allocate additional/remaining memory for channel flow parameters
            hchnew[i] = (float *) malloc ((nnodes[i] + 1) * sizeof (float));
            dqch[i] = (float *) malloc ((nnodes[i] + 1) * sizeof (float));
            dqchin[i] =
                (float **) malloc ((nnodes[i] + 1) * sizeof (float *));
            dqchout[i] =
                (float **) malloc ((nnodes[i] + 1) * sizeof (float *));
            dqchinvol[i] =
                (float **) malloc ((nnodes[i] + 1) * sizeof (float *));
            dqchoutvol[i] =
                (float **) malloc ((nnodes[i] + 1) * sizeof (float *));
            qwchvol[i] = (float *) malloc ((nnodes[i] + 1) * sizeof (float));
            sfch[i] = (float *) malloc ((nnodes[i] + 1) * sizeof (float));

            //Loop over number of nodes
            for (j = 1; j <= nnodes[i]; j++)
            {
                //Initialize cumulative external channel flow volume array
                qwchvol[i][j] = 0.0;

                //initialize channel friction slope
                sfch[i][j] = 0.0;

                //Allocate remaining memory for gross nodal inflow and outflow
                dqchin[i][j] = (float *) malloc (11 * sizeof (float));
                dqchout[i][j] = (float *) malloc (11 * sizeof (float));
                dqchinvol[i][j] = (float *) malloc (11 * sizeof (float));
                dqchoutvol[i][j] = (float *) malloc (11 * sizeof (float));

                //loop over flow sources
                for (k = 0; k <= 10; k++)
                {
                    //Initialize cumulative channel flow volumes
                    dqchinvol[i][j][k] = 0.0;
                    dqchoutvol[i][j][k] = 0.0;

                }               //end loop over flow sources

            }                   //end loop over nodes

        }                       //end loop over links

        //Initialize cumulative sum of external flow volume entering channel network (m3)
        totalqwch = 0.0;

        //Initialize cumulative sum of flow volume entering channel network (m3)
        totalqinch = 0.0;

        //Initialize cumulative sum of flow volume exiting channel network (m3)
        totalqoutch = 0.0;

    }                           //end if chnopt = 1

    /*************************************************************/
    /*        Initialize Global Reporting Station Arrays         */
    /*************************************************************/

    //Allocate memory for reported water discharges (export) time series

    //reported sum of overland and channel water discharge (units vary)
    qreportsum = (float *) malloc ((nqreports + 1) * sizeof (float));
    //reported overland water discharge (units vary)
    qreportov = (float *) malloc ((nqreports + 1) * sizeof (float));
    //reported channel water discharge (units vary)
    qreportch = (float *) malloc ((nqreports + 1) * sizeof (float));
    //discharge export conversion factor
    qconvert = (float *) malloc ((nqreports + 1) * sizeof (float));

    //Loop over number of flow reporting stations
    for (i = 1; i <= nqreports; i++)
    {
        //Initialize flow reporting variables
        qreportsum[i] = 0.0;
        qreportov[i] = 0.0;
        qreportch[i] = 0.0;

    }                           //end loop over flow reporting stations

    /***************************************************/
    /*        Initialize Outlet Cell Variables         */
    /***************************************************/

    //Allocate memory for overland outlet cell flows

    //overland inflow entering domain from an outlet (m3/s)
    qinov = (float *) malloc ((noutlets + 1) * sizeof (float));
    //overland outflow exiting domain from an outlet (m3/s)
    qoutov = (float *) malloc ((noutlets + 1) * sizeof (float));
    //cumulative flow volume leaving outlet i (m3)
    qoutovvol = (float *) malloc ((noutlets + 1) * sizeof (float));
    //peak flow leaving outlet i (m3/s)
    qpeakov = (float *) malloc ((noutlets + 1) * sizeof (float));
    //time of peak flow leaving outlet i (hrs)
    tpeakov = (float *) malloc ((noutlets + 1) * sizeof (float));

    //Loop over number of outlet cells
    for (i = 1; i <= noutlets; i++)
    {
        //Initialize overland outlet inflor to zero
        qinov[i] = 0.0;

        //Initialize overland outlet exit flow to zero
        qoutov[i] = 0.0;

        //Initialize cumulative overland outlet flow to zero
        qoutovvol[i] = 0.0;

        //Initialize peak outlet flow to zero
        qpeakov[i] = 0.0;

        //Initialize time of peak outlet flow to zero
        tpeakov[i] = 0.0;

    }                           //end loop over outlets

    //Initialize cumulative sum of flow volume exiting overland plane (m3)
    totalqoutov = 0.0;

    //if channels are not simulated
    if (chnopt == 0)
    {
        //Set the outlet routing option
        outopt = 0;
    }
    else                        //channel are simulated
    {
        //Allocate memory for channel outlet flows (defined by outlet)

        //peak flow leaving outlet i (m3/s)
        qpeakch = (float *) malloc ((noutlets + 1) * sizeof (float));
        //time of peak flow leaving outlet i (hrs)
        tpeakch = (float *) malloc ((noutlets + 1) * sizeof (float));

        //Loop over number of outlet cells
        for (i = 1; i <= noutlets; i++)
        {
            //Initialize peak channel outlet flow to zero
            qpeakch[i] = 0.0;

            //Initialize time of peak channel outlet flow to zero
            tpeakch[i] = 0.0;

        }                       //end loop over outlets

    }                           //end if chnopt == 0

    /***************************************************************/
    /*        Initialize Forcing Function and BC Variables         */
    /***************************************************************/
    //
    //Include forcing functions (rainfall, external flows, loads) and boundary conditions
    //
    //Allocate memory for rainfall function arrays

    //next (upper) interval time for rainfall intensity time series
    nrft = (float *) malloc ((nrg + 1) * sizeof (float));
    //prior (lower) interval time for rainfall intensity time series
    prft = (float *) malloc ((nrg + 1) * sizeof (float));
    //intercept for rainfall intensity interpolation
    brf = (float *) malloc ((nrg + 1) * sizeof (float));
    //slope for rainfall intensity interpolation
    mrf = (float *) malloc ((nrg + 1) * sizeof (float));
    //pointer (to position in time series) for rainfall intensity
    rfpoint = (int *) malloc ((nrg + 1) * sizeof (int));
    //rainfall intensity interpolated in time for gage [i]
    rfinterp = (float *) malloc ((nrg + 1) * sizeof (float));

    //Allocate memory for overland external flow (point source) function arrays

    //next (upper) interval time for external overland flow time series
    nqwovt = (float *) malloc ((nqwov + 1) * sizeof (float));
    //prior (lower) interval time for external overland flow time series
    pqwovt = (float *) malloc ((nqwov + 1) * sizeof (float));
    //intercept for external overland flow interpolation
    bqwov = (float *) malloc ((nqwov + 1) * sizeof (float));
    //slope for external overland flow interpolation
    mqwov = (float *) malloc ((nqwov + 1) * sizeof (float));
    //pointer (to position in time series) for external overland flows
    qwovpoint = (int *) malloc ((nqwov + 1) * sizeof (int));
    //external overland flow interpolated in time for source [i]
    qwovinterp = (float *) malloc ((nqwov + 1) * sizeof (float));

    //Allocate memory for boundary condition (BC) water depth function arrays

    //next (upper) interval time for water depth BC time series
    nhbct = (float *) malloc ((noutlets + 1) * sizeof (float));
    //prior (lower) interval time for water depth BC  time series
    phbct = (float *) malloc ((noutlets + 1) * sizeof (float));
    //intercept for water depth BC interpolation
    bhbc = (float *) malloc ((noutlets + 1) * sizeof (float));
    //slope for water depth BC interpolation
    mhbc = (float *) malloc ((noutlets + 1) * sizeof (float));
    //pointer (to position in time series) for water depth BC
    hbcpoint = (int *) malloc ((noutlets + 1) * sizeof (int));
    //water depth BC interpolated in time for outlet[i]
    hbcinterp = (float *) malloc ((noutlets + 1) * sizeof (float));

    //if channels are simulated
    if (chnopt == 1)
    {
        //Allocate memory for external channel flow function arrays

        //next (upper) interval time for external channel flow time series
        nqwcht = (float *) malloc ((nqwch + 1) * sizeof (float));
        //prior (lower) interval time for external channel flow time series
        pqwcht = (float *) malloc ((nqwch + 1) * sizeof (float));
        //intercept for external channel flow interpolation
        bqwch = (float *) malloc ((nqwch + 1) * sizeof (float));
        //slope for external channel flow interpolation
        mqwch = (float *) malloc ((nqwch + 1) * sizeof (float));
        //pointer (to position in time series) for external channel flows
        qwchpoint = (int *) malloc ((nqwch + 1) * sizeof (int));
        //external channel flow interpolated in time for source[i]
        qwchinterp = (float *) malloc ((nqwch + 1) * sizeof (float));

    }                           //end if chhnopt = 1

    /******************************************/
    /*        Initialize Export Files         */
    /******************************************/

    //Set the water export file pointer to null
    waterexpfile_fp = NULL;

    //Set water export file pointer
    waterexpfile_fp = fopen (waterexpfile, "w");

    //if the water export file pointer is null, abort execution
    if (waterexpfile_fp == NULL)
    {
        //Write message to screen
        printf ("Can't create Water Export File: %s \n", waterexpfile);
        exit (EXIT_FAILURE);    //abort
    }

    //Write header labels to water export file...
    //
    //Write initial part of label
    fprintf (waterexpfile_fp, "Time (date-time)");

    fprintf (waterexpfile_fp, ",Time (hours)");

    //Loop over number of flow reporting stations
    for (i = 1; i <= nqreports; i++)
    {
        //Write intermediate part of label
        fprintf (waterexpfile_fp, ",Qov%d", i);

        //Write intermediate part of label
        fprintf (waterexpfile_fp, ",Qch%d", i);

        //Write intermediate part of label
        fprintf (waterexpfile_fp, ",Qsum%d ", i);

        //Check flow reporting units option (in reverse order)...
        //
        //if qunitsopt = 2, discharge reported in mm/hr
        if (qunitsopt[i] == 2)
        {
            //Write intermediate part of label
            fprintf (waterexpfile_fp, "(mm/hr)");

            //Set conversion factor to report flow in desired units
            qconvert[i] = (float) (1000.0 * 3600.0 / (w * w));  //m3/s * qconvert gives mm/hr
        }
        else                    //else discharge reported in m3/s
        {
            //Write intermediate part of label
            fprintf (waterexpfile_fp, "(m3/s)");

            //Set conversion factor to report flow in desired units
            qconvert[i] = 1.0;

        }                       //end if qunitsopt = 2

    }                           //end loop over flow reporting stations

    //Write final part of label
    fprintf (waterexpfile_fp, "\n");

    //Close water export file
    fclose (waterexpfile_fp);

    //Write Headers to the SMA States file
    if (infopt == 2)
    {
        //Set the SMA output file pointer to null
        SMAstatefile_fp = NULL;

        //open the water export file for writing in append mode
        SMAstatefile_fp = fopen (SMAstatefile, "w");

        //if the SMA output file pointer is null, write to stderr 
        if (SMAstatefile_fp == NULL)
        {
            //Write message to screen
            SMAstatefile_fp = stderr;
            printf ("Can't open SMA output File:  %s\n", SMAstatefile);
            printf ("printing to stderr\n");
            //exit (EXIT_FAILURE);    //abort
        }

        //For first time, print headers
        fprintf (SMAstatefile_fp, "DATETIME");
        fprintf (SMAstatefile_fp, ", TIME");
        fprintf (SMAstatefile_fp, ", STORM");
        for (k = 1; k <= nuz; k++)
        {
            fprintf (SMAstatefile_fp, ", uztwratio%d", k);
            for (h = 1; h <= uzSMA[k]->nparts; h++)
            {
                fprintf (SMAstatefile_fp, ", uzfwratio%d_%d", k, h);
            }
        }
        for (k = 1; k <= nlz; k++)
        {
            fprintf (SMAstatefile_fp, ", lztwratio%d", k);
            for (h = 1; h <= lzSMA[k]->nparts; h++)
            {
                fprintf (SMAstatefile_fp, ", lzfwratio%d_%d", k, h);
            }
        }
        for (k = 1; k <= nuz; k++)
          //Individual upper zone flows
        {
            fprintf (SMAstatefile_fp, ", etdemand%d, et%d, et_deep%d", k, k, k);
            for (h = 1; h <= uzSMA[k]->nparts; h++)
            {
                fprintf (SMAstatefile_fp, ", intf%d_%d", k, h);
            }
        }
        for (k = 1; k <= nlz; k++)
          //Individual lower zone flows
        {
            fprintf (SMAstatefile_fp, ", percdemand%d", k);
            for (h = 1; h <= lzSMA[k]->nparts; h++)
            {
                fprintf (SMAstatefile_fp, ", basf%d_%d", k, h); 
            }
        }
        for (k = 1; k <= nio; k++)
        {
            fprintf (SMAstatefile_fp, ", %di_%dj_io%d_wnew",
                     ioSMA[k]->intfout_i, ioSMA[k]->intfout_j, k);
            fprintf (SMAstatefile_fp, ", %di_%dj_dqchin%d",
                     ioSMA[k]->intfout_i, ioSMA[k]->intfout_j, k);
        }
        for (k = 1; k <= nuz; k++)
        {
            fprintf (SMAstatefile_fp, ", uz_wnew%d, uztwc%d, uztwm%d", k, k,
                     k);
            for (h = 1; h <= uzSMA[k]->nparts; h++)
            {
                fprintf (SMAstatefile_fp, ", uzfwc%d_%d, uzfwm%d_%d", k, h, k,
                         h);
            }
        }
        for (k = 1; k <= nlz; k++)
        {
            fprintf (SMAstatefile_fp, ", lz_wnew_perc%d, lztwc%d, lztwm%d", k,
                     k, k);
            for (h = 1; h <= lzSMA[k]->nparts; h++)
            {
                fprintf (SMAstatefile_fp, ", lzfwc%d_%d, lzfwm%d_%d", k, h, k,
                         h);
            }
        }

        //print a carriage return after each complete line
        fprintf (SMAstatefile_fp, "\n");

        //Close water export file
        fclose (SMAstatefile_fp);



    }

    /************************************************************/
    /*        Initialize Min and Max Statistics Variables       */
    /************************************************************/

    //Initialize minimum and maximum rainfall intensities
    maxrainintensity = -9e30;   //maximum rainfall intensity (m/s)
    minrainintensity = 9e30;    //minimum rainfall intensity (m/s)

    //Initialize minimum and maximum overland water depths
    maxhovdepth = -9e30;        //maximum water depth in the overland plane (m)
    minhovdepth = 9e30;         //minimum water depth in the overland plane (m)

    //if infiltration is simulated (infopt > 0)
    if (infopt > 0)
    {
        //Initialize minimum and maximum infiltration water depths
        maxinfiltdepth = -9e30; //maximum cumulative infiltration depth (overland plane) (m)
        mininfiltdepth = 9e30;  //cumulative infiltration depth (overland plane) (m)

    }                           //end if infopt > 0

    //if channels are simulated (chnopt > 0)
    if (chnopt > 0)
    {
        //Initialize minimum and maximum channel water depths
        maxhchdepth = -9e30;    //maximum water depth in the channel network (m)
        minhchdepth = 9e30;     //minimum water depth in the channel network (m)

        //if channel transmission loss is simulated (ctlopt > 0)
        if (ctlopt > 0)
        {
            //Initialize minimum and maximum transmission loss water depths
            maxtranslossdepth = -9e30;  //maximum cumulative transmission loss depth (channel network) (m)
            mintranslossdepth = 9e30;   //minimum cumulative transmission loss depth (channel network) (m)

        }                       //end if ctlopt > 0

    }                           //end if chnopt > 0

//End of function: Return to Initialize
}
