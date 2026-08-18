import numpy as np
import gymnasium as gym
from C.conditions import *
from C.charges import *

class Charge_Space(gym.spaces.Box):
    def __init__(self, max_charge=5, charges=np.zeros((6,3)), rh_v=True):
        super().__init__(low=-max_charge, high=max_charge, shape = (6, 3), dtype = np.int32)

        self.max_charge = max_charge
        self.rh_v = rh_v

    def _get_ordered_charges(self, n):
        particle_charges = []
        bounds = self.max_charge
        for i in range(n):
            particle_charges.append(self.np_random.integers(-np.abs(bounds), abs(bounds), dtype=np.int32, endpoint=True))
            bounds = particle_charges[i]

        return np.array(particle_charges, dtype=np.int32)   # for compatibility, throw a dtype=np.int32 everywhere np is used
                                                            # because C interprets int64 and in32 ptr differently

    def sample(self):
        while(1):
            q_charges = self._get_ordered_charges(3)
            if (self.rh_v):
                v_charges = self._get_ordered_charges(3)
            else:
                v_charges = np.zeros((3,), dtype=np.int32)
            l_charges = np.concat(
                        (self._get_ordered_charges(2),
                         np.zeros((1,), dtype=np.int32)), axis=0)
            e_charges = np.concat(
                        (self._get_ordered_charges(2),
                         np.zeros((1,), dtype=np.int32)), axis=0)
            u_charges = np.concat(
                        (self._get_ordered_charges(2),
                         np.zeros((1,), dtype=np.int32)), axis=0)
            d_charges = np.concat(
                        (self._get_ordered_charges(2),
                         np.zeros((1,), dtype=np.int32)), axis=0)

            charges = np.stack((q_charges, l_charges, e_charges, u_charges, d_charges, v_charges), axis=0)
            charges_sum, charges_bar, charges_32 = get_charges_properties(charges)

            for i in range(1, 6): 
                charges[i, 2] = charges_sum[i] - charges[i, 0] - charges[i, 1]
                if (np.abs(charges[i, 2]) > np.abs(charges[i, 1])):
                    break
            else:
                break
        return charges, charges_sum, charges_bar, charges_32

    def contains(self, sample):
        if not super().contains(sample):
            return False

        for i in range(6):
            if (np.abs(sample[i, 1]) > np.abs(sample[i, 0]) or np.abs(sample[i, 2]) > np.abs(sample[i, 1])):
                return False

        # equivalance checks?