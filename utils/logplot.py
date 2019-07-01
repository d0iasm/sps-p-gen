import argparse
import csv
import json
import sys
import numpy as np
import matplotlib.pyplot as plt


src = ''
kparam = ''


def read_csv():
    data = []
    with open(src) as f:
        reader = csv.reader(f, delimiter=',')
        next(reader)
        for row in reader:
            data.append([float(x) for x in row])
    return data 


def read_json():
    with open(src) as f:
        data = json.load(f)
    return data


def plot(n, e_ave, e_var):
    fig, ax = plt.subplots()

    x = np.arange(n)
    # log y axis
    ax.semilogy(x, e_ave, label='Average') 
    ax.semilogy(x, e_var, label='Variance') 
    ax.set(title='Dynamic Energy (average/variance): ' + kparam)
    leg = ax.legend(loc='upper right', fancybox=True, shadow=True) 
    leg.get_frame().set_alpha(0.4)
    ax.grid()
    
    fig.tight_layout()
    plt.savefig(kparam + '.png')


def parse_args():
    global src
    global kparam

    parser = argparse.ArgumentParser(description='Customize src csv file to generate log plot for energies.')
    parser.add_argument('-k', required=True, nargs='+',
            help='The K parameters')
    parser.add_argument('-src', required=True,
            help='The source csv file.') 

    args = parser.parse_args()
    kparam = ','.join(args.k)
    src = args.src


if __name__ == '__main__':
    parse_args()
    a = src.split('.')
    extension = a[len(a)-1]
    if extension == 'csv':
        data = read_csv()
        e_ave = [y[1] for y in data]
        e_var = [y[2] for y in data]
    elif extension == 'json':
        data = read_json()
        e_ave = [y['energy']['dynamic']['average'] for y in data] 
        e_var = [y['energy']['dynamic']['variance'] for y in data] 
    else:
        sys.exit('Error: ' + extension + ' file is not supported.')
    plot(len(data), e_ave, e_var)

