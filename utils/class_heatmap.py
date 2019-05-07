import csv
import sys
import numpy as np
import matplotlib.pyplot as plt


DEFAULT = "../open-boundary/csv/result.csv"

def read_csv():
    csvfile = DEFAULT
    if len(sys.argv) > 1:
        csvfile = sys.argv[1]

    data = []
    with open(csvfile) as f:
        reader = csv.DictReader(f, delimiter=',')
        for row in reader:
            data.append(dict(row))

    data = sorted(data, key = lambda x: (float(x['km']), float(x['kp'])))
    return data


def shape(data):
    kp = np.array([x / 10 for x in np.arange(-8, 13)])
    km = np.array([x / 10 for x in np.arange(-8, 13)])
    c = []
    for i in range(0, len(data), len(kp)):
        c.append([int(x['class']) for x in data[i:i+len(kp)]])
        #print("===================", i)
        #print([x['kp'] for x in data[i:i+len(kp)]], [x['km'] for x in data[i:i+len(kp)]])
        #print([int(x['class']) for x in data[i:i+len(kp)]])
        
    return kp, km, c


def plot(x, y, z): 
    fig, ax = plt.subplots()
    im = ax.imshow(z)
    ax.set_xticks(np.arange(len(x)))
    ax.set_yticks(np.arange(len(y)))
    ax.set_xticklabels(x)
    ax.set_yticklabels(y)

    # Rotate the tick labels and set their alignment.
    plt.setp(ax.get_xticklabels(), rotation=45, ha="right",
            rotation_mode="anchor")

    # Loop over data dimensions and create text annotations.
    for i in range(len(y)):
        print(c[i])
        for j in range(len(x)):
            text = ax.text(j, i, c[i][j],
                       ha="center", va="center", color="w")

    ax.set_title("Class Category based on X and V")
    ax.set_xlabel("Kp")
    ax.set_ylabel("Km")
    fig.tight_layout()
    plt.show()


if __name__ == '__main__':
    data = read_csv()
    kp, km, c = shape(data)
    plot(kp, km, c)

