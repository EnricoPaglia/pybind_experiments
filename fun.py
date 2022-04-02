import numpy as np


def set_state_from_txt(gen, fpath):
    assert fpath.split('.')[1] == 'txt'
    with open(fpath, "r") as f:
        arr = ([x for x in next(f).split()])
        pos = int(arr[-1])
        arr.pop(-1)
    gen.__setstate__({'bit_generator': 'MT19937', 'state': {'key': np.array(arr, dtype='uint32'), 'pos': pos}})


def write_state_to_txt(gen, fpath):
    gen_state = gen.__getstate__()
    assert fpath.split('.')[1] == 'txt'
    with open(fpath, "w") as f:
        for x in gen_state['state']['key']:
            f.write(str(x) + " ")
        f.write(str(gen_state['state']['pos']))


def sample_normal(mu, sigma, gen):
    write_state_to_txt(gen, "3_py_gen_from_py_state_before.txt")
    a = gen.uniform(0,9)
    a = gen.gamma(2,2)
    a = gen.normal(mu, sigma)
    write_state_to_txt(gen, "4_py_gen_from_py_state_after.txt")
    return a
