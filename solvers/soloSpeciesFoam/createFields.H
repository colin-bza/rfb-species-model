// Info<< "Reading field p\n" << endl;
// volScalarField p
// (
//     IOobject
//     (
//         "p",
//         runTime.timeName(),
//         mesh,
//         IOobject::MUST_READ,
//         IOobject::AUTO_WRITE
//     ),
//     mesh
// );


#include "createRDeltaT.H"
// reactingFoam definitions

// Create a thermophysical model applicable for liquids and gases
// IDEA: Overkill, modify for a simpler transport model
// in a second version

Info<< "Reading thermophysical properties\n" << endl;

autoPtr<psiReactionThermo> pThermo(psiReactionThermo::New(mesh));

// Initialise psiReactionThermo object that includes enthalpy and mass
// fractions of the mixture
psiReactionThermo& thermo = pThermo();

thermo.validate(args.executable(), "h", "e");

// Create a reference to the list that stores the compositions
basicSpecieMixture& composition = thermo.composition();

// Create a pointer list to the compositions
PtrList<volScalarField>& Y = composition.Y();


const word inertSpecie(thermo.get<word>("inertSpecie"));
if (!composition.species().found(inertSpecie))
{
    FatalIOErrorIn(args.executable().c_str(), thermo)
        << "Inert specie " << inertSpecie << " not found in available species "
        << composition.species() << exit(FatalIOError);
}



 Info<< "Reading field U here\n" << endl;
 volVectorField U
 (
     IOobject
     (
         "U",
         runTime.timeName(),
         mesh,
         IOobject::MUST_READ,
         IOobject::AUTO_WRITE
     ),
     mesh
 );

#include "createPhi.H"

label pRefCell = 0;
scalar pRefValue = 0.0;
// setRefCell(p, piso.dict(), pRefCell, pRefValue);
// mesh.setFluxRequired(p.name());


//singlePhaseTransportModel laminarTransport(U, phi);
singlePhaseTransportModel laminarTransport(U, phi);

// Turbulent Schmidt Number
dimensionedScalar Sct("Sct", dimless, laminarTransport);

// Molecular diffusivity m^2/s, binary diffusion
// dimensionedScalar D("D", dimless, laminarTransport);
dimensionedScalar D("D", dimensionSet(0, 2, -1, 0, 0, 0), laminarTransport);

 Info << "Creating incompressible turbulence model.\n" << nl;
 autoPtr<incompressible::turbulenceModel> turbulence
 (
     incompressible::turbulenceModel::New(U, phi, laminarTransport)
 );



#include "createMRF.H"
#include "createFvOptions.H"

// Info << "Creating porosity field" << endl;

// // Brinkman equation porous cells definition
// // Info << "Pre-error message" << endl;
// // Info << mesh.cellZones() << endl;

// // Read porousCells
// const labelList& porousCells = mesh.cellZones()[0];

// Info << "Post-error message" << endl;

Info<< "Initialising density field" << endl;
// Initialise density field. thermo.rho() could be changed in the future
volScalarField rho
(
    IOobject
    (
        "rho",
        runTime.timeName(),
        mesh
    ),
    thermo.rho()
);

// Compressible turbulence model only for mass transfer
Info << "Creating turbulence model for mass transfer" << nl;
autoPtr<compressible::turbulenceModel> turbulence_mass
(
    compressible::turbulenceModel::New
    (
        rho,
        U,
        phi,
        thermo
    )
);

// Chemical reactions
// They require to import the combustion 
// We may want to remove this in the future
Info<< "Creating reaction model\n" << endl;

// The combustionModel can only be initialised with a compressible
// mass transfer model
autoPtr<CombustionModel<psiReactionThermo>> reaction
(
    CombustionModel<psiReactionThermo>::New(thermo, turbulence_mass())
);

// Surface interpolation has mass fractions and enthalpy
multivariateSurfaceInterpolationScheme<scalar>::fieldTable fields;

forAll(Y, i)
{
    fields.add(Y[i]);
}

fields.add(thermo.he());

// Heat of reaction
volScalarField Qdot
(
    IOobject
    (
        "Qdot",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionedScalar(dimEnergy/dimVolume/dimTime, Zero)
);

// volScalarField porosity
//read kinematic viscosity value (nu) from the transportProperties directory in constant/
dimensionedScalar nu("nu", dimViscosity, laminarTransport);

Info<< "Reading kinematic viscosity\n" << endl;
volScalarField kinematicViscosity
(
    IOobject
    (
	"kinematicViscosity",
	runTime.timeName(),
	mesh,
    IOobject::NO_READ,
	IOobject::AUTO_WRITE // NO_WRITE
    ),
    mesh,
    dimensionedScalar("kinematicViscosity",dimViscosity,nu.value())
    // dimensionedScalar("kinematicViscosity",dimViscosity,1.0)
);
 
tmp<fv::convectionScheme<scalar>> mvConvection
(
     fv::convectionScheme<scalar>::New
     (
         mesh,
         fields,
         phi,
         mesh.divScheme("div(phi,Yi_h)")
     )
);

tmp<fv::convectionScheme<scalar>> icConvection
 (
     fv::convectionScheme<scalar>::New
     (
         mesh,
         fields,
         phi,
         mesh.divScheme("div(phi,Yi)")
     )
 );

// Hard-coded placeholder density until I find out how to read the density (dimensionless)
// and give dimensions for the consistent of the R eqn
dimensionedScalar rho_dim = dimensionedScalar("rho_dim", dimensionSet(1, -3, 0, 0, 0, 0, 0), 1);

// forAll(porousCells,i)
// {
// 	porosity[porousCells[i]] = eps.value();
// }

// surfaceScalarField porosityFaces = fvc::interpolate(porosity);

// Info << kinematicViscosity << endl;
