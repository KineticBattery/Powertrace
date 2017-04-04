# Contiki

This Kinetic Battery code has been tested on COOJA - Contiki.

This is a portion of code used to implement a Solution named: Neutral Operation of the Minimum Energy Node in Energy-Harvesting Environments. Please, fell free to modify the code to your needs and contact me any time to clear doubts or to upload improved versions of this code.

For this implementation (battery_kinetic.c), the Kinetic Battery code uses the Energest variables to estimate the amount of energy consumed. Besides, this code loads an array of solar measurements. The harvested energy can be discarted in battery_kinetic.c.

The file all-in-one.c is an example of solar array, created to provide 156 values considering 13 hours per day, which means that each measurement longs 5 min. Due to the solar measurements, the code has to keep track of the time. This array contains values of solar irradiance in micro-Watt/cm2. In total, the solar trace cover 156x5=780 minutes of sun linght, which is 13 hours.

The CoapServer-DataAggregation.c contains the commands that starts the Kinetic code.
