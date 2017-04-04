# Contiki

This Kinetic Battery code has been tested on COOJA - Contiki.

For this implementation (battery_kinetic.c), the Kinetic Battery code uses the Energest variables to estimate the amount of energy consumed. Besides, this code loads an array of solar measurements.

The file all-in-one.c is an example of solar array, created to provide 156 values considering 13 hours per day, which means that each measurement longs 5 min. Due to the solar measurements, the code has to keep track of the time.

The CoapServer-DataAggregation.c contains the commands that starts the Kinetic code.
