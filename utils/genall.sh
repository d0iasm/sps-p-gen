#!/bin/bash


# Class.
python heatmap.py -z class -title "Class based on X and V"

python heatmap.py -z energy-average -title "Average of Macro Energy (open)" -src ../csv/open-boundary.csv
python heatmap.py -z energy-variance -title "Variance of Macro Energy (open)" -src ../csv/open-boundary.csv
python heatmap.py -z energy-average -title "Average of Macro Energy (periodic)" -src ../csv/periodic-boundary.csv
python heatmap.py -z energy-variance -title "Variance of Macro Energy (periodic)" -src ../csv/periodic-boundary.csv
