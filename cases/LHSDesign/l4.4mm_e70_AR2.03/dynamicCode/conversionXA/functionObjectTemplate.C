/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2019-2021 OpenCFD Ltd.
    Copyright (C) YEAR AUTHOR, AFFILIATION
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

\*---------------------------------------------------------------------------*/

#include "functionObjectTemplate.H"
#define namespaceFoam  // Suppress <using namespace Foam;>
#include "fvCFD.H"
#include "unitConversion.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(conversionXAFunctionObject, 0);

addRemovableToRunTimeSelectionTable
(
    functionObject,
    conversionXAFunctionObject,
    dictionary
);


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

// dynamicCode:
// SHA1 = f0d04d2df675c4f85b1223e113866fd6ea194762
//
// unique function name that can be checked if the correct library version
// has been loaded
extern "C" void conversionXA_f0d04d2df675c4f85b1223e113866fd6ea194762(bool load)
{
    if (load)
    {
        // Code that can be explicitly executed after loading
    }
    else
    {
        // Code that can be explicitly executed before unloading
    }
}


// * * * * * * * * * * * * * * * Local Functions * * * * * * * * * * * * * * //

//{{{ begin localCode

//}}} end localCode

} // End namespace Foam


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

const Foam::fvMesh&
Foam::conversionXAFunctionObject::mesh() const
{
    return refCast<const fvMesh>(obr_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::
conversionXAFunctionObject::
conversionXAFunctionObject
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    functionObjects::regionFunctionObject(name, runTime, dict)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::
conversionXAFunctionObject::
~conversionXAFunctionObject()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool
Foam::
conversionXAFunctionObject::read(const dictionary& dict)
{
    if (false)
    {
        printMessage("read conversionXA");
    }

//{{{ begin code
    
//}}} end code

    return true;
}


bool
Foam::
conversionXAFunctionObject::execute()
{
    if (false)
    {
        printMessage("execute conversionXA");
    }

//{{{ begin code
    #line 67 "/home/lucasortizalfaro/rfb-model/cases/LHSDesign/l5.7mm_e58_AR0.99/system/controlDict/functions/conversionXA"
const volVectorField& U = mesh().lookupObject<volVectorField>("U");
                const volScalarField UZ(U.component(vector::Z));
                const volScalarField& A = mesh().lookupObject<volScalarField>("A");

                const label inletPatchID = mesh().boundaryMesh().findPatchID("inlet");
                const label outletPatchID = mesh().boundaryMesh().findPatchID("outlet");

                const scalarField& A_inlet = A.boundaryField()[inletPatchID];
                const scalarField& UZ_inlet = UZ.boundaryField()[inletPatchID];
                const vectorField& Sf_inlet = mesh().Sf().boundaryField()[inletPatchID];

                const scalarField& A_outlet = A.boundaryField()[outletPatchID];
                const scalarField& UZ_outlet = UZ.boundaryField()[outletPatchID];
                const vectorField& Sf_outlet = mesh().Sf().boundaryField()[outletPatchID];

                scalar fluxIn = 0.0;
                scalar fluxOut = 0.0;

                forAll(A_inlet, i)
                {
                    fluxIn += UZ_inlet[i] * A_inlet[i] * mag(Sf_inlet[i]);
                }

                forAll(A_outlet, i)
                {
                    fluxOut += UZ_outlet[i] * A_outlet[i] * mag(Sf_outlet[i]);
                }

                scalar fluxInAbs = mag(fluxIn);
                scalar fluxOutAbs = mag(fluxOut);

                scalar conversion = 0.0;

                if (fluxInAbs > SMALL)
                {
                    conversion = 1.0 - (fluxOutAbs / fluxInAbs);
                }
                else
                {
                    Info << ">>> [WARNING] flujo de entrada muy bajo, conversión forzada a 0" << endl;
                    conversion = 0.0;
                }

                scalar timeVal = mesh().time().value();

                Info << ">>> Conversión X_A = " << conversion << endl;
                Info << "fluxIn = " << fluxIn << ", fluxOut = " << fluxOut << endl;

                std::ifstream checkFile("XA_vs_time.dat");
                bool newFile = !checkFile.good();
                checkFile.close();

                std::ofstream logFile("XA_vs_time.dat", std::ios::app);
                if (logFile.is_open())
                {
                    if (newFile)
                        logFile << "# time\tX_A\tfluxIn\tfluxOut" << std::endl;

                    logFile << timeVal << "\t" << conversion << "\t" << fluxIn << "\t" << fluxOut << std::endl;
                    logFile.close();
                }
//}}} end code

    return true;
}


bool
Foam::
conversionXAFunctionObject::write()
{
    if (false)
    {
        printMessage("write conversionXA");
    }

//{{{ begin code
    
//}}} end code

    return true;
}


bool
Foam::
conversionXAFunctionObject::end()
{
    if (false)
    {
        printMessage("end conversionXA");
    }

//{{{ begin code
    
//}}} end code

    return true;
}


// ************************************************************************* //

