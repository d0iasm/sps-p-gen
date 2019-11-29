import argparse
import sys
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import steps_pb2


src = ''
out = ''

thinning = 200


def read_proto():
    data = steps_pb2.Steps()

    # Read data from a protocol buffer binary
    f = open(src, "rb")
    data.ParseFromString(f.read())
    f.close()
    return data


def plot(n, satisfactions, k_average):
    fig, ax = plt.subplots()

    x = np.arange(0, thinning*n, thinning)
    pal = sns.color_palette("GnBu_d", 50)

    for i in range(len(satisfactions)):
        ax.plot(x, satisfactions[i], color=pal[i], linewidth=1, alpha=0.4)
    ax.grid()

    ave = [0] * n
    for i in range(n):
        for num in range(len(satisfactions)):
            ave[i] += satisfactions[num][i]

    ave = [a/len(satisfactions) for a in ave]
    ax.plot(x, ave, color='blue', linewidth=4, alpha=0.7)

    subax = ax.twinx()
    subax.plot(x, k_average, color='orange', linewidth=4, alpha=0.7, label='K average')
    subax.set_ylim(-1, 1)

    ax.set_xlabel("Step")
    ax.set_ylabel("Satisfactions")
    subax.set_ylabel("K average")

    ax.yaxis.label.set_color('blue')
    subax.yaxis.label.set_color('orange')

    tkw = dict(size=4, width=1.5)
    ax.tick_params(axis='y', colors='blue', **tkw)
    subax.tick_params(axis='y', colors='orange', **tkw)
    ax.tick_params(axis='x', **tkw)

    fig.tight_layout()
    plt.savefig(out)


def calc_k_ave(particles):
    s = 0
    for p in particles:
        for k in p.kparams:
            s += k
    return s / (len(particles) * len(particles[0].kparams))


def k_average(steps):
    return [calc_k_ave(step.particles) for step in steps]


def reshape(steps):
    n_particles = 0
    for p in steps[0].particles:
        n_particles += 1

    satisfactions = []
    for i in range(n_particles):
        satisfactions.append([step.particles[i].satisfaction for step in steps])
    return satisfactions


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
    k_average = k_average(data.steps)
    satisfactions = reshape(data.steps)
    plot(n, satisfactions, k_average)
