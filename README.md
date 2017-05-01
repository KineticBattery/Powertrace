# Powertrace with Kibam Battery model

This code we added the Kibam Battery model in the original Powertrace tool. 

This Kinetic Battery code has been tested on COOJA - Contiki.

This is a portion of code used to implement a Solution named: Neutral Operation of the Minimum Energy Node in Energy-Harvesting Environments. Please, fell free to modify the code to your needs and contact me any time to clear doubts or to upload improved versions of this code.



Step 1) Download Instant Contiki
https://sourceforge.net/projects/contiki/files/Instant%20Contiki/

Step 2) Download or clone our git hub project

Step 3) Run the downloaded Instant Contiki Virtual Machine

Step 4) Replace the apps/powertrace files in the original Contiki by the ones provided in our git hub project

Step 5) To run an example replace the examples/er-rest-example files in the original Contiki by the ones provided in our git hub project
If you want to use the our powertrace code in other application code, you just have to add the following commands in your application file: 
#include "powertrace.h"
  PROCESS_BEGIN();
...
powertrace_start(CLOCK_SECOND * seconds, seconds, fixed_perc_energy, variation);
...
  PROCESS_PAUSE();


Step 6) Run Cooja simulation
*If you have problems with MSPSIM running Cooja the first time, download the MSPSIM folder from Contiki's Git Hub and paste the files in Contiki/tools/MSPSIM
