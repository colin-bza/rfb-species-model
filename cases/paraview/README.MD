# ParaView:

This folder contains useful .pvsm files to post-process different
simulation results.

Usage:
1. Change the working directory to the case directory
2. Ensure that an empty foam.foam file exists.
3. Open ParaView in the case through `paraview foam.foam`
4. Select "Load State" and choose one of the files below
5. In the drop-down menu, select Choose File Names.
6. Navigate to the case directory that you are interested to postprocess and select the foam.foam file in that directory.
7. Modify the filters if required

### Rescaling:

brinkmanPisoFoam solves the mass fraction $\omega_A$. To convert this quantity to concentration, multiply by the solution
density and divide by the molar weight. This gives a conversion factor of 20161.29.

### Colour conventions:

#### Plot over line
-  #004488 Dark blue
- #66ccee Light blue
- #bbbbbb Grey

#### Slice
- Paraview default; Black, Blue and White

## List of files

### grid.pvsm
- Plots a z-cross section of the channel in a wireframe
view to visualise the grid.

### interfacialMassFlux.pvsm: 
- Plots the spatial distribution of the mass flux 
over a center line at the channel-electrode interface.
- Plots the integrated mass flow at the interface as
a function of time.

### interfacialProfiles.pvsm
- In the lower-right corner, plots the mass fraction profile in a centreline from the channel at x = 0 to the
porous region boundary at x = l_c + l_e. Three positions are displayed, at the entrance (z = 1 mm), after the
entrance length (z = 10 mm) and at the exit (z = 50 mm) of the channel.
- In the upper-right corner, plots the 2-D concentration profiles at the same position as the 1-D plots over lines

### streamwiseProfiles.pvsm
- In the lower-right corner, plots the mass fraction profile through a centreline from the entrance of the channel
at z = 0 to the exit of the channel at $z =L$. Three transverse positions are displayed:
    - At the centre of the channel at (x = 0.5 mm, y = 0.5 mm) for the square channel cases, (x = 0.5 mm, y = 0.4 mm) for the rectangular case.
    - At the channel - porous region interface at (x = 1.0 mm, y = 0.5 mm) for the square channel cases, (x = 1.0 mm, y = 0.4 mm) for the rectangular case.
    - At the centre of the porous region interface at (x = 2.5 mm, y = 0.5 mm) for the square channel cases, (x = 2.5 mm, y = 0.4 mm) for the rectangular case.

- In the upper-right corner, plots the 2-D concentration profiles at the x-normal planes with the position vector at the same coordinates to represent the channel, interface and porous reigon, respectively.

### outlet.pvsm
- Plots the average mass flow  of species A that leaves the channel at the outlet $o$ as a function of time.
$$ \bar{\omega}_A(t) = \frac{\int_o \rho \omega_A(t) dS}{ \int_o \rho dS} $$

