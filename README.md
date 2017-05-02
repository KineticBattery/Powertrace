# Powertrace with Kibam Battery model

This code we added the Kinetic Battery model in the original Powertrace tool. So, it is possible to know periodically the remaining energy in the nodes. In harvesting or non-harvesting scenarios, the kinetic model is useful to measure the nodes' lifetime and/or the non-linear residual energy.

This Kinetic Battery code has been tested on COOJA - Contiki.

This is a portion of code used to implement a Solution named: Neutral Operation of the Minimum Energy Node in Energy-Harvesting Environments. This work has been accepted in ISCC 2017, soon it will be in ieeexplorer.

Please, fell free to modify the code to your needs and contact me any time to clear doubts or to upload improved versions of this code. ariker(at)dei.uc.pt



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


#include "powertrace.h"


Then, call the following powertrace function inside the PROCESS_BEGIN() code:


PROCESS_BEGIN();
...

powertrace_start(CLOCK_SECOND * seconds, seconds, fixed_perc_energy, variation);
...

PROCESS_PAUSE();

In the Makefile you have to add:

TARGET_LIBFILES += -lm

and also:

APPS += powertrace

<b>Step 6) </b>

Run Cooja simulation

To Watch a short video about this step, open the following link as a new tab:
https://vimeo.com/215535590

*If you have problems with MSPSIM running Cooja the first time, download the MSPSIM folder from Contiki's Git Hub and paste the files in Contiki/tools/MSPSIM*
