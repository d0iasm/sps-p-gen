import argparse
import fcntl
import sys
import math
import re
import numpy as np
import matplotlib.pyplot as plt
from scipy.cluster import hierarchy
import steps_pb2


src = ''
out = ''

text = ''

cycle = 20
thinning = 200
# 0.1 (300,000~) 0.2 (400,000~) 0.3 (500,000~) 0.4 (600,000~) 0.5 (700,000~) 0.6 (800,000~) 0.7 (1,000,000~) 0.8 (1,200,000) 0.9 (1,400,000) 1.0 (1,600,000)
print_steps = [300000, 400000, 500000, 600000, 700000, 800000, 1000000, 1200000, 1400000, 1600000]


def read_proto():
    data = steps_pb2.Steps()

    # Read data from a protocol buffer binary
    f = open(src, "rb")
    data.ParseFromString(f.read())
    f.close()
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
    #ax.set_xlim(0.0, 20.0)
    ax.axis("equal")
    title = "threshold: %f, number of clusters: %d, step: %d" % (thresh, len(set(clusters)), step)
    plt.title(title)
    fig.tight_layout()

    filename = out.replace(".png", "&step=%d.png" % (step))
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
            description='Generate an image from a data file.')
    parser.add_argument('-src', required=True,
            help='The source file path')
    parser.add_argument('-out', required=True,
            help='The output image path')

    args = parser.parse_args()
    src = args.src
    out = args.out


if __name__ == '__main__':
    parse_args()
    data = read_proto()
    n = len(data.steps)

    particles = [step.particles for step in data.steps]
    init_text()
    for step in print_steps:
        i = step//thinning
        if i >= len(particles):
            break
        particle = np.array([[p.x, p.y] for p in particles[i]])
        plot(particle, step)
    write_clustering_csv()

