import argparse
import csv
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


def plot(d):
    x = np.arange(int(d[len(d)-1][0])+1)

    fig, ax = plt.subplots()
   
    # log y axis
    ax.semilogy(x, [y[1] for y in d], label='Average') 
    ax.semilogy(x, [y[2] for y in d], label='Variance') 
    ax.set(title='Microscopic Average and Variance Energy: ' + kparam)
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
    data = read_csv()
    plot(data)

