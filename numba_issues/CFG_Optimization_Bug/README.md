HLC CFG Optimization Bug
=========================

The Testcase
-------------

A minimal testcase is provided (bug.py).  It contains a HSA kernel "foo"
that uses shared memory to communicate.  Thread-0 (tid==0) writes values to the shared memory that is read by thread-1 (tid==1).  At the end,
each thread writes the corresponding value in the shared memory to an
output array.

The Bug
-------

Control flow optimization is duplicating barrier calls at the common
outgoing edge.  Threads are waiting at different barriers
and shared memory store is not visible by other threads.  As a result,
thread-1 does not see the the value written by thread-0 and is loading
the initial value.

The original LLVM (bad.orig.ll) only has 3 barriers.
The optimized one (bad.opt.ll) has 4.
The HSAIL code (bad.hail) also has 4.

A Workaround
------------

Having two barrier calls in a row seems to disable the invalid
duplication of the barrier into distinct control flow path.

See the "good.*" files.
