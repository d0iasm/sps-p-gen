from PIL import Image


l1 = Image.open("../images/class-by-hand.png")
l2 = Image.open("../images/Average of Macro Energy.png")
r1 = Image.blend(l1, l2, alpha=.7)
r1.save("../images/class-with-macro-energy-average.png")

l3 = Image.open("../images/Variance of Macro Energy.png")
r2 = Image.blend(l1, l3, alpha=.7)
r2.save("../images/class-with-macro-energy-variance.png")
