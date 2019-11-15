import argparse
import json
import numpy as np

import stackplot
import logplot
import loglogplot
import clustering


src = ''


def read_json():
    with open(src) as f:
        data = json.load(f)
    return data


def parse_args():
    global src

    parser = argparse.ArgumentParser(
            description='Generate an image from a json file.')
    parser.add_argument('-src', required=True,
            help='The source file path')
    parser.add_argument('-out', required=True,
            help='The output image path')

    args = parser.parse_args()
    src = args.src
    out = args.out

    stackplot.src = args.src
    stackplot.out = out.replace("b=", "kparam?b=", 1)

    logplot.src = args.src
    logplot.out = out.replace("b=", "energy?b=", 1)

    loglogplot.src = args.src
    loglogplot.out = out.replace("b=", "xv?b=", 1)

    clustering.src = args.src
    clustering.out = out.replace("b=", "clustering?b=", 1)


if __name__ == '__main__':
    parse_args()
    a = src.split('.')
    extension = a[len(a)-1]
    if extension != 'json':
        sys.exit('Error: ' + extension + ' file is not supported.')
    data = read_json()

    # Plot for K param.
    k_data = stackplot.reshape_k(data)
    stackplot.plot(len(data), k_data)

    # Plot for static energy.
    e_ave = [y['energy']['static']['average'] for y in data]
    e_var = [y['energy']['static']['variance'] for y in data]
    logplot.plot(len(data), e_ave, e_var, False)

    # Plot for dynamic energy.
    e_ave = [y['energy']['dynamic']['average'] for y in data]
    e_var = [y['energy']['dynamic']['variance'] for y in data]
    logplot.plot(len(data), e_ave, e_var, True)

    # Plot for XV.
    x_data = [d['xv']['x'] for d in data]
    v_data = [d['xv']['v'] for d in data]
    loglogplot.plot(len(data), x_data, v_data)

    # Plot for clustering.
    points = [d['points'] for d in data]
    # 0.1 (300,000~) 0.2 (400,000~) 0.3 (500,000~) 0.4 (600,000~) 0.5 (700,000~) 0.6 (800,000~) 0.7 (1,000,000~) 0.8 (1,200,000) 0.9 (1,400,000) 1.0 (1,600,000)
    steps = [300, 400, 500, 600, 700, 800, 1000, 1200, 1400, 1600]
    for step in steps:
        point = np.array([[p['x'], p['y']] for p in points[step]])
        clustering.plot(point, step*clustering.thinning)
