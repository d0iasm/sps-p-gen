import argparse
import csv
import sys
import numpy as np
import matplotlib.pyplot as plt
import steps_pb2


src = ''
out = ''

maxk = 1.0
mink = -1.0

thinning = 200


def read_proto():
    data = steps_pb2.Steps()

    # Read data from a protocol buffer binary
    f = open(src, "rb")
    data.ParseFromString(f.read())
    f.close()
    return data


def plot(n, y):
    fig, ax = plt.subplots(figsize=(6.4, 9.6))

    x = np.arange(0, thinning*n, thinning)

    # Get labels which have a value more than 0.
    labels = [None for _ in range(len(y[0]))]
    for step in y:
        i = 0
        for key, val in step.items():
            if val > 0:
                labels[i] = key
            i += 1

    # Color pallete.
    pal = ["#ff0000", "#ff5500", "#ff8000", "#ffaa00",
            "#ffd500", "#ffff00", "#d4ff00", "#80ff00",
            "#55ff00", "#00ff00", "#00ff55", "#00ff80",
            "#00ffaa", "#00ffd5", "#00ffff", "#00d5ff",
            "#00aaff", "#0080ff", "#0055ff", "#002aff",
            "#0000ff"]
    # Revese the color order (red: friendly, blue: hostile)
    pal = pal[::-1]

    y = [list(step.values()) for step in y]
    # x: 1d array of dimension N.
    # y: 2d array (dimension MxN), or sequence of
    #    1d arrays (each dimension 1xN)
    # x is the number of steps and y is the 2d array of each preference value and each step.
    # y = [
    #   (preference 1 and step 1, preference 1 and step 2, ..., preference 1 and step N),
    #   (preference 2 and step 1, preference 2 and step 2, ..., preference 2 and step N),
    #   ...
    #   (preference M and step 1, preference M and step 2, ..., preference M and step N)]
    # What is list(zip(*y))?
    #   a = [[1,2],[3,4]]
    #   print(list(zip(*a))) # [(1, 3), (2, 4)]
    ax.stackplot(x, list(zip(*y)), labels=labels, colors=pal, edgecolors='black', linewidths=.5)
    handles, labels = ax.get_legend_handles_labels()
    # Place legend at the upper right and reverse to keep order consistent.
    leg = ax.legend(reversed(handles), reversed(labels), loc='upper right', prop={'size': 6})
    leg.get_frame().set_alpha(0.4)

    plt.savefig(out)


def count_k(steps):
    count = []
    for step in steps:
        dic = {key/10:0 for key in range(int(mink*10), int(maxk*10)+1)}
        for particle in step.particles:
            for kparam in particle.kparams:
                dic[kparam/10] += 1
        count.append(dic)
    return count


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

    kparams_count = count_k(data.steps)
    plot(n, kparams_count)

