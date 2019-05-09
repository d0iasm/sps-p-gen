#!/bin/bash


# Class.
python heatmap.py -z class -title "Class based on X and V"

# Macro energy average.
python heatmap.py -z energy-average -title "Macro Energy (Average)"

# Macro energy variance.
python heatmap.py -z energy-variance -title "Macro Energy (Variance)"
