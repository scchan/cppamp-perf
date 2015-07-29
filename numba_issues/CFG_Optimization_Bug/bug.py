"""
Code to reproduce the CFG bug.
"""
from numba import hsa, intp
import numpy as np


@hsa.jit
def foo(out):
    sm = hsa.shared.array(2, dtype=intp)
    tid = hsa.get_local_id(0)
    sm[tid] = 666

    hsa.barrier(0)

    if tid == 0:
        sm[0] = 123
        sm[1] = 321

    hsa.barrier(0)
    # Uncomment the following lines to prevent the optimization bug
    # hsa.barrier(0)

    if tid == 1:
        val = sm[tid]
    else:
        val = 456

    hsa.barrier(0)
    out[tid] = val


def test():
    out = np.zeros(2, dtype=np.intp)
    foo[1, 2](out)
    print(out)
    assert out[0] == 456
    assert out[1] == 321


if __name__ == '__main__':
    test()
