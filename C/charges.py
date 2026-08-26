import numpy as np
import numpy.typing as npt
import C.charges as charges

def get_sorted_charges(charges:npt.NDArray):
    return charges.get_sorted_charges(charges)

def get_higgs_charge(charges_sum:npt.NDArray):
    return charges.get_higgs_charge(charges_sum)

def get_charges_properties(charges:npt.NDArray):
    return charges.get_charges_properties(charges)