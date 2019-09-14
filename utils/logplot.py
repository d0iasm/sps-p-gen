import argparse
import json
import sys
import numpy as np
import matplotlib.pyplot as plt


src = ''
out = ''
dynamic = False


def read_json():
    with open(src) as f:
        data = json.load(f)
    return data


def plot(n, e_ave, e_var, is_dynamic):
    fig, ax = plt.subplots()

    #x = np.arange(n)
    x = np.arange(0, 100*n, 100)

    ax.plot(x, e_ave, label='Average')
    ax.plot(x, e_var, label='Variance')
    leg = ax.legend(loc='upper right', fancybox=True, shadow=True)
    leg.get_frame().set_alpha(0.4)
    ax.grid()

    fig.tight_layout()
    fn = out.replace("energy", "dynamic_energy" if is_dynamic else "static_energy", 1)
    plt.savefig(fn)


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
    # Save an image for static energy.
    e_ave = [y['energy']['static']['average'] for y in data]
    e_var = [y['energy']['static']['variance'] for y in data]
    plot(len(data), e_ave, e_var, False)

    # Save an image for dynamic image.
    e_ave = [y['energy']['dynamic']['average'] for y in data]
    e_var = [y['energy']['dynamic']['variance'] for y in data]
    plot(len(data), e_ave, e_var, True)
