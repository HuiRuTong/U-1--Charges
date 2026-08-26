from src.charge_space import Charge_Space
from src.charge_env import Charge_Env
from src.rwd_func import *
import torch
import numpy as np

space = Charge_Space()
env = Charge_Env()
env.charges = np.array([[-1, 0, -1], [3, 3, 0], [6, 6, 0], [-4, -4, 0], [2, 2, 0], [-1, 0, 1]], dtype=np.int32)
env._upd_charges()

print(env._get_obs())
print(env._get_info())

env.step([torch.tensor(0), torch.tensor(1), torch.tensor(0)], [])
print(env._get_obs())
print(env._get_info())