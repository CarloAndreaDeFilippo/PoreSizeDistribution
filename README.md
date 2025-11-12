# PoreSizeDistribution

C++11 program that computes the pore distribution of a system of spheres (representing polymer beads), parallelized with OpenMP.

## Theoretical background

Given a system of polymers with explicit solvent, the goal of this program is to compute the system pore distribution, i.e. the distribution of the pores created by the presence of the solvent. The program follows the algorithm described in [^1].

Given a random point P (chosen to be not close to the spheres), the algorithm computes the biggest sphere that contains P but does not overlap with the spheres, representing the pore size in that point. By doing this procedure ```stepsToAverage``` times, the pore size histogram is computed.

The stopping criteria is based on the normalized difference between the old pore size histogram and the new one:

$$\overline{Error} = \left\langle \frac{PSD_{new}(D) - PSD_{old}(D)}{PSD_{new}(D)} \right\rangle$$

## Installation

Currently, only Linux is supported.

### Requirements

* C++11 compiler (g++)
* OpenMP-enabled compiler (for parallelization)
* [NLopt](https://github.com/stevengj/nlopt) for nonlinear optimization, follow their installation instructions. Alternatively, install libnlopt0 package on Linux (sudo apt-get install libnlopt0).

To build:

```
cd PoreSizeDistribution
make -j
```

To use once built (with all available threads):

```
./psd.out SETTINGS_FILE
```

To select the number of threads, use:

```
OMP_NUM_THREADS=NUMBER_OF_THREADS ./psd.out SETTINGS_FILE
```

### Settings file

The file ```SETTINGS_FILE```, in the JSON format (see [nlohmann json](https://github.com/nlohmann/json)), contains all simulation-related information. An example  ```settings.json``` is provided.
In particular, it is possible to choose:
* ```confFileName```: the name of the file for the particle configuration.
* ```dataFolder```: the name of the folder for the output data.
* ```"useCogli2"```: choose whether to save a visualization file compatible with [cogli2](https://sourceforge.net/projects/cogli1/). If set to true, it is possible to change other settings such as
    * ```cogliFile```: file name of cogli2 output.
    * ```sphereColor```: string that represents the color of the spheres to represent the polymer beads in RGBA (from 0 to 1), _e.g._ ```"1,0,0,1"``` for blue spheres.
    * ```poreSphereColor```: string that represents the color of the spheres to represent the pore.
    *  ```maxCogliSpheres```: maximum number of spheres to save into the cogli2 file.
* ```errorTolerance```: tolerance on the normalized error between the new pore distribution and the old one. If the error is lower than the chosen tolerance, the simulation stops. Default value is ```1e-2```.
* ```stepsToAverage```: number of steps between the computation of the average error between the old distribution and the new distribution.
* ```maxLoops```: max number of loops to average. If the simulation is not converging fast enough, _i.e._ the loop number is higher than the max, it stops.


### Input file

The input file describes the system. The first row of the input file contains the box dimensions $(Lx, Ly, Lz)$, e.g. ```50 50 50```. The system is centered around $(0, 0, 0)$, meaning that each axis range is $(-L_i/2, L_i/2)$. Each subsequent row represents the position of the center of mass of each sphere in the system $(x, y, z)$.

Currently, the program assumes the spheres to be all the same with diameter set to 1, but this will be changed to account for sets of spheres of different sizes.

There is an example configuration in the ```docs/ExampleConfiguration``` folder, together with a simple lammpstrj and a Python script to convert the trajectory to usable data.

### Output

The program computes the distribution and returns the Histogram.txt in the chosen ```dataFolder```. The first column represents the pore diameter, the second one its frequency.

Moreover, it returns a cogli2 file to visualize the pore in the Cogli2 folder. The spheres of the input system are by default visualized in blue, while the pore is represented in red:

<img src="/docs/PoreExample.png" width=50% height=50%>

[^1]: Langmuir 2006, 22, 18, 7726-7731, [https://doi.org/10.1021/la052651k](https://doi.org/10.1021/la052651k)
