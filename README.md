SANE Conversion Patch
=====================

By Stephen Heumann

This patch fixes a bug in SANE (the Standard Apple Numerics Environment) on the Apple IIGS. The bug can cause SANE to produce incorrect values when converting certain numbers from floating-point to integer types.

The affected numbers are certain negative multiples of 65536 (e.g. -8388608, -65536000, etc). When SANE is used to convert these numbers from floating-point types or string representations to 32-bit or 64-bit (comp) integers, the values produced will often (but not always) be incorrect. This bug can potentially affect any program that calls SANE to do such conversions, although the specific numbers that trigger it are unlikely to come up in most circumstances.

To install the patch, place the `SANEConvPatch` file in the `System:System.Setup` folder on your boot disk and then restart the system. This patch is compatible with System 6.0.1 through 6.0.4 (and potentially with future versions, unless they include a built-in fix for this problem).

There is also a test program called `SANEConvTest` which you can use to see the problem and check if the patch has fixed it. It displays the results of converting several potentially affected numbers, and also allows you to try converting other numbers. Without the patch, at least some of these conversions will typically produce incorrect results; with the patch installed, they should all be correct.

__You can download the patch [here](https://github.com/sheumann/SANEConvPatch/releases/download/v1.0/SANEConvPch.shk).__


Technical Details
-----------------

This patch works by setting certain locations on the SANE direct page to zero before performing the `FX2C`, `FX2L`, `FDEC2C`, or `FDEC2L` conversion operations. This fixes the problem, because it results from the SANE code using the contents of those locations without initializing them, and having zeros in them causes it to behave correctly.

Without the patch, the behavior will depend on what happened to be in those memory locations before. This will often be left-over data from previous SANE calls, but it may also be data from the application, or uninitialized memory left over from other programs. Because of this dependency on uninitialized memory values, the bug may or may not be triggered on any specific conversion operation, but in general it happens fairly often when converting the affected numbers.


SANE.Fix
--------

There was another SANE patch called `SANE.Fix` produced by Seven Hills Software and BrainStorm Software in 1993. It was intended to work around a different SANE bug, which existed in System 6.0 and earlier but was fixed in System 6.0.1. Because of the way `SANE.Fix` works (zeroing out the SANE direct page when SANE is started), it may avoid some instances of the bug that this patch addresses as well as some instances of the bug it was originally intended to address, but it actually does not fully fix either bug. If you are using System 6.0.1 or later and have `SANEConvPatch` installed, both bugs will be fully fixed, so there is no need to use `SANE.Fix` (although having it installed should not cause any problems).
