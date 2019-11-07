import argparse
import csv
import json
import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
from sklearn import mixture
from sklearn.cluster import KMeans
import seaborn as sns


src = ''
out = ''


def read_json():
    with open(src) as f:
        data = json.load(f)
    return data


def plot(n, y):
    fig, ax = plt.subplots()

    #x = np.arange(n)
    x = np.arange(0, 100*n, 100)


    y = [list(step.values()) for step in y]
    # x: 1d array of dimension N.
    # y: 2d array (dimension MxN), or sequence of
    #    1d arrays (each dimension 1xN)
    ax.stackplot(x, list(zip(*y)), labels=labels, colors=pal)
    leg = ax.legend(loc='upper right', prop={'size': 6})
    leg.get_frame().set_alpha(0.4)

    plt.savefig(out)


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
    points = [d['points'] for d in data]
    step = 0
    X = np.array([[p['x'], p['y']] for p in points[step]])

    random_state=0
    y_pred = KMeans(n_clusters=6, random_state=random_state).fit_predict(X)

    plt.subplot(221)
    plt.scatter(X[:, 0], X[:, 1], c=y_pred)
    plt.title("Incorrect Number of Blobs")

    # Anisotropicly distributed data
    transformation = [[0.60834549, -0.63667341], [-0.40887718, 0.85253229]]
    X_aniso = np.dot(X, transformation)
    y_pred = KMeans(n_clusters=3, random_state=random_state).fit_predict(X_aniso)

    plt.subplot(222)
    plt.scatter(X_aniso[:, 0], X_aniso[:, 1], c=y_pred)
    plt.title("Anisotropicly Distributed Blobs")

    plt.show()



    #print(X_train)
    #clf = mixture.GaussianMixture(n_components=2, covariance_type='full')
    #clf.fit(X_train.reshape(-1, 1))

    #x = np.linspace(-20., 30.)
    #y = np.linspace(-20., 40.)
    #X, Y = np.meshgrid(x, y)
    #XX = np.array([X.ravel(), Y.ravel()]).T
    #Z = -clf.score_samples(XX)
    #Z = Z.reshape(X.shape)

    #CS = plt.contour(X, Y, Z, norm=LogNorm(vmin=1.0, vmax=1000.0),
                             #levels=np.logspace(0, 3, 10))
    #CB = plt.colorbar(CS, shrink=0.8, extend='both')
    #plt.scatter(X_train[:, 0], X_train[:, 1], .8)

    #plt.title('Negative log-likelihood predicted by a GMM')
    #plt.axis('tight')
    #plt.show()

    #plot(len(data), y)




