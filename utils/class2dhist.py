import csv
import sys
import matplotlib.pyplot as plt


DEFAULT = "../open-boundary/csv/result.csv"

csvfile = DEFAULT
if len(sys.argv) > 1:
    csvfile = sys.argv[1]

data = []
with open(csvfile) as f:
    reader = csv.reader(f, delimiter=',')
    for row in reader:
        data.append(row)
        
data = data[1:]
km = [float(x[9]) for x in data] 
kp = [float(x[8]) for x in data] 
c = [int(x[10]) for x in data] 

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

h = ax.hist2d(kp, km)
ax.set_title('Class based on X and V.')
ax.set_xlabel('Kp')
ax.set_ylabel('Km')
fig.colorbar(h[3], ax=ax)
plt.show()


# Data 1

x = (20, 30, 40, 50, 60, 70)

y = (10, 10, 10, 10, 10, 10)

 

# Plot frequency distribution using histogram

plt.hist2d(x, y)

plt.title("2D Histogram Construction")

plt.margins(0)

plt.colorbar()

# Display the histogram

plt.show()
