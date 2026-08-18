import numpy as np
import numpy.typing as npt
import gymnasium as gym
from charge_space import Charge_Space
from C.conditions import *
from C.charges import *

class Charge_Env(gym.Env):
    def __init__(self, max_charge=5, rh_v=True, seed=None):
        self.max_charge = max_charge
        self.rh_v = rh_v
        self._np_random_seed = seed
        self._np_random = np.random.default_rng(seed)

        self.observation_space = Charge_Space(max_charge=5, rh_v=True)
        self.action_space = gym.spaces.Dict(
                                {
                                 "choose_particle":gym.spaces.Discrete(6, dtype=np.int32),
                                 "choose_generation":gym.spaces.Discrete(2, dtype=np.int32),
                                 "modify_charge":gym.spaces.Discrete(2, dtype=np.int32),
                                }
                            )
        self.charges : npt.NDArray
        self.charges_sum : npt.NDArray
        self.charges_bar : npt.NDArray
        self.charges_32 : npt.NDArray
        
        self.rewards_sum = 0.0
        self.steps = 0
        # self.particles = {"q":0, "l":1, "e":2, "u":3, "d":4, "v":5} unused, here for reference
        self.modifications = {0:-1, 1:1}

    def upd_charges(self, charges):
        scharges_sum, charges_bar, charges_32 = get_charges_properties(charges)

        for i in range(1, 6): 
            charges[i, 2] = self.charges_sum[i] - charges[i, 0] - charges[i, 1]
            if (np.abs(charges[i, 2]) > np.abs(charges[i, 1])):
                # penalty for going oob?
                continue
        else:
            return charges

    def _get_obs(self):
        return f"current_charges:\n{self.charges}"

    def _get_info(self):
        return f"quadratic coeff: {anomaly_quadratic(self.charges)}\ncubic coeff: {anomaly_cubic(self.charges)}\n"

    def step(self, action):
        chosen_particle = action["choose_particle"]
        chosen_generation = action["choose_generation"]
        chosen_modification = action["modify_charge"]

        self.charges[chosen_particle, chosen_generation] += self.modifications[chosen_modification]
        self.upd_charges

        # unfinished bc what truncation condition is there

        return self._get_obs(), self._get_info()#, reward, terminated, truncated

    def reset(self, seed=0):
        super().reset(seed=seed)

        self.charges, self.charges_sum, self.charges_bar, self.charges_32 = self.observation_space.sample()

        return self._get_obs(), self._get_info()