# PowertraceK
## It is a Powertrace extension that implements the Kibam Battery model

### Features of PowertraceK:
###  1. It measures the energy consumption in Receive, Transmit, Low power and idle.
###  2. It provides the remaining energy in the battery of each node.
###  3. It can be easily used for energy-harvesting nodes, where there is a charging electrical current.

This code adds the Kinetic Battery model in the original Powertrace tool. So, it is possible to know periodically the remaining energy in the nodes. In harvesting or non-harvesting scenarios, the kinetic model is useful to measure the nodes' lifetime and/or the non-linear residual energy.

The PowertraceK code has been developed and tested on COOJA - Contiki.

This is a portion of code used to implement a Solution named: Neutral Operation of the Minimum Energy Node in Energy-Harvesting Environments. This work has been accepted in ISCC 2017, soon it will be in ieeexplorer.


If you use PowertraceK, don't forget to use the following bibtex entry to cite our paper.
```
@INPROCEEDINGS{riker2017iscc, 
author={Riker, Andr{\'e} and and Curado, Marilia and Monteiro, Edmundo},
booktitle={2017 IEEE Symposium on Computers and Communication (ISCC)}, 
title={Neutral Operation of the Minimum Energy Node in Energy-Harvesting Environments}, 
year={2017}, 
pages={1-6},  
month={July}}
```


Please, feel free to modify the code to your needs and contact me any time to clear doubts or to upload improved versions of this code. My contact is ariker(at)dei.uc.pt

![Screenshot](example.png)

<b>Step 1) </b> 

Download Instant Contiki

https://sourceforge.net/projects/contiki/files/Instant%20Contiki/

To Watch a short video about this step, open the following link as a new tab:
https://vimeo.com/215535613

<b>Step 2) </b> 

Download or clone our git hub project 

To Watch a short video about this step, open the following link as a new tab:
https://vimeo.com/215535637


<b>Step 3) </b> 

Run the downloaded Instant Contiki Virtual Machine</b>

To Watch a short video about this step, open the following link as a new tab:
https://vimeo.com/215535582

<b>Step 4) </b> 

Replace the apps/powertrace files in the original Contiki by the ones provided in our git hub project

To Watch a short video about this step, open the following link as a new tab:
https://vimeo.com/215535658

<b>Step 5) </b> 

To run an example, replace the examples/er-rest-example files in the original Contiki by the ones provided in our git hub project


If you want to use the our powertrace in other application code, you just have to add the following include: 


```
#include "powertrace.h"
```


Then, call the following powertrace function inside the PROCESS_BEGIN() code:


```
PROCESS_BEGIN();
...

powertrace_start(CLOCK_SECOND * seconds, seconds, fixed_perc_energy, variation);
...

PROCESS_PAUSE();
```

In the Makefile you have to add:

```
TARGET_LIBFILES += -lm
```

and also add in the Makefile:

```
APPS += powertrace
```

<b>Step 6) </b>

Run Cooja simulation

To Watch a short video about this step, open the following link as a new tab:
https://vimeo.com/215535590

*If you have problems with MSPSIM running Cooja the first time, download the MSPSIM folder from Contiki's Git Hub and paste the files in Contiki/tools/MSPSIM*
