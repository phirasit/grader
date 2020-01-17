#!/bin/python3
import random
import os
import sys

random.seed(0)
try:
    os.mkdir("tests")
except FileExistsError:
    pass

for i in range(5):
    a, b = random.randint(1, 100), random.randint(1, 100)
    print("generating test {}".format(i+1), file=sys.stderr)
    ab = a + b
    inp_file = "tests/" + str(i+1) + ".in"
    sol_file = "tests/" + str(i+1) + ".sol"
    with open(inp_file, "w") as inp, open(sol_file, "w") as sol:
        inp.write("{} {}\n".format(a, b))
        sol.write("{}\n".format(ab))

