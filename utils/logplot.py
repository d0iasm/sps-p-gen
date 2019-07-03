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
    # Replace directory and extension.
    extension = src.split('.')[len(a)-1]
    dest = 'img/energy?' + src.split('/')[1]
    plt.savefig(dest.replace(extension, 'png'))


def parse_args():
    global src
    global kparam

    parser = argparse.ArgumentParser(
            description='Generate an image from a csv/json file.')
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

