#!/usr/bin/env python3

import sys
import os
import re

filepath = __file__.split('/')
filepath.pop()
filepath.append("input")
files = os.listdir("/".join(filepath))

orig_scenes = (list(map(lambda x: '.'.join(x.split('.')[:-1]), files)))

exclude = ['none', 'hard2', 'example2', 'example2b']
scenes = []
for name in orig_scenes:
    if name in exclude:
        continue
    scenes.append(name)

perfs = [[None] * 2 for _ in range(len(scenes))]

# def compare(actual, ref):
#     threshold = 1.0 if 'repeat' in actual else 0.1
#     actual = open(actual).readlines()
#     ref = open(ref).readlines()
#     assert len(actual) == len(ref), \
#         f'ERROR -- number of particles is {len(actual)}, should be {len(ref)}'
#     for i, (l1, l2) in enumerate(zip(actual, ref)):
#         l1 = [float(x) for x in l1.split()]
#         l2 = [float(x) for x in l2.split()]
#         assert len(l2) == 5 and len(l1) == len(l2),\
#             f'ERROR -- invalid format at line {i}, should contain {len(l2)} floats'
#         assert all(abs(x - y) < threshold for x, y in zip(l1, l2)),\
#             f'ERROR -- incorrect result at line {i}'


# def compute_score(actual, ref):
#     # actual <= 1.2 * ref: full score
#     # actual >= 3.0 * ref: zero score
#     # otherwise: linear in (actual / ref)
#     return min(max((3.0 - actual / ref) / 1.8, 0.0), 1.0)


os.system('mkdir -p output')
os.system('rm -rf output/*')
for i, (scene_name) in enumerate(scenes):
    if scene_name in exclude:
        continue
    print(f'--- running {scene_name} ---')
    init_file = f'input/{scene_name}.txt'
    output_file = f'output/{scene_name}.txt'
    cmd = f'./solver-sequential --file {init_file} >> {output_file}'
    ret = os.system(cmd)
    assert ret == 0, 'ERROR -- solver-sequential exited with errors'
    t_seq = float(re.findall(r'sequential time to solve: (.*?)s', open(output_file).read())[0])
    print(f'sequential time to solve: {t_seq:.9f}s\n')
    cmd = f'./solver-parallel --file {init_file} >> {output_file}'
    ret = os.system(cmd)
    assert ret == 0, 'ERROR -- solver-parallel exited with errors'
    t_par = float(re.findall(r'parallel time to solve: (.*?)s', open(output_file).read())[0])
    print(f'parallel time to solve: {t_par:.9f}s\n')
    perfs[i][0] = t_seq
    perfs[i][1] = t_par

print('\n-- Performance Table ---')
header = '|'.join(f' {x:<15} ' for x in ['Scene Name'])
print(header)
print('-' * len(header))
for scene, perf in zip(scenes, perfs):
    if scene in exclude:
        continue
    print('|'.join(f' {x:<15} ' for x in [scene] + perf))
