/*----------------------------------------------------------------------
C-  Header:     trex_water_definitions.h
C-
C-  Purpose/    Global definitions header file for hydraulic/hydrologic
C-  Methods:    calculations.  Used with trex_water_declarations.h.
C-
C-  Created:    Mark Velleux
C-              Department of Civil Engineering
C-              Colorado State University
C-              Fort Collins, CO  80523
C-
C-              John F. England, Jr.
C-              Bureau of Reclamation
C-              Flood Hydrology Group, D-8530
C-              Bldg. 67, Denver Federal Center
C-              Denver, CO  80225
C-
C-  Date:       17-JUL-2004
C-
C-  Revised:
C-
C-  Date:
C-
C-  Revisions:
C-
C---------------------------------------------------------------------*/

//Data Group B
FILE *maskfile_fp = NULL;  //watershed mask file pointer
FILE *elevationfile_fp = NULL;  //elevation grid file pointer
FILE *soiltypefile_fp = NULL;  //soils grid file pointer
FILE *landusefile_fp = NULL;  //land use grid file pointer
FILE *storagedepthfile_fp = NULL;  //storage depth in overland plane grid file pointer

FILE *linkfile_fp = NULL;  //link map grid file pointer
FILE *nodefile_fp = NULL;  //node map grid file pointer
FILE *channelfile_fp = NULL;  //channel file pointer
FILE *channeltlossfile_fp;  //channel transmission loss propery file pointer

//jfe add this FILE *snow_file_fp;  //initial snow depth on overland plane grid file pointer

FILE *wateroverlandfile_fp = NULL;  //initial water on overland plane grid file pointer
FILE *infiltrationfile_fp = NULL;  //initial water in soils grid file pointer
FILE *waterchannelfile_fp = NULL;  //initial water in channels file pointer
FILE *translossfile_fp;  //initial water in sediments file pointer

FILE *raingridfile_fp;  //rainfall grid file pointer

//Data Group F
FILE *waterexpfile_fp = NULL;  //water export file pointer

/************************************/
/*   GLOBAL VARIABLE DEFINITIONS    */
/************************************/

/**********************/
//  state variables
/**********************/
float
  **hov,  //overland water depth in a grid cell (at current time t) (m)
  **hovnew,  //new overland water depth in a grid cell (for next time t+dt) (m)
  **hch,  //depth of water in a channel (at current time t) (m)
  **hchnew;  //depth of water in a channel (for next time t+dt) (m)

/******************************************************************/
//  hydrologic/hydraulic (ksim = 1) controls, support, reporting
/******************************************************************/

char
  *maskfile,  //Mask file
  *elevationfile,  //Elevation File
  *soiltypefile,  //Soil type file
  *landusefile,  //Land Use Classification File
  *channelfile,  //channel file
  *storagedepthfile,  //Storgage depth file (for grid cells in overland plane)
  *wateroverlandfile,  //Initial Depth of Water for Overland Plane file
  *waterchannelfile,  //Initial Depth of Water for Channel Network file
  *infiltrationfile,  //Initial infiltration depth file
  *translossfile,  //Initial transmission loss depth file
  *linkfile,  //link file
  *nodefile,  //node file
  *topologyfile,  //topology file
  *channeltlossfile,  //channel transmission loss property file

  *raingridfileroot,  //rainfall grid file root name
  *raingridfile,  //rainfall grid file root name (assumed extension = .0, .1, .2, .3 etc.)

  **landname,  //Land use name
  **soilname,  //Soil name

  **qwovdescription,	//Overland flow source description
  **qwchdescription,	//Channel flow source description
  **hbcdescription,	//Outlet boundary condition description

  *waterexpfile,  //water export file
  *rainrategrid,  //rainfall rate grid file
  *raindepthgrid,  //rainfall depth grid file
  *infrategrid,  //infiltration rate grid file
  *infdepthgrid,  //infiltration depth grid file
  *qgrid,  //water discharge (q) grid file
  *waterdepthgrid;  //water depth grid file

int
  noutlets,  //number of outlet cells
  *iout,  //Overland outlet cell row
  *jout,  //Overland outlet cell column
  ncells,  //number of active (non-null) grid cells in the simulation mask
  nodatavalue, //value used to represent a null entry (no data) in a grid file
  **imask,  //Location (row, column) of watershed mask index for each cell in overland plane
  infopt,  //Infiltration Option, 0 = no infiltration, 1 = infiltration
  chnopt,  //Channel Option, 0 = no channels, 1 = channels
  tplgyopt,  //Topology Option, 0 = Compute topology from channel property file
            //and link, and node masks, 1 = Topology read from topology file
  ctlopt,  //Channel Transmission Loss Option, 0 = no transmission loss, 1 = transmission loss
  fldopt,  //floodplain transfer option, 0 = water transferred from overland to channel only,
          //1 = water transfered in either direction between the overland plane and channels
  outopt,  //Outlet Option, 0 = route to channels first, 1 = route overland first

  nsoils,  //Number Of Soil Types
  nlands,  //Number of Land Use Classes
  **nstackov,  //present number of layers in the overland soil stack [row][col] (1 for ksim=1)
  **nstackch,  //present number of layers in the channel sediment stack [link][node] (1 for ksim=1)
	
  ***soiltype,  //Location (row, column, layer) of soil index for each cell in overland plane
  **landuse,  //Location (row, column) of land use index for each cell in overland plane
  **designrainindex,  //Location (row, column) of design rain index for each cell in overland plane

  nlinks,  //Number of links
  *nnodes,  //Number of nodes (for a link) in the channel network
  **link,  //Location (row, column) of channels cells for each link in channel network
  **node,  //Location (row, column) of channels cells for each node of each link in channel network
  **ichnrow,  //row address of channel link and node
  **ichncol,  //column address of channel link and node
  **nupbranches,  //Number of links (branches) upstream at a junction
  **ndownbranches,  //Number of links (branches) downstream at a junction
  ***updirection,  //Direction to the upstream interface of a node in a link
  ***downdirection,  //Direction to the downstream interface of a node of a link
	
  nrg,  //Number Of raingages, 1 = Uniform rain
  rainopt,  //Rainfall spatial interpolation option
  *rgid,  //Raingage id
  *nrpairs,  //Number of pairs of rainfall intensity and time

  raingridcount,  //running count of the number of times rainfall grids are read

  *dbcopt,  //Domain boundary Condition Option, 0 = normal depth BC, 1 = specified water depth
	
  iteration,  //flow direction index for overland routing

  nqreports,  //Number of Flow reports
  nqwov,  //Number of overland flow point sources (external flows)
  nqwch,  //Number of channel flow point sources (external flows)

  *qreprow,  //Row number for flow report
  *qrepcol,  //Column number for flow report
  *qunitsopt,  //Units option for flow report
  *qwovrow,  //Overland flow source row number
  *qwovcol,  //Overland flow source column number
  *nqwovpairs,  //Number of pairs of overland external flow volume and time
  *qwchlink,  //Channel flow source link number
  *qwchnode,  //Channel flow source node number
  *nqwchpairs,  //Number of pairs of channel external flow volume and time
  *nhbcpairs,  //Number of pairs of outlet water depth and time

  *qchoutlet,  //Outlet number associated with a channel network boundary link

  *rfpoint,  //pointer to current position for a rainfall function
  *qwovpoint,  //pointer to current position for an external overland flow function
  *qwchpoint,  //pointer to current position for an external channel flow function
  *hbcpoint;  //pointer to current position for a water depth BC function

float
	xllcorner,		//x location of grid lower left corner (m) (GIS projection)
	yllcorner,		//y location projection of grid lower left corner (m) (GIS projection)
	**elevationov0,	//initial ground surface elevation of each cell in the overland plane (m)
	**elevationov,	//present ground surface elevation of each cell in the overland plane (m)
	**elevationch0,	//initial channel bed elevation of node in channel betwork (m)
	**elevationch,	//present channel bed elevation of node in channel betwork (m)
	*ocx,			//Overland outlet x location (m)
	*ocy,			//Overland outlet y location (m)
	*sovout,		//Overland outlet cell slope

	**grossrainrate,	//gross rainfall rate (m/s)
	**grossraindepth,	//cumulative gross rainfall depth (m)
	**grossrainvol,		//gross rainfall volume entering a cell (m3)
	**netrainrate,		//net rainfall rate (m/s)
	**netrainvol,		//net rainfall volume entering a cell (m3)

	**interceptiondepth,	//remaining (unsatisfied) interception depth (m)
	**interceptionvol,		//cumulative interception volume in a cell (m3)

	**infiltrationrate,		//infiltration rate (m/s)
	**infiltrationdepth,	//cumulative infiltration depth (m)
	**infiltrationvol,	//cumulative infiltration volume in a cell (m3)
        **SMAinfiltrationvol, 	//value used to connect the SMA procedure
                //with the existing infiltration computation
        **SMAprecipvol, 	//value used to trigger reinitialization of the
                //infiltration parameters using SMA states. 

	**translossrate,	//transmission loss rate (m/s)
	**translossdepth,	//cumulative transmission loss depth (m)
	**translossvol,		//cumulative transmission loss volume in a node (m3)

	**storagedepth,		//depression storage depth in overland plane
	
	*nmanningov,		//Manning's n value in the overland plane
	*interceptionclass,	//interception depth for a land use class

	*khsoil,			//Hydraulic Conductivity of soil
	*capshsoil,			//Capillary Suction Head of soil
	*soilmd,			//Soil Moisture Deficit

	**khsed,			//Hydraulic Conductivity of sediment
	**capshsed,			//Capillary Suction Head of sediment
	**sedmd,			//Sediment Moisture Deficit

	*rgx,				//Raingage x coordinate (m)
	*rgy,				//Raingage y coordinate (m)
	**rfintensity,		//Rainfall intensity
	**rftime,			//Rainfall time break (hours)
	*rfinterp,			//rainfall intensity interpolated in time
	idwradius,			//radius of influence for IDW spatial interpolation
	idwexponent,		//weighting exponent for IDW spatial interpolation
	raingridfreq,		//frequency at which rainfall grids are read from file (hours)
	rainconvunits,		//rainfall grid conversion factors for length, area, volume or mass units
	rainconvtime,		//rainfall grid conversion factor for time units
	rainscale,			//rainfall grid scale factor
	timeraingrid,		//time at which the next rainfall grid is read from file

	**twidth,			//top width at bank height
	**bwidth,			//bottom width
	**sideslope,		//sideslope
	**hbank,			//channel bank height of channel (distance from bed to top of bank)
	**nmanningch,		//manning's n roughness coefficient for channels
	**sinuosity,		//channel sinuosity
	**deadstoragedepth,	//channel dead storage depth (m)
	**chanlength,		//length of channel element (m) (link/node)
	**lengthup,			//upstream half length of channel element (m) (link/node)
	**lengthdown,		//downstream half length of channel element (m) (link/node)

	***sfov,		//friction slope of an overland cell (dimensionless) [row][col][direction: N-S, E-W]
	**sfch,			//friction slope of a channel node (dimensionless) [link][node]

	maxcourant,		//maximum courant number computed in channelroute

	**dqov,			//net overland flow (m3/s)
	***dqovin,		//gross overland flow into a cell (m3/s) [r][c][direction]
	***dqovinvol,	//cumulative gross overland flow volume into a cell (m3) [r][c][direction]
	***dqovout,		//gross overland flow out of a cell (m3/s) [r][c][direction]
	***dqovoutvol,	//cumulative gross overland flow volume out of a cell (m3) [r][c][direction]
	**dqch,			//net channel flow at a node (m3/s)
	***dqchin,		//gross channel flow into a node (m3/s) [l][n][direction]
	***dqchinvol,	//cumulative gross channel flow volume into a node (m3) [l][n][direction]
	***dqchout,		//gross channel flow out of a node (m3/s) [l][n][direction]
	***dqchoutvol,	//cumulative gross channel flow volume out of a node (m3) [l][n][direction]

	*qinov,				//overland inflow entering domain from an outlet (m3/s)
	*qoutov,			//overland outflow exiting domain from an outlet (m3/s)
	*qoutovvol,			//cumulative flow volume exiting an overland outlet (m3)
	*qinch,				//channel flow entering a channel link (at a downstream junction) (m3/s)
	*qinchvol,			//cumulative flow volume entering a channel link (m3)
	*qoutch,			//channel flow exiting a channel link (at a downstream junction) (m3/s)
	*qoutchvol,			//cumulative flow volume exiting a channel link (m3)

	**qwovvol,			//cumulative external flow volume entering a cell (m3)
	**qwchvol,			//cumulative external flow volume entering a node (m3)
	**qwov,				//Overland external flow volume
	*qwovinterp,		//Overland external flow interpolated in time
	**qwovtime,			//Overland external flow volume time break (hours)

	**qwch,				//Channel external flow volume
	*qwchinterp,		//Channel external flow interpolated in time
	**qwchtime,			//Channel external flow volume time break (hours)

	**hbc,				//Outlet water depth (m) 
	*hbcinterp,			//Outlet water depth interpolated in time (m)
	**hbctime,			//Outlet water depth time break (hours)

	ntr,		//update time for rainfall functions
	ntqwov,		//update time for overland flow functions
	ntqwch,		//update time for channel flow functions
	nthbc,		//update time for outler water depth functions

	*nrft,		//next (upper) interval time for rainfall intensity time series
	*prft,		//prior (lower) interval time for rainfall intensity time series
	*brf,		//intercept for rainfall intensity interpolation
	*mrf,		//slope for rainfall intensity interpolation

	*nqwovt,	//next (upper) interval time for external overland flow time series
	*pqwovt,	//prior (lower) interval time for external overland flow time series
	*bqwov,		//intercept for external overland flow interpolation
	*mqwov,		//slope for external overland flow interpolation

	*nqwcht,	//next (upper) interval time for external channel flow time series
	*pqwcht,	//prior (lower) interval time for external channel flow time series
	*bqwch,		//intercept for external channel flow interpolation
	*mqwch,		//slope for external channel flow interpolation

	*nhbct,		//next (upper) interval time for outlet water depth time series
	*phbct,		//prior (lower) interval time for outlet water depth time series
	*bhbc,		//intercept for outlet water depth interpolation
	*mhbc,		//slope for outlet water depth interpolation

	*qarea,				//Area of a watershed basin at a reporting station for flow report (m3)
	*qreportsum,		//Sum of overland and channel discharge at a flow reporting station (m3/s)
	*qreportov,			//Overland discharge at a flow reporting station (m3/s)
	*qreportch,			//Channel discharge at a flow reporting station (m3/s)
	*qconvert,			//Discharge export time series reporting conversion factor		

	**initialwaterov,	//initial water volume in a overland plane cell (m3)
	**initialwaterch,	//initial water volume in a channel node (m3)
	
	initialwaterovvol,	//cumulative initial water volume in the overland plane (m3)
	initialwaterchvol,	//cumulative initial water volume in the channel network (m3)
	
	**finalwaterov,		//final water volume in a overland plane cell (m3)
	**finalwaterch,		//final water volume in a channel node (m3)
	finalwaterovvol,	//cumulative final water volume in the overland plane (m3)
	finalwaterchvol,	//cumulative final water volume in the channel network (m3)
	
	*qpeakov,			//Peak overland flow at an outlet (m3/s)
	*tpeakov,			//Time of peak overland flow at an outlet (hrs)
	*qpeakch,			//Peak channel flow at an outlet (m3/s)
	*tpeakch,			//Time of peak channel flow at an outlet (hrs)

	totalgrossrainvol,	//cumulative sum of gross rainfall volumes entering domain (m3)
	totalnetrainvol,	//cumulative sum of net rainfall volumes entering domain (m3)
	totalinterceptvol,	//cumulative sum of interception volumes in domain (m3)
	totalinfiltvol,		//cumulative sum of infiltration volumes in domain (m3)
	totaltranslossvol,	//cumulative sum of transmission loss volumes in domain (m3)
	
	totalqoutov,		//cumulative sum of flow volume exiting overland plane (m3)
	totalqinch,			//cumulative sum of flow volume entering channel network (m3)
	totalqoutch,		//cumulative sum of flow volume exiting channel network (m3)
	totalqwov,			//cumulative sum of external flow volume entering overland plane (m3)
	totalqwch;			//cumulative sum of external flow volume entering channel network (m3)

double 
	maxhovdepth,	//maximum water depth in overland plane
	minhovdepth,	//minimum water depth in overland plane
	maxhchdepth,	//maximum water depth in channels
	minhchdepth,	//minimum water depth in channels
	maxinfiltdepth,	//maximum infiltration depth (cumulative)
	mininfiltdepth,	//minimum infiltration depth (cumulative)
	maxtranslossdepth,	//maximum transmission loss depth (cumulative)
	mintranslossdepth,	//minimum transmission loss depth (cumulative)
	maxrainintensity,	//maximum rainfall intensity (m/s)
	minrainintensity;	//minimum rainfall intensity (m/s)
