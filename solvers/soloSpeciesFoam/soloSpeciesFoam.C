/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    pisoFoam

Group
    grpIncompressibleSolvers

Description
    Transient solver for incompressible, turbulent flow, using the PISO
    algorithm.

    \heading Solver details
    The solver uses the PISO algorithm to solve the continuity equation:

        \f[
            \div \vec{U} = 0
        \f]

    and momentum equation:

        \f[
            \ddt{\vec{U}} + \div \left( \vec{U} \vec{U} \right) - \div \gvec{R}
          = - \grad p
        \f]

    Where:
    \vartable
        \vec{U} | Velocity
        p       | Pressure
        \vec{R} | Stress tensor
    \endvartable

    Sub-models include:
    - turbulence modelling, i.e. laminar, RAS or LES
    - run-time selectable MRF and finite volume options, e.g. explicit porosity

    \heading Required fields
    \plaintable
        U       | Velocity [m/s]
        p       | Kinematic pressure, p/rho [m2/s2]
        \<turbulence fields\> | As required by user selection
    \endplaintable

\*---------------------------------------------------------------------------*/

// Original include directives for pisoFoam
#include "fvCFD.H"
#include "singlePhaseTransportModel.H"
#include "turbulentTransportModel.H"
#include "pisoControl.H"
#include "fvOptions.H"

// Extra include directives from reactingFoam to create
// a multicomponent mixture and a liquid diffusion-reaction
// equation

// Necessary to create the psiReactionThermo object
#include "turbulentFluidThermoModel.H"

// Class that contains the composition variable to define
// multicomponent mixtures
#include "psiReactionThermo.H"

// A combustion model is necessary to initialise the
// reaction object
#include "CombustionModel.H"

// Required for time stepping
#include "multivariateScheme.H"

// Local timestep for Chemical Reactions
#include "localEulerDdtScheme.H"

// SMOOTH
#include "fvcSmooth.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "Transient solver for incompressible, turbulent flow in porous media with multiple species mass transfer and electrochemical reactions"
    );

    #include "postProcess.H"

    #include "addCheckCaseOptions.H"
    #include "setRootCaseLists.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "createControl.H"
    #include "createPorousMedia.H" // add for porous media
    #include "createFields.H"
    #include "initContinuityErrs.H"


    // imported from ReactingFoam
    #include "createFieldRefs.H"
    #include "createTimeControls.H"// add for adjustTimeStep

    // Setting the initial time-step requires  the definition
    // of the Courant Number
   
    // imported from ReactingFoam
    #include "localEulerDdtScheme.H"

    turbulence->validate();

    // #include "CourantNo.H"
    // #include "setInitialDeltaT.H"



    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.loop())
    {

        #include "CourantNo.H"
        #include "setDeltaT.H"
        
        //++runTime;

        Info<< "Time = " << runTime.timeName() << nl << endl;

	    //#include "setDeltaT.H"// add for adjustTimeStep

        // Update settings from the control dictionary
        // piso.read();

        // Pressure-velocity PISO corrector
        {
            // Momentum conservation 
            //#include "UEqn.H"

            // Multicomponent advection diffusion
            // reaction equation based on ?? fractions
            #include "YEqn.H"

            // --- PISO loop
            //while (piso.correct())
            //{
            //    #include "pEqn.H"
            //}
        }

        laminarTransport.correct();
        turbulence->correct();

        runTime.write();

        runTime.printExecutionTime(Info);
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
