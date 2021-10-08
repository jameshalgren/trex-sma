/*---------------------------------------------------------------------
C-  Function:	ChannelChemicalPartitioning.c
C-
C-	Purpose/	Compute the partitioning of chemicals in the channel
C-	Methods:	network.  Partitioning determines all chemical phases:
C-              dissolved, bound, and particulate.
C-
C-
C-  Inputs:	    advinflowch[][][], advoutflowch[][][]
C-
C-  Outputs:    fparticulatech[chem][solid][link][node][layer]
C-              fboundch[chem][link][node][layer],
C-              fdissolvedch[chem][link][node][layer]
C-
C-  Controls:	partopt[]
C-
C-  Calls:      None
C-
C-  Called by:  ChemicalTransport
C-
C-	Created:	Mark Velleux
C-				Department of Civil Engineering
C-				Colorado State University
C-				Fort Collins, CO 80523
C-
C-				John F. England, Jr.
C-				Bureau of Reclamation
C-				Flood Hydrology Group, D-8530
C-				Bldg. 67, Denver Federal Center, Denver, CO 80225
C-
C-	Date:		12-JUL-2004
C-
C-	Revised:
C-
C-	Date:	
C-
C-	Revisions:
C-
C----------------------------------------------------------------------*/

//trex global variable declarations
#include "trex_general_declarations.h"

//trex global variable declarations for water transport
#include "trex_water_declarations.h"

//trex global variable declarations for sediment transport
#include "trex_solids_declarations.h"

//trex global variable declarations for chemical transport
#include "trex_chemical_declarations.h"

//trex global variable declarations for environmental conditions
#include "trex_environmental_declarations.h"

void ChannelChemicalPartitioning(void)
{
    //local variable declarations/definitions
    int ichem,                  //loop index for chemical type
      isolid,                   //loop index for solids (particle) type
      ilayer;                   //loop index for layer in the sediment stack

    float pic0,                 //solids independent partition coefficient (m3/g)
     *pic,                      //solids dependent/independent partition coefficient (m3/g) [isolid]
      fpoc,                     //fraction particulate organic carbon (dimensionless)
     *m,                        //solids partitioning basis (solids or poc concentration) (g/m3 = mg/L)
      sumpicm,                  //sum of pic * m values
      sumfp,                    //overal sum (fraction of total) of chemical sorbed to solids (dimensionless)
      fdoc,                     //fraction dissolved organic carbon (dimensionless) (effective fration doc)
      pib,                      //dissolved organic compound binding coefficient (m3/g) (effective binding coef.)
      b;                        //doc binding basis (doc concentration) (g/m3)

    float densityw,             //density of water (kg/m3)
      densityp,                 //particle density (kg/m3)
      fsolids,                  //volume fraction of solids (volume solids / total volume) (dimensionless)
      wcporosity;               //water column porosity (dimensionless)

    //Allocate memory for local arrays
    pic = (float *) malloc ((nsolids + 1) * sizeof (float));    //partition coefficient (m3/g)
    m = (float *) malloc ((nsolids + 1) * sizeof (float));      //solids partitioning basis (g/m3 = mg/L)

    //Define local constants...
    //
    //set density of water = 1000 kg/m3
    densityw = (float) (1000.0);

    //loop over links
    for (i = 1; i <= nlinks; i++)
    {
        //loop over nodes
        for (j = 1; j <= nnodes[i]; j++)
        {
            //Calculate water column porosity...
            //
            //initialize the volumefraction of solids to zero
            fsolids = 0.0;

            //loop over solids
            for (isolid = 1; isolid <= nsolids; isolid++)
            {
                //set particle density (kg/m3)
                densityp = spgravity[isolid] * densityw;

                //compute volume fraction of solids (sum for all particle types) (dimensionless)
                fsolids =
                    fsolids +
                    csedch[isolid][i][j][0] / (float) (densityp * 1000.0);

            }                   //end loop over solids

            //compute water column porosity (dimensionless)
            wcporosity = (float) (1.0 - fsolids);

            //loop over number of chemical types
            for (ichem = 1; ichem <= nchems; ichem++)
            {
                //if the chemical partitions (partopt > 0)
                if (partopt[ichem] > 0)
                {
                    //Water Column...
                    //
                    //Particulate phases:
                    //
                    //initialize the sum of pic * m (used to compute fparticulate)
                    sumpicm = 0.0;

                    //initialize the sum of all particulate fractions
                    sumfp = 0.0;

                    //loop over number of solids types
                    for (isolid = 1; isolid <= nsolids; isolid++)
                    {
                        //Note:  Partitioning is generalized for organic or
                        //       inorganic chemicals.  Organics partition
                        //       on a cabon normalized basis.  Inorganics
                        //       partition on a solids basis.  This code
                        //       works for either case and also accounts
                        //       for particle dependent partitioning.
                        //
                        //if the chemical partitions to organic carbon
                        if (partopt[ichem] > 1)
                        {
                            //set the fraction (particulate) organic carbon
                            fpoc = fpocch[isolid][i][j][0];

                            //set the solids independent partition coefficient (m3/g)
                            pic0 = koc[ichem];

                            //set the particulate organic carbon (poc) concentration (g/m3 = mg/L))
                            m[isolid] = csedch[isolid][i][j][0] * fpoc;
                        }
                        else    //else the chemical partitions to solids
                        {
                            //set the solids independent partition coefficient (m3/g)
                            pic0 = kp[ichem];

                            //set the solids concentration (g/m3 = mg/L)
                            m[isolid] = csedch[isolid][i][j][0];

                        }       //end if partopt[]

                        //Adjust partition coefficient for porosity
                        //
                        //if the water column porosity > 0.0
                        if (wcporosity > 0.0)
                        {
                            //correct partition coeffificent for porosity
                            pic0 = pic0 / wcporosity;
                        }
                        else    //else wcporosity <= 0
                        {
                            //set the partition coefficient to effective infinity
                            pic0 = (float) (1.0e30);

                        }       //end if wcporosity > 0.0

                        //Note:  For the water column, the particle dependent
                        //       partition coefficient is used...
                        //
                        //Compute the particle dependent partition coefficient (m3/g)
                        pic[isolid] =
                            (float) (pic0 /
                                     (1.0 + m[isolid] * pic0 / nux[ichem]));

                        //compute the sum of pic * m (used to compute fparticulate)
                        sumpicm = sumpicm + pic[isolid] * m[isolid];

                    }           //end loop over solids

                    //Bound phase:
                    //
                    //if the chemical partitions to organic carbon
                    if (partopt[ichem] > 1)
                    {
                        //set the effective fraction of doc for binding
                        fdoc = fdocch[i][j][0];

                        //set the organic binding coefficient (m3/g)
                        pib = koc[ichem];

                        //set the effective dissolved organic compound (doc) concentration (g/m3)
                        b = cdocch[i][j][0] * fdoc;
                    }
                    else        //else the chemical partitions to solids
                    {
                        //set the organic binding coefficient (m3/g)
                        pib = kb[ichem];

                        //set the dissolved organic compound (doc) concentration (g/m3)
                        b = cdocch[i][j][0];

                    }           //end if partopt[] > 1

                    //Adjust binding coefficient for porosity
                    //
                    //if the water column porosity > 0.0
                    if (wcporosity > 0.0)
                    {
                        //correct binding coeffificent for porosity
                        pib = pib / wcporosity;
                    }
                    else        //else wcporosity <= 0
                    {
                        //set the binding coefficient to zero
                        pib = 0.0;

                    }           //end if wcporosity > 0.0

                    //loop over number of solids types
                    for (isolid = 1; isolid <= nsolids; isolid++)
                    {
                        //compute the fraction of the chemical sorbed to this solids type
                        fparticulatech[ichem][isolid][i][j][0] =
                            (float) (pic[isolid] * m[isolid] /
                                     (1.0 + pib * b + sumpicm));

                        //compute sum of all particulate phases
                        sumfp = sumfp
                            + fparticulatech[ichem][isolid][i][j][0];

                    }           //end loop over solids

                    //Note:  Watch out for roundoff errors.  For large
                    //       Kp values, the sum of particulate fractions
                    //       can be > 1.0 (i.e. 1.0000001).  When this
                    //       occurs individual values need to be scaled
                    //       and the sum reset to exactly 1.0.
                    //
                    //if the sum of particulate fractions (sumfp) > 1.0
                    if (sumfp > 1.0)
                    {
                        //loop over number of solids types
                        for (isolid = 1; isolid <= nsolids; isolid++)
                        {
                            //scale the fraction of the chemical sorbed to this solids type
                            fparticulatech[ichem][isolid][i][j][0] =
                                (float) (1.0 / sumfp) *
                                fparticulatech[ichem][isolid][i][j][0];

                        }       //end loop over solids

                        //reset sum of all particulate phases to exactly 1.0
                        sumfp = (float) (1.0);

                    }           //end if sumfp > 1.0

                    //compute the fraction of the chemical sorbed to doc
                    fboundch[ichem][i][j][0] =
                        (float) (pib * b / (1.0 + pib * b + sumpicm));

                    //compute the dissolved fraction of the chemical
                    fdissolvedch[ichem][i][j][0] = (float) (1.0 - sumfp
                                                            -
                                                            fboundch[ichem][i]
                                                            [j][0]);

                    //Sediment Column...
                    //
                    //loop over layers (in reverse order: top down)
                    for (ilayer = nstackch[i][j]; ilayer >= 1; ilayer--)
                    {
                        //Particulate phases:
                        //
                        //initialize the sum of pic * m (used to compute fparticulate)
                        sumpicm = 0.0;

                        //initialize the sum of all particulate fractions
                        sumfp = 0.0;

                        //loop over number of solids types
                        for (isolid = 1; isolid <= nsolids; isolid++)
                        {
                            //Note:  Partitioning is generalized for organic or
                            //       inorganic chemicals.  Organics partition
                            //       on a cabon normalized basis.  Inorganics
                            //       partition on a solids basis.  This code
                            //       works for either case.  Partitioning in
                            //       the sediments is always independent
                            //       of particle concentration.
                            //
                            //if the chemical partitions to organic carbon
                            if (partopt[ichem] > 1)
                            {
                                //set the fraction (particulate) organic carbon
                                fpoc = fpocch[isolid][i][j][ilayer];

                                //set the solids independent partition coefficient (m3/g)
                                pic0 = koc[ichem];

                                //set the particulate organic carbon (poc) concentration (g/m3 = mg/L))
                                m[isolid] =
                                    csedch[isolid][i][j][ilayer] * fpoc;
                            }
                            else        //else the chemical partitions to solids
                            {
                                //set the solids independent partition coefficient (m3/g)
                                pic0 = kp[ichem];

                                //set the solids concentration (g/m3 = mg/L)
                                m[isolid] = csedch[isolid][i][j][ilayer];

                            }   //end if partopt[]

                            //Adjust partition coefficient for porosity
                            //
                            //if the sediment layer porosity > 0.0
                            if (porositych[i][j][ilayer] > 0.0)
                            {
                                //correct partition coeffificent for porosity
                                pic0 = pic0 / porositych[i][j][ilayer];
                            }
                            else        //else porositych[i][j][ilayer] <= 0
                            {
                                //set the partition coefficient to effective infinity
                                pic0 = (float) (1.0e30);

                            }   //end if porositych[i][j][ilayer] > 0.0

                            //Note:  For the sediment column, the particle independent
                            //       partition coefficient is used...
                            //
                            //Compute the (particle independent) partition coefficient (m3/g)
                            pic[isolid] = pic0;

                            //compute the sum of pic * m (used to compute fparticulate)
                            sumpicm = sumpicm + pic[isolid] * m[isolid];

                        }       //end loop over solids

                        //Bound phase:
                        //
                        //if the chemical partitions to organic carbon
                        if (partopt[ichem] > 1)
                        {
                            //set the effective fraction of doc for binding
                            fdoc = fdocch[i][j][ilayer];

                            //set the organic binding coefficient (m3/g)
                            pib = koc[ichem];

                            //set the effective dissolved organic compound (doc) concentration (g/m3 = mg/L)
                            b = cdocch[i][j][ilayer] * fdoc;
                        }
                        else    //else the chemical partitions to solids
                        {
                            //set the organic binding coefficient (m3/g)
                            pib = kb[ichem];

                            //set the dissolved organic compound (doc) concentration (g/m3 = mg/L)
                            b = cdocch[i][j][ilayer];

                        }       //end if partopt[] > 1

                        //loop over number of solids types
                        for (isolid = 1; isolid <= nsolids; isolid++)
                        {
                            //compute the fraction of the chemical sorbed to this solids type
                            fparticulatech[ichem][isolid][i][j][ilayer] =
                                (float) (pic[isolid] * m[isolid] /
                                         (1.0 + pib * b + sumpicm));

                            //compute sum of all particulate phases
                            sumfp = sumfp
                                + fparticulatech[ichem][isolid][i][j][ilayer];

                        }       //end loop over solids

                        //Note:  Watch out for roundoff errors.  For large
                        //       Kp values, the sum of particulate fractions
                        //       can be > 1.0 (i.e. 1.0000001).  When this
                        //       occurs individual values need to be scaled
                        //       and the sum reset to exactly 1.0.
                        //
                        //if the sum of particulate fractions (sumfp) > 1.0
                        if (sumfp > 1.0)
                        {
                            //loop over number of solids types
                            for (isolid = 1; isolid <= nsolids; isolid++)
                            {
                                //scale the fraction of the chemical sorbed to this solids type
                                fparticulatech[ichem][isolid][i][j][ilayer] =
                                    (float) (1.0 / sumfp) *
                                    fparticulatech[ichem][isolid][i][j]
                                    [ilayer];

                            }   //end loop over solids

                            //reset sum of all particulate phases to exactly 1.0
                            sumfp = (float) (1.0);

                        }       //end if sumfp > 1.0

                        //compute the fraction of the chemical sorbed to doc
                        fboundch[ichem][i][j][ilayer] =
                            (float) (pib * b / (1.0 + pib * b + sumpicm));

                        //compute the dissolved fraction of the chemical
                        fdissolvedch[ichem][i][j][ilayer] =
                            (float) (1.0 - sumfp -
                                     fboundch[ichem][i][j][ilayer]);

                    }           //end loop over layers

                }               //end if partopt[] > 0

            }                   //end loop over number of chemical types

        }                       //end loop over nodes

    }                           //end loop over links

    //Deallocate memory for local arrays
    free (pic);
    free (m);

//End of function: Return to ChemicalTransport
}
