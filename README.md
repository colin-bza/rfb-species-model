### RFB-model

RFB-model is a suite of OpenFOAM cases and solvers
applicable to redox flow battery modelling.

- 3-D simulation of **flow in a rectangular channel**
with a porous blockage element using the Darcy-Forcheimer
model.

- Mesh of a **channel electrode**

- Tested in OpenFOAM-v2006

Test: simulate one-dimensional flow in a
rectangular channel

```
cd cases/porousBlockage_3D
./Allrun
```

Postprocess the results with paraview

```
paraview foam.foam
```
The following instructions are applicable for OpenFOAM-v2312. This version of OpenFOAM can be downloaded from the OpenFOAM official website, which also provides the Installation Guide for different operating systems. The code has been tested in OpenFOAM installations only on Linux. 

For post processing and visualizing the simulation results, it has to be used ParaView. This software is automatically installed when following the Installation Guide of the “ThirdParty” open-source software that comes along with OpenFOAM. However, this part of the installation may fail. If this happens, ParaView can be installed separately from the ParaView official website. Python 3 is required to execute a number of scripts that facilitate the creation of the meshes and post processing the results (Python website).

Once OpenFOAM is installed, the solver ```brinkmanPisoFoam``` must be compiled, which solves the momentum and mass conservation equation. Also, the solver ```soloSpeciesFoam``` must be compiled, which solves the mass conservation equation only.

The general structure of each case contains three folders, alongside several code utilities and scripts. The 0/ directory contains the initial and boundary conditions for all fields. The constant directory contains the thermophysical and transport properties of the fluid, the electrode porosity, permeability (which is a function of porosity), among others. The system directory contains files to set-up the time and space discretization algorithms, the convergence criteria of the solvers, the domain decomposition dictionary and the runtime simulation control dictionary.

The Allclean script removes all the simulation results that have been written to files in serial and parallel simulations. The Allrun executes a series of commands to create the mesh with blockMesh, generate the porous medium in the mesh with topoSet, and decompose the domain using the decomposeParDict file depending on the number of cores where the simulations are run, although it is not necessary to include all of them.

One of the advantages of the Allrun file is that it allows two different solvers (brinkmanPisoFoam and soloSpeciesFoam) to be run for a desired time. This greatly reduces the simulation time for each case. To mitigate the high computational cost of the first solver, a quasi-steady velocity field is saved and used as input for the second solver, significantly reducing simulation time and cost.

For all simulated cases, the brinkmanPisoFoam solver was run for up to 0.1 seconds of simulation, while the soloSpeciesFoam solver was run from that time until 60 seconds. For each solver, the corresponding parameters must be set, such as writeInterval, maxDeltaT, DeltaT, maxCo (corresponding to the Courant number, relevant for solving the velocity field), etc.

The foam.foam file is a dummy file required to see the simulation results using ParaView. Finally, ParaView and Python are used to post process. To make images in Python, a .csv file is created from ParaView and then exported. 

In OpenFOAM, all the files required for a particular simulation are conventionally grouped into a folder denominated "XCase". A .zip file is provided which contains two cases: BaseCase and OLSCase. The first case corresponds to the base case studied based on the parameters of Huerta and Pino, and the second case corresponds to the optimised case found in this study.

```
cd cases/BaseCase
code . (if you have installed VSCode)
```
First, you will have to edit the parameters of the case in the constant and system folders. Then, you will need to modify the Allrun file with the parameters that you want.

```
./Allrun
```
Once the model has finished simulating, you can view the results in Paraview.

```
paraview foam.foam
```
Authors
- Catalina Pino - Research Associate in Flow Battery Modelling at Imperial College London
- Felipe Huerta - 4th year PhD Student at Imperial College London
- Lucas Ortiz - Master of Science Student at Pontificia Universidad Católica de Chile
