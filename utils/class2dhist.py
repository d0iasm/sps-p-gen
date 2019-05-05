import csv
import sys
import numpy as np
import matplotlib.pyplot as plt


DEFAULT = "../open-boundary/csv/result.csv"

csvfile = DEFAULT
if len(sys.argv) > 1:
    csvfile = sys.argv[1]

data = []
with open(csvfile) as f:
    reader = csv.DictReader(f, delimiter=',')
    for row in reader:
        data.append(dict(row))

data = sorted(data, key = lambda x: (float(x['kp']), float(x['km'])))
kp = np.array([x / 10 for x in np.arange(-8, 13)])
km = np.array([x / 10 for x in np.arange(-8, 13)])
c = []
for i in range(0, len(data), len(kp)):
    c.append([int(x['class']) for x in data[i:i+len(kp)]])

fig, ax = plt.subplots()
im = ax.imshow(c)
ax.set_xticks(np.arange(len(kp)))
ax.set_yticks(np.arange(len(km)))
ax.set_xticklabels(kp)
ax.set_yticklabels(km)

# Rotate the tick labels and set their alignment.
plt.setp(ax.get_xticklabels(), rotation=45, ha="right",
         rotation_mode="anchor")

# Loop over data dimensions and create text annotations.
for i in range(len(km)):
    for j in range(len(kp)):
        text = ax.text(j, i, c[i][j],
                       ha="center", va="center", color="w")

ax.set_title("Class Category based on X and V")
ax.set_xlabel("Kp")
ax.set_ylabel("Km")
fig.tight_layout()
plt.show()

