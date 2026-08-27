import numpy as np
from src.neural_net import *
import torch

num_iterations = 512
num_transitions = 200
minibatch_size = 20
num_epochs = 50

lr = 1e-4
step_size = 15
lr_gamma = 0.2
gamma = 0.99
lmbda = 0.95
clip_epsilon = 0.5
entropy_coef = 0.02

max_charge = 5
max_steps = 25

actor = ActorCritic()
agent = PPO(num_epochs, num_transitions, minibatch_size, lr, step_size, lr_gamma, gamma, lmbda, clip_epsilon, entropy_coef)
env = Charge_Env(max_charge, max_steps)
found_charges = []

log_file = open("./found_charges.txt", "w")
log_file.write("   Q   Q   Q   n   n   n   e   e   e   u   u   u   L   L   L   d   d   d\n")

for i in range(num_iterations):
    print(f"Currently on iteration {i+1} of {num_iterations}")

    states = []
    actions = []
    log_probs = []
    entropies = []
    rwd_func = []
    ended = []
    vals = []

    for j in range(num_transitions):
        state = torch.tensor(env.charges, dtype=torch.float32)
        states.append(state)

        particle_logits, generation_logits, mod_logits, val = actor(torch.unsqueeze(state, 0))
        
        particle_distr = torch.distributions.Categorical(logits=particle_logits)
        chosen_particle = particle_distr.sample()
        particle_log_prob = particle_distr.log_prob(chosen_particle)

        if (chosen_particle.item() < 2):
            # To avoid picking 3rd charge for non doublet and neutrino
            generation_logits.masked_fill_(torch.tenzor([False, False, 1e-9]))

        generation_distr = torch.distributions.Categorical(logits=generation_logits)
        chosen_generation = generation_distr.sample()
        generation_log_prob = generation_distr.log_prob(chosen_generation)

        mod_distr = torch.distributions.Categorical(logits=mod_logits)
        chosen_mod = mod_distr.sample()
        mod_log_prob = mod_distr.log_prob(chosen_mod)

        action = torch.stack((chosen_particle, chosen_generation, chosen_mod))
        log_prob = torch.sum(torch.stack((particle_log_prob, generation_log_prob, mod_log_prob)), 0)
        val = torch.flatten(val)

        actions.append(action)
        log_probs.append(log_prob)
        vals.append(val)

        state, reward, terminated, truncated, info = env.step(action, found_charges, log_file)
        rwd_func.append(reward)

        ended.append(int(terminated or truncated))

        if terminated or truncated or j == num_transitions - 1:  # Last non terminal / truncated state also requires the next value
            state = torch.tensor(state, dtype=torch.float32)
            vals.append(int(not terminated) * actor.forward(torch.unsqueeze(state, 0), value_only=True))   # Terminated states will hvae zero value

            env.reset()

    agent.states = torch.stack(states).detach()
    agent.actions = torch.stack(actions).detach()
    agent.log_probs = torch.stack(log_probs).detach()
    agent.vals = torch.stack(vals).detach()
    agent.rewards = torch.tensor(rwd_func)
    agent.ended = torch.tensor(ended)

    agent.calc_gae_tar()

    for j in range(num_epochs):
        print(f"\t Epoch {j+1} of {num_epochs}")
        agent.upd(torch.randperm(num_transitions))

log_file.close()