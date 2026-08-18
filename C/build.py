from setuptools import Extension, setup
import numpy as np

# also change this name
conditions = Extension("conditions", sources=["conditions.c"])
charges = Extension("charges", sources=["charges.c"])

setup(
    name="C_Implementation",
    description="C implementation of various functions. MIGHT provide minimal speedups idk",
    ext_modules=[conditions, charges],
    include_dirs=[np.get_include()]
)