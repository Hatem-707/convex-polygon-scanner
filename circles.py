import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import griddata

# Change this to your file path
filename = 'circles2.txt'

# Load data from the file:
# Each line is assumed to be: z x y
data = np.loadtxt(filename)

# Extract columns:
z = data[:, 0]
x = data[:, 1]
y = data[:, 2]

# Create a grid over the domain of x and y:
grid_x, grid_y = np.mgrid[x.min():x.max():100j, y.min():y.max():100j]

# Interpolate the z values on the grid:
grid_z = griddata((x, y), z, (grid_x, grid_y), method='cubic')

# Plot the results:
plt.figure(figsize=(8, 6))
# Use contourf to visualize the interpolated surface:
contour = plt.contourf(grid_x, grid_y, grid_z, levels=15, cmap='viridis')
plt.colorbar(contour, label='z value')

# Plot original data points:
plt.scatter(x, y, c=z, edgecolor='k', cmap='viridis')
plt.xlabel('x')
plt.ylabel('y')
plt.title('Interpolated Surface Plot of z (first column) vs. x & y')
plt.show()
