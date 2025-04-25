from functools import reduce
import statistics
import matplotlib.pyplot as plt
import numpy as np

f = open("benchmark3.txt", "r")

lines = f.readlines()
means = []
deviations = []
x_axis = np.linspace(0, 50, 50)

base = 8

plt.figure(figsize=(20,10))
for i in range(len(lines)):
    points = base+i
    data = list(map(int,lines[i].split()))
    sum = reduce(lambda a,b: a+b, data)
    average = sum/len(data)
    means.append((points, average))
    std_dev = statistics.stdev(data)
    deviations.append(std_dev)
    plt.subplot(2,3,i+1)
    plt.scatter(x_axis, data)
    plt.title(f"Distribution of iterations of a set of size {points}")

plt.show()


x = [point for point, _ in means]  
y = [average for _, average in means]  
plt.figure(figsize=(12, 8))
plt.subplot(1,2,1)
plt.bar(x, y)
plt.xlabel("Points")
plt.ylabel("Average Iterations")
plt.title("Average Iterations for Different Set Sizes")
plt.subplot(1,2,2)
plt.bar(x, deviations[:1])
plt.xlabel("Points")
plt.ylabel("Standard Deviation")
plt.title("Standard Deviation for Different Set Sizes")
plt.show()


    