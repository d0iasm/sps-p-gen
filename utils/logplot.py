import argparse
import sys
import numpy as np
import matplotlib.pyplot as plt
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


def plot(n, e_ave, e_var, is_dynamic):
    fig, ax = plt.subplots()

    x = np.arange(0, thinning*n, thinning)

    ax.plot(x, e_ave, label='Average')
    #ax.plot(x, e_var, label='Variance')
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

    # Plot for static energy.
    static_energy = [step.static_energy for step in data.steps]
    static_energy_variance = [step.static_energy_variance for step in data.steps]
    plot(n, static_energy, static_energy_variance, False)

    # Plot for dynamic energy.
    dynamic_energy = [step.dynamic_energy for step in data.steps]
    dynamic_energy_variance = [step.dynamic_energy_variance for step in data.steps]
    plot(n, dynamic_energy, dynamic_energy_variance, True)

