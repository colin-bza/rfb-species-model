#!/usr/bin/env python3
"""
We wish to choose a number of cells N (so that there are N+1 nodes)
and a geometric progression factor q (so that the cell widths are
d_i = (x_u/4)*q^i,  i = 0,...,N-1)
such that:
  (i)  The smallest cell is x_u/4,
  (ii) The largest cell is x_u/r, where by definition
         r = 4 / q^(N-1)   (so that the refinement ratio is r_f = q^(N-1) = 4/r),
  (iii) The total length is
         L = (x_u/4)*(q^N - 1)/(q-1) = 2*x_u.
Thus, we must have

      (q^N - 1)/(q - 1) = 8 .
      
Because N must be an integer an exact solution is not possible (the only
exact solution is the uniform mesh, q = 1, N = 8). One may therefore
“choose” N and solve for q.
This script loops over a range of candidate N values and (using a root‐finder)
computes the corresponding q, r_f, and r.
Change the candidate range as desired.
"""

import numpy as np
from scipy.optimize import newton

def total_length_eq(q, N):
    """
    For a given number of cells N and progression factor q,
    the dimensionless total length (in units of x_u) is
       L_dim = (q^N - 1)/(q - 1).
    We require L_dim = 8 (because (x_u/4)*8 = 2*x_u).
    This function returns f(q) = (q^N - 1)/(q - 1) - 8.
    """
    # When q is very close to 1, use the limit (which is N).
    if abs(q - 1) < 1e-8:
        return N - 8
    return (q**N - 1)/(q - 1) - 8

def solve_q(N, q_guess):
    """
    For a given integer N, find q such that (q^N - 1)/(q-1)=8.
    """
    return newton(lambda q: total_length_eq(q, N), q_guess)

# Here x_u is the only dimensional parameter.
# (In what follows the equations are written in nondimensional form.)
x_u = 1.0   # (You can supply any positive number; the answers scale with x_u)

results = []
# We want a graded mesh (q ≠ 1) so we choose N different from 8.
# (For N=8, q=1 and the mesh is uniform.)
# Try N = 3,4,5,6,7. (The user may choose a different range.)
for N in range(3, 8):
    # For an increasing mesh (cell widths growing) we need q > 1.
    # A reasonable initial guess is:
    q_guess = 1.1 if N < 8 else 0.99
    try:
        q = solve_q(N, q_guess)
    except RuntimeError as e:
        print(f"Could not solve for N={N}: {e}")
        continue
    # The refinement ratio is r_f = q^(N-1)
    r_f = q**(N-1)
    # And by our requirement d_{N-1} = (x_u/4)*q^(N-1) = x_u/r,
    # the blockMesh grading parameter (often called "r") must be
    r_param = 4 / r_f
    # (The total length computed from the cells is then exactly:
    #    L = (x_u/4)*(q^N-1)/(q-1) = 2*x_u.)
    L = (x_u/4)*(q**N - 1)/(q - 1) if abs(q-1)>1e-8 else (x_u/4)*N
    # (Since we solve the equation exactly, the relative error in L is ~0.)
    rel_error = abs(L - 2*x_u) / (2*x_u)
    results.append((N, q, r_f, r_param, L, rel_error))

# Print the results
print("For a given x_u (taken as {:.3f}), here are some candidate solutions:".format(x_u))
print("  N (cells)   q          r_f = d_max/d_min    blockMesh r   Total length (x_u units)   Rel. error")
for (N, q, r_f, r_param, L, err) in results:
    print(f"    {N:2d}      {q:7.4f}      {r_f:7.4f}         {r_param:7.4f}       {L:7.4f}             {err:7.1e}")

print("\nA possible answer is to choose, say, N=5 cells (6 nodes),")
print("with q ≈ {:.4f}, so that the refinement ratio is".format(results[1][1]))
print("  r_f = q^(N-1) ≈ {:.4f} and the blockMesh parameter r = 4/r_f ≈ {:.4f}."
      .format(results[1][2], results[1][3]))
print("Then the cell sizes are:")
print("  d0 = x_u/4,  d4 = (x_u/4)*q^(4) ≈ x_u/{:.4f}, and L = 2*x_u (to within numerical accuracy)."
      .format(results[1][3]))
