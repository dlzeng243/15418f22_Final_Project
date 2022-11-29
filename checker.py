#!/usr/bin/env python3

import sys
import os
import re

scenes = (
    ('example'),
    ('four_T'),
    # ('none'),
    ('penta'),
    ('simple_IJLO')
)

scene_names = (
    'example', 
    'four_T', 
    # 'none', 
    'penta',
    'simple_IJLO'
)

perfs = [[None] for _ in range(len(scenes))]

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
    print(f'--- running {scene_name} ---')
    init_file = f'input/{scene_name}.txt'
    output_file = f'output/{scene_name}.txt'
    cmd = f'./solver-sequential --file {init_file} >> {output_file}'
    ret = os.system(cmd)
    cmd = f'./solver-parallel --file {init_file} >> {output_file}'
    ret = os.system(cmd)
    assert ret == 0, 'ERROR -- nbody exited with errors'
    # t = float(re.findall(
    #     r'total simulation time: (.*?)s', open(output_file).read())[0])
    # print(f'total simulation time: {t:.6f}s')
    # perfs[i][j] = t

# print('\n-- Performance Table ---')
# header = '|'.join(f' {x:<15} ' for x in ['Scene Name'] + version)
# print(header)
# print('-' * len(header))
# for scene, perf in zip(scenes, perfs):
#     print('|'.join(f' {x:<15} ' for x in [scene[0]] + perf))
