import numpy as np
import numpy.typing as npt

def anomaly_check_quadratic(q_charges:npt.NDArray, l_charges:npt.NDArray, e_charges:npt.NDArray,
                  u_charges:npt.NDArray, d_charges:npt.NDArray, v_charges:npt.NDArray):
    return np.sum(q_charges**2 - l_charges**2 + e_charges**2 - 2*u_charges**2 + d_charges**2)

def anomaly_check_cubic(q_charges:npt.NDArray, l_charges:npt.NDArray, e_charges:npt.NDArray,
                  u_charges:npt.NDArray, d_charges:npt.NDArray, v_charges:npt.NDArray):
    return np.sum(6*q_charges**2 + 2*l_charges**2 - e_charges**2 - 3*u_charges**2 - 3*d_charges**2 - v_charges**3)

def yukawa_check(charges_sum:npt.NDArray):
    q_sum = charges_sum[0]
    l_sum = charges_sum[1]
    e_sum = charges_sum[2]
    u_sum = charges_sum[3]
    d_sum = charges_sum[4]
    v_sum = charges_sum[5]

    if (2*q_sum - u_sum - d_sum != - l_sum + e_sum + 3*q_sum + v_sum):
        return False
    return True

def multiple_check(found_charges:npt.NDArray, curr_charges:npt.NDArray):
    # Assumes charges are already sorted in order of increasing magnitude
    for i in range(6):
        if (np.any(np.logical_xor(found_charges[i,:], curr_charges[i,:]), axis=None)):
            return False
        factors = np.array([found_charges[i,j] // curr_charges[i,j]
                            if np.logical_and(found_charges[i,:], curr_charges[i,:])[j] else 0
                            for j in range(3)])
        if np.unique(factors).size > 1:
            return False

    return True