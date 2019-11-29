import argparse
import sys
import numpy as np
import matplotlib.pyplot as plt
from cycler import cycler
from matplotlib.colors import Normalize
from matplotlib.colorbar import ColorbarBase
import steps_pb2


src = ''
out = ''


def read_proto():
    data = steps_pb2.Steps()

    # Read data from a protocol buffer binary
    f = open(src, "rb")
    data.ParseFromString(f.read())
    f.close()
    return data


def plot(n, x, v):
    fig, (ax, colorbar) = plt.subplots(1, 2, gridspec_kw={'width_ratios': [10, 1]})
    ax.grid(True)
    ax.set_xlabel('X (condensation)')
    ax.set_ylabel('V (deformation)')
    ax.set_xscale('log')
    ax.set_yscale('log')

    # Choose a color map, loop through the colors, and assign them to the color
    # cycle. You need n-1 colors, because you'll plot that many lines
    # between pairs. In other words, your line is not cyclic, so there's
    # no line from end to beginning.
    # Colormaps: https://matplotlib.org/3.1.0/tutorials/colors/colormaps.html
    cm = plt.get_cmap('Spectral')
    cycle = cycler(color=[cm(1.*i/(n-1)) for i in range(n-1)])
    ax.set_prop_cycle(cycle)
    for i in range(n-1):
        ax.plot(x[i:i+2], v[i:i+2])

    norm = Normalize(vmin=0, vmax=n)
    ColorbarBase(colorbar, cmap=cm, norm=norm)
    colorbar.set_xlabel('step')
    fig.tight_layout()
    plt.savefig(out)


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

    # Plot for XV.
    x_value = [step.x_value for step in data.steps]
    v_value = [step.v_value for step in data.steps]
    plot(n, x_value, v_value)

