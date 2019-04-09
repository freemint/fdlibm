This is a first version of a testsuite for fdlibm for 680x0.

Part of the data for the tests were orginally taken from an
older release of GLIBC, but many additional test cases for
corner-cases have been added.

The tests where originally designed to test emulator behaviour
of FPU instructions, and had already been verified by running them
on real hardware. However, since the FPU internally only uses long
doubles, and fdlibm currently only handles (64-bit) doubles, the
tests were adjusted.

Some failures have already been identified, and are currently skipped
to concentrate on any remaining issues and to prevent regressions.

Note that the constants involved in the tests are still defined as
long doubles, and are converted during the tests. This is not perfect,
and might change in a later release. For the same reason, the Coldfire
versions might not work yet at all, because the Coldfire has no long double
format.
