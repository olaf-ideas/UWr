import sys
import numpy as np
import random

GAMMA = 0.99
ALPHA = 0.66

def update(state, action, is_oil):
    x, y, vx, vy = state
    dvx, dvy = action
    
    if is_oil:
        move_list = []
        for rx in [-1, 0, +1]:
            for ry in [-1, 0, +1]:
                nvx = max(-3, min(+3, vx + dvx + rx))
                nvy = max(-3, min(+3, vy + dvy + ry))
                nx = x + nvx
                ny = y + nvy
                
                move_list.append((nx, ny, nvx, nvy))
        return move_list
    
    nvx = max(-3, min(+3, vx + dvx))
    nvy = max(-3, min(+3, vy + dvy))
    nx = x + nvx
    ny = y + nvy
    
    return [(nx, ny, nvx, nvy)]
task_file = sys.argv[1]

track = []
for line in open(task_file):
    row = list(line.strip())
    if len(row) > 0:
        track.append(row)

rows, cols = len(track), len(track[0])

# print(track)
# print(rows, cols)

V = np.zeros((rows, cols, 7, 7))
policy = {}

actions = [(dvx, dvy) for dvx in [-1, 0, +1] for dvy in [-1, 0, +1]]
states = [(x, y, vx, vy) for x in range(rows)
                         for y in range(cols)
                         for vx in range(-3, +3 + 1)
                         for vy in range(-3, +3 + 1)]

for rep in range(10000):
    
    delta = 0
    move_changes = 0
    for state in states:
        x, y, vx, vy = state
        
        if track[x][y] in ('.', 'e'):
            # car is out of the track
            continue
        
        new_v = -float('inf')
        best_action = None
        
        for action in actions:
            action_v = 0

            next_states = update(state, action, track[x][y] == 'o')
            for next_state in next_states:
                
                # print(next_state)
                nx, ny, nvx, nvy = next_state

                if 0 <= nx < rows and 0 <= ny < cols:
                    if track[nx][ny] == '.':
                        reward = -100
                    elif track[nx][ny] == 'e':
                        reward = +100
                    else:
                        reward = -1
                    
                    # print(reward, x, y, vx, vy, "=>", nx, ny, nvx, nvy)
                
                    action_v += reward + GAMMA * V[nx, ny, nvx + 3, nvy + 3]
                else:
                    action_v += -100

            action_v /= len(next_states)

            if new_v < action_v:
                new_v = action_v
                best_action = action
        
        if best_action == None:
            continue
        
        old_v = V[x, y, vx + 3, vy + 3]
        
        delta = max(delta, abs(old_v - new_v))
        
        V[x, y, vx + 3, vy + 3] = (1 - ALPHA) * old_v + ALPHA * new_v
        
        if state not in policy or policy[state] != best_action:
            move_changes += 1
            
        policy[state] = best_action
    
    print(rep, "delta:", delta, "changes:", move_changes)
    
    if delta < 1e-12 and move_changes == 0:
        break
    # for x in range(rows):
    #     for y in range(cols):
    #         best_score = -float('inf')
    #         for vx in range(-3, +3 + 1):
    #             for vy in range(-3, +3 + 1):
    #                 if best_score < V[x, y, vx, vy]:
    #                     best_score = V[x, y, vx, vy]
    #         print(format(best_score, '.0f'), end=' ')
    #     print()

with open("policy_for_" + task_file, "w") as f:
    for state, action in policy.items():
        if action:
            f.write(f"{state[1]} {state[0]} {state[3]} {state[2]}   {action[1]} {action[0]}\n")
