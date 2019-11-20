import argparse
import json
import fcntl
import sys
import math
import re
import numpy as np
import matplotlib.pyplot as plt
from scipy.cluster import hierarchy


src = ''
out = ''

text = ''

cycle = 20
thinning = 100
# 0.1 (300,000~) 0.2 (400,000~) 0.3 (500,000~) 0.4 (600,000~) 0.5 (700,000~) 0.6 (800,000~) 0.7 (1,000,000~) 0.8 (1,200,000) 0.9 (1,400,000) 1.0 (1,600,000)
steps = [3000, 4000, 5000, 6000, 7000, 8000, 10000, 12000, 14000, 16000]


def read_json():
    with open(src) as f:
        data = json.load(f)
    return data


def periodic_distance(p1, p2):
    """
    Calculate a shortest distance in a torus field.
    The maximum distance is 1414.1421... (= Square Root 200)
    with the length of a cycle is 20.
    """
    closest = sys.maxsize
    x1 = p1[0]
    y1 = p1[1]
    x2 = p2[0]
    y2 = p2[1]

    for i in range(-1, 2):
        for j in range(-1, 2):
            dx = (x2 + cycle * i) - x1
            dy = (y2 + cycle * j) - y1
            tmp = math.sqrt(dx * dx + dy * dy)
            if tmp < closest:
                closest = tmp
    return closest

def plot(data, step):
    global text
    fig, ax = plt.subplots()

    # clustering
    thresh = 1.5
    clusters = hierarchy.fclusterdata(data, thresh,
            criterion="distance", metric=periodic_distance)

    # plotting
    ax.scatter(*np.transpose(data), c=clusters)
    ax.axis("equal")
    title = "threshold: %f, number of clusters: %d, step: %d" % (thresh, len(set(clusters)), step)
    plt.title(title)
    fig.tight_layout()

    filename = out.replace(".png", "&step=%d.png" % (step))
    #plt.show()
    plt.savefig(filename)

    text += str(len(set(clusters))) + "\t"


def write_clustering_csv():
    f = open("clustering.csv", "a+")
    fcntl.lockf(f, fcntl.LOCK_EX)
    f.seek(0, 2)
    f.write(text + "\n")
    f.flush()
    fcntl.lockf(f, fcntl.LOCK_UN)
    f.close()


def init_text():
    global text
    match = re.search('.*s=([0-9]+)\.png', out)
    if match:
        seed = match.group(1)
        text += seed + "\t"
    else:
        text += "unknown\t"


def parse_args():
    global src
    global out

    parser = argparse.ArgumentParser(
            description='Generate an image from a json file.')
    parser.add_argument('-src', required=True,
            help='The source file path')
    parser.add_argument('-out', required=True,
            help='The output image path')

    args = parser.parse_args()
    src = args.src
    out = args.out


if __name__ == '__main__':
    parse_args()
    a = src.split('.')
    extension = a[len(a)-1]
    if extension != 'json':
        sys.exit('Error: ' + extension + ' file is not supported.')
    raw_data = read_json()
    points = [d['points'] for d in raw_data]
    init_text()
    for step in steps:
        data = np.array([[p['x'], p['y']] for p in points[step]])
        plot(data, step*thinning)
    write_clustering_csv()