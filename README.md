# Powertrace with Kibam Battery model

This code we added the Kibam Battery model in the original Powertrace tool. 

This Kinetic Battery code has been tested on COOJA - Contiki.

This is a portion of code used to implement a Solution named: Neutral Operation of the Minimum Energy Node in Energy-Harvesting Environments. Please, fell free to modify the code to your needs and contact me any time to clear doubts or to upload improved versions of this code.



<b>Step 1) </b> 

Download Instant Contiki

https://sourceforge.net/projects/contiki/files/Instant%20Contiki/

<p>This is <a href="https://vimeo.com/215535613" target="_blank">a link</a> that opens in a new window.</p>
<a href="http://example.com/" target="_blank">example</a>

<b>Step 2) </b> 

Download or clone our git hub project 

This is [a link](https://vimeo.com/215535613){:target="_blank"} that opens in a new window.


<b>Step 3) </b> 

Run the downloaded Instant Contiki Virtual Machine</b>

This is [a link](https://vimeo.com/215535582){:target="_blank"} that opens in a new window.

<b>Step 4) </b> 

Replace the apps/powertrace files in the original Contiki by the ones provided in our git hub project

This is [a link](https://vimeo.com/215535658){:target="_blank"} that opens in a new window.

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


<b>Step 6) </b>

Run Cooja simulation
This is [a link](https://vimeo.com/215535590){:target="_blank"} that opens in a new window.

*If you have problems with MSPSIM running Cooja the first time, download the MSPSIM folder from Contiki's Git Hub and paste the files in Contiki/tools/MSPSIM*
