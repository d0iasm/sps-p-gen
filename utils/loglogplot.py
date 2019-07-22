import argparse
import json
import sys
import numpy as np
import matplotlib.pyplot as plt
from cycler import cycler
from matplotlib.colors import Normalize
from matplotlib.colorbar import ColorbarBase


src = ''
out = ''


def read_json():
    with open(src) as f:
        data = json.load(f)
    return data

def func():
    return 'r'


def plot(n, x, v):
    fig, (ax, colorbar) = plt.subplots(1, 2, gridspec_kw={'width_ratios': [10, 1]})
    ax.set(title='XV: ' + src)
    ax.grid(True)
    ax.set_xlabel('X')
    ax.set_ylabel('V')
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
    ColorbarBase(colorbar, cmap='Spectral', norm=norm)
    colorbar.set_xlabel('step')
    fig.tight_layout()
    plt.savefig(out)


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
    data = read_json()
    x = [d['xv']['x'] for d in data] 
    v = [d['xv']['v'] for d in data] 
    plot(len(data), x, v)
