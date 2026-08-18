import numpy as np
import numpy.typing as npt

# Potentially unneeded
def get_sorted_charges(charges:npt.NDArray):
    for i in range(6):
        if (abs(charges[i, 0]) > abs(charges[i, 1])):
            charges[i, 0], charges[i, 1] = charges[i, 1], charges[i, 0]
        if (abs(charges[i, 1]) > abs(charges[i, 2])):
            charges[i, 1], charges[i, 2] = charges[i, 2], charges[i, 1]
            if (abs(charges[i, 0]) > abs(charges[i, 1])):
                charges[i, 0], charges[i, 1] = charges[i, 1], charges[i, 0]

    return charges

def get_higgs_charge(charges_sum):
    q_sum = charges_sum[0]
    v_sum = charges_sum[5]

    return (-3*q_sum - v_sum) // 3

def get_charges_properties(charges:npt.NDArray):
    charges_sum = np.zeros((6,))
    charges_bar = np.zeros((6,))
    charges_32 = np.zeros((6,))

    q_sum = np.sum(charges[0,:])
    v_sum = np.sum(charges[5,:])

    charges_sum[0] = q_sum
    charges_sum[1] = -3*q_sum
    charges_sum[2] = -6*q_sum - v_sum
    charges_sum[3] = 4*q_sum + v_sum
    charges_sum[4] = -2*q_sum - v_sum
    charges_sum[5] = v_sum

    for i in range(6):
        charges_bar[i] = charges[i, 2] + charges[i, 1] - 2*charges[i, 0]
        charges_32[i] = charges[i, 2] - charges[i, 1]

    return charges_sum, charges_bar, charges_32