#!/usr/bin/env python3

# Change lines 55 and 60 to test different solvers

import sys
import os
import re

scenes=[f'medium{x}' for x in range(1, 6)]

versions = [3, 5]
perfs = [[None] * (2*len(versions)) for _ in range(len(scenes))]

os.system('mkdir -p output')
os.system('rm -rf output/*')
for i, (scene_name) in enumerate(scenes):
    for j, version in enumerate(versions):
        print(f'--- running {scene_name} v{version} ---')
        init_file = f'input_random/medium/{scene_name}.txt'
        output_file = f'output/{scene_name}.txt'
        cmd = f'./counter_sequential_v{version} --file {init_file} >> {output_file}'
        ret = os.system(cmd)
        assert ret == 0, 'ERROR -- solver-sequential exited with errors'
        t_seq = float(re.findall(r'sequential time to solve: (.*?)s', open(output_file).read())[j])
        print(f'sequential v{version} time to solve: {t_seq:.9f}s\n')
        cmd = f'./counter_parallel_v{version} --file {init_file} >> {output_file}'
        ret = os.system(cmd)
        assert ret == 0, 'ERROR -- solver-parallel exited with errors'
        t_par = float(re.findall(r'parallel time to solve: (.*?)s', open(output_file).read())[j])
        print(f'parallel v{version} time to solve: {t_par:.9f}s\n')
        perfs[i][0 + j*2] = t_seq
        perfs[i][1 + j*2] = t_par

print('\n-- Performance Table ---')
header = '|'.join(f' {x:<15} ' for x in ['Scene Name'])
print('| '.join([header] + sum([[f'sequential_v{x}   ', f'parallel_v{x}     '] for x in versions], [])))
print('-' * ((len(header)+1) * (1 + 2 * len(versions))))
for scene, perf in zip(scenes, perfs):
    print('|'.join(f' {x:<15} ' for x in [scene] + perf))
