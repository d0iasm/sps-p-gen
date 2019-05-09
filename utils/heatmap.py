import argparse
import csv
import sys
import numpy as np
import matplotlib.pyplot as plt


title = ""
src = ""
x_axis = ""
y_axis = ""
z_axis = ""
is_print_annotation = False


def read_csv():
    data = []
    with open(src) as f:
        reader = csv.DictReader(f, delimiter=',')
        for row in reader:
            data.append(dict(row))

    data = sorted(data, key = lambda x: (float(x[y_axis]), float(x[x_axis])))
    return data


def shape(data):
    kp = np.array([x / 10 for x in np.arange(-8, 13)])
    km = np.array([x / 10 for x in np.arange(-8, 13)])
    c = []
    for i in range(0, len(data), len(kp)):
        c.append([float(x[z_axis]) for x in data[i:i+len(kp)]])
        #print("===================", i)
        #print([x['kp'] for x in data[i:i+len(kp)]], [x['km'] for x in data[i:i+len(kp)]])
        #print([float(x['energy-average']) for x in data[i:i+len(kp)]])
        
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
    if is_print_annotation:
        for i in range(len(y)):
            #print(c[i])
            for j in range(len(x)):
                t = int(c[i][j]) if c[i][j].is_integer() else c[i][j]
                text = ax.text(j, i, t,
                        ha="center", va="center", color="w")

    ax.set_title(title)
    ax.set_xlabel(x_axis)
    ax.set_ylabel(y_axis)
    fig.tight_layout()
    plt.show()


def parse_args():
    global title
    global src
    global x_axis
    global y_axis
    global z_axis
    global is_print_annotation

    parser = argparse.ArgumentParser(description='Customize src csv file and header keys in it to generate heatmap.')
    parser.add_argument('-title', required=True,
            help='The title of headmap.')
    parser.add_argument('-src', default='../open-boundary/csv/result.csv',
            help='The source csv file. (Default: ../open-boundary/csv/result.csv)')
    parser.add_argument('-x', default='kp',
            help='The x-axis key for heatmap. This is used as a key of csv (Default: kp).')
    parser.add_argument('-y', default='km',
            help='The y-axis key for heatmap. This is used as a key of csv (Default: km).')
    parser.add_argument('-z', default='class',
            help='The z-axis key for heatmap. This is used as a key of csv (Default: class).')
    parser.add_argument('-print', default=False, const=True, nargs='?',
            help='Whether print annotations on heatmap. (Default: False).')

    args = parser.parse_args()
    title = args.title
    src = args.src
    x_axis = args.x
    y_axis = args.y
    z_axis = args.z
    is_print_annotation = args.print


if __name__ == '__main__':
    parse_args()
    print("title (x, y, z) src: ", title, x_axis, y_axis, z_axis, src)
    data = read_csv()
    kp, km, c = shape(data)
    plot(kp, km, c)

