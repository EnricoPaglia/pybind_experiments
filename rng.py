import numpy as np
import numpy.random

seed = 10
gen = np.random.Generator(np.random.MT19937(seed))


# state = gen.__getstate__()
# print(gen.uniform(0,1,1), "\n", "\n")
# gen.__setstate__(state)
# print(gen.uniform(0,1,1), "\n", "\n")


def read_state_from_txt(fpath):
    assert fpath.split('.')[1] == 'txt'
    with open(fpath, "r") as f:
        arr = ([x for x in next(f).split()])
        pos = int(arr[-1])
        arr.pop(-1)
    return {'bit_generator': 'MT19937', 'state': {'key': np.array(arr, dtype='uint32'), 'pos': pos}}


rst = read_state_from_txt('state.txt')
gen.__setstate__(rst)


# print(gen.__getstate__())


def write_state_to_txt(gen_state, fpath):
    with open(fpath, "w") as f:
        for x in gen_state['state']['key']:
            f.write(str(x) + " ")
        f.write(str(gen_state['state']['pos']))

# wst = gen.__getstate__()['state']['key']
# write_state_to_txt(wst, 'state_py.txt')

# print(gen.integers(low=0, high=None, size=1, dtype='uint32', endpoint=False))
# print("state ", gen.__getstate__())
print("random int ", gen.integers(low=2**32-1, size=1, dtype='uint32', endpoint=False))
print("random normal ", gen.normal(loc=0, scale=2, size=1))
# print("state ", gen.__getstate__())
wst = gen.__getstate__()
write_state_to_txt(wst, 'state_new_py.txt')


# for i in range(5):
#     st = gen.__getstate__()['state']['key']
#     print("state ", st[0], st[-1])
#     print("pos ", gen.__getstate__()['state']['pos'])
#     print("random int ", gen.integers(low=2**32-1, size=1, dtype='uint32', endpoint=False))
#     # print("random int ", gen.integers(low=2**32-1, size=1, dtype='uint32', endpoint=False))
#     print("state ", st[0], st[-1])
#     print("pos ", gen.__getstate__()['state']['pos'])
#     print("--------------------------------------")

# cpp_new = read_state_from_txt("state_new_cpp.txt")['state']['key']
# py_new = read_state_from_txt("state_new_py.txt")['state']['key']
#
# print("#####", np.array_equal(cpp_new, py_new))