import argparse
import json
import numpy as np

import stackplot
import logplot
import loglogplot
import clustering

import steps_pb2


src = ''
proto = ''


def read_json():
    with open(src) as f:
        data = json.load(f)
    return data


def parse_args():
    global src
    global proto

    parser = argparse.ArgumentParser(
            description='Generate an image from a json file.')
    parser.add_argument('-src', required=True,
            help='The source file path')
    parser.add_argument('-proto', required=True,
            help='The source file path')
    parser.add_argument('-out', required=True,
            help='The output image path')

    args = parser.parse_args()
    src = args.src
    proto = args.proto
    out = args.out

    stackplot.src = args.src
    stackplot.out = out.replace("b=", "kparam_b=", 1)

    logplot.src = args.src
    logplot.out = out.replace("b=", "energy_b=", 1)

    loglogplot.src = args.src
    loglogplot.out = out.replace("b=", "xv_b=", 1)

    clustering.src = args.src
    clustering.out = out.replace("b=", "clustering_b=", 1)


if __name__ == '__main__':
    parse_args()

    steps = steps_pb2.Steps()

    # Read data from a protocol buffer binary
    f = open(proto, "rb")
    steps.ParseFromString(f.read())
    f.close()

    a = src.split('.')
    extension = a[len(a)-1]
    if extension != 'json':
        sys.exit('Error: ' + extension + ' file is not supported.')
    data = read_json()

    n = len(steps.steps)

    # Plot for K param.
    k_data = stackplot.reshape_k(data)
    #k_data_reshaped = stackplot.reshape_k_count(steps)
    #print(k_data_reshaped)
    stackplot.plot(len(data), k_data)

    # Plot for static energy.
    static_energy = [step.static_energy for step in steps.steps]
    static_energy_variance = [step.static_energy_variance for step in steps.steps]
    logplot.plot(n, static_energy, static_energy_variance, False)

    # Plot for dynamic energy.
    dynamic_energy = [step.dynamic_energy for step in steps.steps]
    dynamic_energy_variance = [step.dynamic_energy_variance for step in steps.steps]
    logplot.plot(n, dynamic_energy, dynamic_energy_variance, True)

    # Plot for XV.
    x_value = [step.x_value for step in steps.steps]
    v_value = [step.v_value for step in steps.steps]
    loglogplot.plot(n, x_value, v_value)

    # Plot for clustering.
    particles = [step.particles for step in steps.steps]
    clustering.init_text()
    for step in clustering.steps:
        if step >= len(particles):
            break
        particle = np.array([[p.x, p.y] for p in particles[step]])
        clustering.plot(particle, step*clustering.thinning)
    clustering.write_clustering_csv()
