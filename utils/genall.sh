#!/bin/bash


# Class.
python3 heatmap.py -z class -title "Class based on X and V"

# Macroscopic energy is same for open boundary and periodic boundary.
python3 heatmap.py -z energy-average -title "Average of Macro Energy" -src ../csv/open-boundary.csv
python3 heatmap.py -z energy-variance -title "Variance of Macro Energy" -src ../csv/open-boundary.csv

