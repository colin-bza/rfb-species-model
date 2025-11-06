# Scientific computing
import numpy as np

# Facilitate CFD data handling
import pandas as pd

# Visualisation
import matplotlib.pyplot as plt

# Data source
df = pd.read_csv("postProcessing/singleGraph/30/line_A_U.xy", sep='\t', header=None)

# Extract length coordinate, velocity and A mass fraction
z_CFD = df.iloc[:,0]

# Mass fraction
c_CFD = df.iloc[:,1]

# Velocity near the exit / m / s
v = df.iloc[80,4]

# Maximum concentration
c0 = 0.01

# Reaction constant
k = 1.25

# Diffusivity, to be updated
D = 1.569e-5

# The domain is 14mm long
L = 0.014

# Number of axial elements
n_z = 100

# Vertical coordinate vector to sample solution
z_1D = np.linspace(0, L, n_z)

def c_an(z, c0, k, D, v):
    ''' Concentration profile in steady state for 1-D
    advection diffusion reaction equation'''

    return c0 * np.exp( - (np.sqrt(1 + (4*k*D/v**2)) - 1) * (v*z/(2*D)) )

# Error calculation and printing
MSE = np.sqrt(sum((c_an(z_CFD, c0, k, D ,v) - c_CFD)**2)/len(c_CFD))
MAPE = np.sqrt(sum( ((c_an(z_CFD, c0, k, D ,v) - c_CFD)/c_CFD)**2)/len(c_CFD))
print("The mean squared error of molar fraction of A is MSE = %.3e" % MSE)
print("The mean average percentage error of molar fraction of A is MAPE = %.3f %%" % MAPE)

# Plot production
plt.plot(z_1D,c_an(z_1D, c0, k, D ,v), label='1-D,SS, k=%.3f' % k)
plt.plot(z_CFD, c_CFD, label='CFD')
plt.title("1-D ADR Verification: MSE = %.2e, MAPE = %.2f%%" % (MSE, MAPE))

plt.xlabel('z')
plt.ylabel(r'$w_{A}$')
plt.legend()
plt.savefig("verification.jpg")