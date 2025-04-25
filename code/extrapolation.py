import numpy as np
import matplotlib.pyplot as plt

# Step 1: Read file and compute log of row averages
log_averages = []
with open("benchmark3.txt", "r") as f:
    for line in f:
        values = list(map(float, line.strip().split()))
        if values:
            avg = sum(values) / len(values)
            log_averages.append(np.log(avg))  # store ln(average)

# Step 2: Define x as 8 + i
x_raw = np.array([8 + i for i in range(len(log_averages))])
log_y_raw = np.array(log_averages)

# Step 3: Fit quadratic model to all available data
coeffs = np.polyfit(x_raw, log_y_raw, 2)
a, b, c = coeffs

# Step 4: Extrapolate for x = 14, 15, 16
x_extrapolated = np.array([14, 15, 16])
log_y_extrapolated = a * x_extrapolated**2 + b * x_extrapolated + c
y_extrapolated = np.exp(log_y_extrapolated)

# Print extrapolated values
for x_val, log_val, y_val in zip(x_extrapolated, log_y_extrapolated, y_extrapolated):
    print(f"Extrapolated ln(average) at x = {x_val}: {log_val:.4f}")
    print(f"Extrapolated average at x = {x_val}: {y_val:.2f}")

# Step 5: Prepare for plotting
x_plot_limit = 16
x_dense = np.linspace(min(x_raw), x_plot_limit, 300)
log_y_dense = a * x_dense**2 + b * x_dense + c

# Combine raw + extrapolated for connection
x_combined = np.concatenate((x_raw[x_raw < 14], x_extrapolated))
log_y_combined = np.concatenate((log_y_raw[x_raw < 14], log_y_extrapolated))

# Step 6: Plot
plt.figure(figsize=(10, 6))

# Original data (up to x < 14)
plt.plot(x_raw[x_raw < 14], log_y_raw[x_raw < 14], 'bo--', label="ln(Average) from Data", linewidth=1)

# Extrapolated points
plt.plot(x_extrapolated, log_y_extrapolated, 'go--', label="Extrapolated (x=14,15,16)", linewidth=1.5, marker='o', markersize=7)

# Quadratic fitted curve
plt.plot(x_dense, log_y_dense, 'r-', label="Quadratic Fit (Log-Space)", linewidth=2)

# Labels
plt.xlabel("n (Row index + 8)")
plt.ylabel("ln(Average Value)")
plt.title("Quadratic Extrapolation in Log-Space (Up to x = 16)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
