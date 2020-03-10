import argparse
import steps_pb2


src = ''
out = ''


def read_proto():
    data = steps_pb2.Steps()

    # Read data from a protocol buffer binary
    f = open(src, "rb")
    data.ParseFromString(f.read())
    f.close()
    return data


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
    f = open(out, "a+")
    f.truncate(0)

    for step in data.steps: 
        text = str(step.step) + "," + str(step.static_energy) + "," + str(step.dynamic_energy) + "," + str(step.clustering)
        f.write(text + "\n")

    f.close()
