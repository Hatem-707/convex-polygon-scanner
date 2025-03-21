import matplotlib.pyplot as plt
import numpy as np

# Function to compute slope
def compute_slope(iterations, time_seconds):
    slopes = iterations / time_seconds  # Element-wise division
    average_slope = np.mean(slopes)  # Compute the average slope
    return average_slope, slopes

# Load first dataset
file_path_1 = "benchmark1.txt"  # Adjust path as needed
data1 = np.loadtxt(file_path_1)
iterations1 = data1[:, 0]
time_seconds1 = data1[:, 1]

# Load second dataset
file_path_2 = "benchmark2.txt"  # Adjust path as needed
data2 = np.loadtxt(file_path_2)
iterations2 = data2[:, 0]
time_seconds2 = data2[:, 1]

# Compute slopes for both datasets
slope1, slopes_array1 = compute_slope(iterations1, time_seconds1)
slope2, slopes_array2 = compute_slope(iterations2, time_seconds2)

# Generate points for the slope lines
time_range = np.linspace(min(time_seconds1), max(time_seconds1), 100)
line1 = slope1 * time_range
time_range2 = np.linspace(min(time_seconds2), max(time_seconds2), 100)
line2 = slope2 * time_range2

# Plot both datasets in the same figure
plt.figure(figsize=(10, 5))

# Plot first dataset
plt.scatter(time_seconds1, iterations1, color='b', label="Old code data", alpha=0.6)
plt.plot(time_range, line1, color='b', linestyle='--', label=f"Old code slope: {slope1:.2f}")

# Plot second dataset
plt.scatter(time_seconds2, iterations2, color='r', label="New code Data", alpha=0.6)
plt.plot(time_range2, line2, color='r', linestyle='--', label=f"New code Slope: {slope2:.2f}")

# Add labels, title, legend, and grid
plt.xlabel("Time (seconds)")
plt.ylabel("Iterations")
plt.title("Benchmark Comparison: Time vs Iterations with Computed Slopes")
plt.legend()
plt.grid(True)

# Show the combined plot
plt.show()

# Plot bar graph for slopes comparison
plt.figure(figsize=(8, 5))
plt.bar(["Old", "New code"], [slope1, slope2], color=['blue', 'red'])
plt.xlabel("Dataset")
plt.ylabel("Average Slope (Iterations/Time)")
plt.title("Comparison of Average Slopes")
plt.grid(axis='y')

# Show bar graph
plt.show()