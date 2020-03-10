import argparse
import numpy as np
import kparam
import energy
import xv
import clustering
import satisfaction
import steps_pb2


src = ''


def read_proto():
    data = steps_pb2.Steps()

    # Read data from a protocol buffer binary
    f = open(src, "rb")
    data.ParseFromString(f.read())
    f.close()
    return data


def parse_args():
    global src

    parser = argparse.ArgumentParser(
            description='Generate an image from a data file.')
    parser.add_argument('-src', required=True,
            help='The source file path')
    parser.add_argument('-out', required=True,
            help='The output image path')

    args = parser.parse_args()
    src = args.src
    out = args.out

    kparam.src = args.src
    kparam.out = out.replace("b=", "kparam_b=", 1)

    energy.src = args.src
    energy.out = out.replace("b=", "energy_b=", 1)

    xv.src = args.src
    xv.out = out.replace("b=", "xv_b=", 1)

    clustering.src = args.src
    clustering.out = out.replace("b=", "clustering_b=", 1)

    satisfaction.src = args.src
    satisfaction.out = out.replace("b=", "satisfaction_b=", 1)


if __name__ == '__main__':
    parse_args()
    data = read_proto()
    n = len(data.steps)

    # Plot for K param.
    kparams_count = kparam.count_k(data.steps)
    kparam.plot(n, kparams_count)

    # Plot for static energy.
    static_energy = [step.static_energy for step in data.steps]
    static_energy_variance = [step.static_energy_variance for step in data.steps]
    energy.plot(n, static_energy, static_energy_variance, False)

    # Plot for dynamic energy.
    dynamic_energy = [step.dynamic_energy for step in data.steps]
    dynamic_energy_variance = [step.dynamic_energy_variance for step in data.steps]
    energy.plot(n, dynamic_energy, dynamic_energy_variance, True)

    # Plot for XV.
    x_value = [step.x_value for step in data.steps]
    v_value = [step.v_value for step in data.steps]
    xv.plot(n, x_value, v_value)

    # Plot for clustering.
    particles = [step.particles for step in data.steps]
    # Initialize the global variable `text`.
    clustering.init_text()
    # The final step for output.
    final_step = 0
    for step in clustering.print_steps:
        i = step//clustering.thinning
        if i >= len(particles):
            break
        particle = np.array([[p.x, p.y] for p in particles[i]])
        cluster_size = clustering.plot(particle, step)
        final_step = step//clustering.thinning
    # Output the final step.
    clustering.text += str(cluster_size) + "\t" + \
        str(data.steps[final_step].static_energy) + "\t" + \
        str(data.steps[final_step].dynamic_energy) + "\t"
    clustering.write_clustering_csv()

    # Plot for satisfaction.
    satisfactions = satisfaction.reshape(data.steps)
    k_average = satisfaction.k_average(data.steps)
    satisfaction.plot(n, satisfactions, k_average)
