import argparse
import csv
import json
import sys
import numpy as np
import matplotlib.pyplot as plt


src = ''
kparam = ''


def read_json():
    with open(src) as f:
        data = json.load(f)
    return data


def plot(n, y):
    fig, ax = plt.subplots()

    x = np.arange(n)

    # Get labels which have a value more than 0.
    labels = [None for _ in range(len(y[0]))]
    for step in y:
        i = 0
        for key, val in step.items():
            if val > 0:
                labels[i] = key
            i += 1
    y = [list(step.values()) for step in y]
    # x: 1d array of dimension N.
    # y: 2d array (dimension MxN), or sequence of
    #    1d arrays (each dimension 1xN)
    ax.stackplot(x, list(zip(*y)), labels=labels)
    leg = ax.legend(loc='upper right')
    leg.get_frame().set_alpha(0.4)

    fig.tight_layout()
    # Replace directory and extension.
    dest = 'img/kparam?' + src.split('/')[1]
    plt.savefig(dest.replace('json', 'png'))


def reshape_k(data):
    y = []
    for step in data:
        x = {key/10:0 for key in range(-20, 21)}
        for param_num in step['k']['dynamic']:
            x[param_num[0]] += param_num[1]
        y.append(x)
    return y


def parse_args():
    global src
    global kparam

    parser = argparse.ArgumentParser(
            description='Generate an image from a json file.')
    parser.add_argument('-src', required=True,
            help='The source file path') 
    parser.add_argument('-k', nargs='+',
            help='The K parameters')

    args = parser.parse_args()
    src = args.src
    if args.k == None:
        # Ex. 'abpm=0.8,0.4,0.8,0.5&b=open&d=true.html'
        params = src.split('&')
        kparam = params[0].split('=')[1]
    else:
        kparam = ','.join(args.k)


if __name__ == '__main__':
    parse_args()
    a = src.split('.')
    extension = a[len(a)-1]
    if extension != 'json':
        sys.exit('Error: ' + extension + ' file is not supported.')
    data = read_json()
    y = reshape_k(data)
    plot(len(data), y)
