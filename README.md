# PoreSizeDistribution

C++11 program that computes the pore distribution of a system of spheres (representing polymer beads), parallelized with OpenMP.

## Theoretical background

Given a system of polymers with explicit solvent, the goal of this program is to compute the system pore distribution, i.e. the distribution of the pores created by the presence of the solvent. The program follows the algorithm described in [^1].

## Installation

Currently, only Linux is supported.

### Requirements

* C++11 compiler (g++)
* OpenMP-enabled compiler (for parallelization)
* [NLopt](https://github.com/stevengj/nlopt) for nonlinear optimization, follow their installation instructions. NB: I had to install libnlopt0 package on Linux after installing NLopt to run the code (sudo apt-get install libnlopt0).

To build:

```
cd PoreSizeDistribution
make -j
```

To use once built (with all available threads):

```
./psd.out CONFIGURATION_FILE
```

To select the number of threads, use:

```
OMP_NUM_THREADS=NUMBER_OF_THREADS ./psd.out CONFIGURATION_FILE
```

### Input file

The input file describes the system. The first row of the input file contains the box dimensions $(Lx, Ly, Lz)$, e.g. ```50 50 50```. The system is centered around $(0, 0, 0)$, meaning that each axis range is $(-L_i/2, L_i/2)$. Each subsequent row represents the position of the center of mass of each sphere in the system $(x, y, z)$.

At the moment the program assumes the spheres to be all the same with diameter set to 1, but this will be changed to account for sets of spheres of different sizes.

In the ```docs/ExampleConfiguration``` folder there is a example configuration, together with a simple lammpstrj and a Python script to convert the trajectory to usable data.

### Output

The program computes the distribution and returns the Histogram.txt in the Data folder.

Moreover, it returns a [cogli2](https://sourceforge.net/projects/cogli1/) file to visualize the pore in the Cogli2 folder. The spheres of the input system are visualized in blue, while the pore is represented in red:

<img src="/docs/PoreExample.png" width=50% height=50%>

[^1]: Langmuir 2006, 22, 18, 7726-7731, [https://doi.org/10.1021/la052651k](https://doi.org/10.1021/la052651k)
