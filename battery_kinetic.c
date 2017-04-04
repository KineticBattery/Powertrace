// ANDRE RIKER - ARIKER AT DEI.UC.PT
// UNIVERSIDADE DE COIMBRA


//    Returns the available and bound charges for the next time step
//    Based on the Manwell and McGowan Kinetic Battery Model
//    
//    Inputs: 
//        q1_0    Available charge at beginning of time step (Ah)
//        q2_0    Bound charge at beginning of time step (Ah)
//        k       Battery rate constant (rate at which chemically bound charge becomes available)
//        c       Battery capacity ratio (fraction of total charge that is available)
//        qmax    Maximum amount of charge in battery (Ah)
//        i       Charge (-) or discharge (+) current of battery (A)
//        dt      Length of time step (hours)
//    
//    Outputs:
//        q1      Available charge at next time step (Ah)
//        q2      Bound charge at next time step (Ah)

#include <stdio.h>
#include "sys/compower.h"
#include "contiki.h"
#include <math.h>
#include <solarTrace/all-in-one.c>
#include <node-id.h>

#define DEBUG 0
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

PROCESS(battery_process, "Periodic Battery Update");
unsigned timer_couter=0, solar_array_couter=0;
unsigned btt_interval_sec, btt_interval_min;
unsigned day_profile=0; // Change it if you want to start with a different day
short new_day=1;
double total_consumption=0, periodic_consumption=0;



struct battery{
  // Maximum amount of charge in battery (microAh) - static
   double qmax;
  // Available charge at beginning of time step (microAh)
   long double q1_0;
  // Bound charge at beginning of time step (microAh)
   long double q2_0;
  // Battery rate constant (rate at which chemically bound charge becomes available)
   double k;
  // Battery capacity ratio (fraction of total charge that is available) 
   double c;
  // Charge (-) or discharge (+) current of battery (A)
   double i;
  // Length of time step (micro hours)
   double dt;
  // parameter of the Kinetic model that is the result of exp(-k * dt)
   double r;
}; struct battery batt; // Parameters of the Battery 

struct stats{
unsigned long cpu;
unsigned long lpm;
unsigned long transmit;
unsigned long listen;
unsigned long idle_transmit;
unsigned long idle_listen;
}; struct stats stats_com;

struct energy_states{
	double active; //8Mhz
	double low_power;
	double tx;
	double rx;
}; 
struct energy_states i_energyStt = {0.001200*1000000, 0.000090*1000000, 0.0336*1000000, 0.0185*1000000};// Current values for Wismote (microA)



void battery_start(clock_time_t period, unsigned seconds, double fixed_perc_energy, unsigned variation) {
	double random_variable;
	
	random_variable = fixed_perc_energy + ((rand() % variation)*0.01);
	
	
	printf("rand() is %u and Random variable is %u \n",(unsigned) rand()%variation, (unsigned) (random_variable*100));
	
	batt.qmax = 1*1000000;
	batt.q1_0 = 1*1000000 * random_variable;
	batt.q2_0 = batt.qmax * (1 - random_variable) ; // q1 and q2 should start balanced
	batt.k = 0.1;
	batt.c = batt.q1_0/batt.qmax;
	batt.i = 0; 
	batt.dt = 0;
	batt.r = 0;
	
	printf("Battery: qmax is %lu        q1 is %lu        q2 is %lu \n",(long unsigned) batt.qmax, (long unsigned) batt.q1_0, (long unsigned) batt.q2_0);
	
  process_start(&battery_process, (void *)&period);
  btt_interval_sec=seconds;//gives it in seconds
  btt_interval_min=seconds/60;
}



void update_time_stats(){
  static unsigned long last_cpu, last_lpm, last_transmit, last_listen;
  static unsigned long last_idle_transmit, last_idle_listen;
  unsigned long cpu, lpm, transmit, listen;
  unsigned long all_cpu, all_lpm, all_transmit, all_listen;
  unsigned long idle_transmit, idle_listen;
  unsigned long all_idle_transmit, all_idle_listen;
  unsigned long time, all_time, radio, all_radio;

  energest_flush();	

// Values since the begining
  all_cpu = energest_type_time(ENERGEST_TYPE_CPU);
  all_lpm = energest_type_time(ENERGEST_TYPE_LPM);
  all_transmit = energest_type_time(ENERGEST_TYPE_TRANSMIT);
  all_listen = energest_type_time(ENERGEST_TYPE_LISTEN);
  all_idle_transmit = compower_idle_activity.transmit;
  all_idle_listen = compower_idle_activity.listen;

// Values in the interval
  cpu = all_cpu - last_cpu;
  lpm = all_lpm - last_lpm;
  transmit = all_transmit - last_transmit;
  listen = all_listen - last_listen;
  idle_transmit = compower_idle_activity.transmit - last_idle_transmit;
  idle_listen = compower_idle_activity.listen - last_idle_listen;
  
// Update last variables
  last_cpu = energest_type_time(ENERGEST_TYPE_CPU);
  last_lpm = energest_type_time(ENERGEST_TYPE_LPM);
  last_transmit = energest_type_time(ENERGEST_TYPE_TRANSMIT);
  last_listen = energest_type_time(ENERGEST_TYPE_LISTEN);
  last_idle_listen = compower_idle_activity.listen;
  last_idle_transmit = compower_idle_activity.transmit;

// Update the struct
  stats_com.cpu=cpu;
  stats_com.lpm=lpm;
  stats_com.transmit=transmit;
  stats_com.listen=listen;
  stats_com.idle_transmit=idle_transmit;
  stats_com.idle_listen=idle_listen;

// General values
  radio = transmit + listen;
  time = cpu + lpm;
  all_time = all_cpu + all_lpm;
	
}


void kinetic_model(){
double q0, q1, q2, id_max, ic_max;
	q0 = batt.q1_0 + batt.q2_0;
    
    // Calculate maximum discharge and charging currents
    // id : current discharging
    // ic : current charging
    id_max = (batt.k * batt.q1_0 * batt.r + q0 * batt.k * batt.c * (1 - batt.r)) / (1 - batt.r + batt.c * (batt.k * batt.dt - 1 + batt.r));
    ic_max = (-batt.k * batt.c * batt.qmax + batt.k * batt.q1_0 * batt.r + q0 * batt.k * batt.c * (1 - batt.r)) / (1 - batt.r + batt.c * (batt.k * batt.dt - 1 + batt.r));
    
    // Check if battery current is within maximum bounds
    // If not, set battery current to limits
    //if (batt.i > id_max) batt.i = id_max;
    //if (batt.i < ic_max) batt.i = ic_max;
    
    // Update the available and bound charge
    q1 =  batt.q1_0 * batt.r + ((q0 * batt.k * batt.c - batt.i) * (1 - batt.r) - batt.i * batt.c * (batt.k * batt.dt - 1 + batt.r)) / batt.k ;
    q2 =  batt.q2_0 * batt.r + q0 * (1 - batt.c) * (1 - batt.r) - batt.i * (1 - batt.c) * (batt.k * batt.dt - 1 + batt.r) / batt.k ;
    
    batt.q1_0 = q1;
    batt.q2_0 = q2;

	}


void update_battery(){
  double convert_tick2sec=0.000030517578125;//A single time tick
  double convert_sec2hour=0.0002777777777778;// 1/3600
  unsigned long convert_tick2nano=30517;// a nanoseconds for a single time tick
  double voltage=0.2;//it is 1/5v
  double efficiency_painel=0.2;
  double convert2micro=1000000;
  double painel_area=210;//in cm2. 
  double current_draw[5];// in micro A
  unsigned long time_each_stt[5];
  double solar_charging;
  int stt_couter, num_states=5;
  long unsigned sim_time_minutes;
  unsigned start_sunlight=8, end_sunlight=start_sunlight+13;// in hours, considering 24h --- Trace is conf to run 13, which means between 8h to 21h
  unsigned duration_solartrace_value=1;// number of timer_counter's units which the solar value longs 

  
//-------- selects another day profile in the begining of a new day -----------------
  if(new_day==1){
  //day_profile=(unsigned)(random_rand()*100000L)*0.0001;//random_rand() returns 0 - 65375. So, this line makes day_profile a number between 0-6
  if(day_profile<6)
  day_profile++;
  else day_profile=0;
  printf("Battery: Day is %u \n",day_profile);
  new_day=0;
  }

//-------- Controls the time -----------------
  timer_couter=timer_couter+btt_interval_min;
  sim_time_minutes=timer_couter; // assuming the battery interval is 60 seconds - 1 minute
  printf("Battery: Timer couter is %u and Time is %u \n",timer_couter, (unsigned) sim_time_minutes);
  
//-------- Computes the current charging -----------------
  if((sim_time_minutes>=start_sunlight*60) && (sim_time_minutes<=end_sunlight*60)){// it is in the sun light time
	solar_charging=(unsigned long)(solar_array[day_profile][solar_array_couter]* efficiency_painel* voltage* painel_area);//read the array with solar current values (in micro Watt)
	printf("Battery: 1 Solar charging is %lu \n",(unsigned long)(solar_charging));
	solar_charging=-1*solar_charging;//the current should be negative in case of charging. Kinetic model requires it.
	if(sim_time_minutes % duration_solartrace_value == 0) solar_array_couter++; // For this solar trace, it is 5 min
    }
  else{ // there is no sun light, so it does not compute solar charging
	solar_charging=0;
	num_states=4;
	}
  if(solar_array_couter>=size_of_solar_array)solar_array_couter=0;//it should be an error
  if(sim_time_minutes>=(24*60)){// end of a day
	timer_couter=0;
	new_day=1;
	solar_array_couter=0;
	printf("Battery: End of day \n");
	}// reset for a new day

//-------- Set the current spend/collect in every state -----------------
  current_draw[0] = i_energyStt.active;
  current_draw[1] = i_energyStt.low_power;
  current_draw[2] = i_energyStt.tx;
  current_draw[3] = i_energyStt.rx;
  current_draw[4] = solar_charging;//solar charging current. It should be negative
  
//-------- Set the time spent in every state -----------------
  update_time_stats();
  time_each_stt[0] = stats_com.cpu;
  time_each_stt[1] = stats_com.lpm;
  time_each_stt[2] = stats_com.transmit + stats_com.idle_transmit;
  time_each_stt[3] = stats_com.listen + stats_com.idle_listen;
  time_each_stt[4] = stats_com.cpu + stats_com.lpm; // solar charging time (in ticks) 
  
//-------- call the Kinetic model -----------------
  for(stt_couter=0;stt_couter<num_states;stt_couter++){
	// --------------------------------------- Variable Battery parameters ------------------------------
	// Define the charging or discharging current (microAh)
	batt.i=current_draw[stt_couter];
	
	if(stt_couter<4){ // this is because harvesting does not count for energy consumption
	total_consumption=total_consumption + (current_draw[stt_couter]*time_each_stt[stt_couter]*convert_tick2sec*convert_sec2hour);
	periodic_consumption = (current_draw[stt_couter]*time_each_stt[stt_couter]*convert_tick2sec*convert_sec2hour);
	}
	// Define the time step (hour)
	batt.dt=time_each_stt[stt_couter]*convert_tick2sec*convert_sec2hour; // convert from seconds to hours
	
  
	// Computing r
	batt.r=expf(-batt.k * batt.dt);
  
	// Call kinetic model
	kinetic_model();
 
	// prints
	printf("Battery: Available charge at next time interval is %lu (microAh) \n",(unsigned long) (batt.q1_0));
	PRINTF("Battery: Bound charge at next time interval is %lu (microAh) \n",(unsigned long) (batt.q2_0));
	//PRINTF("Battery: Time of CPU in state %d in nanoseconds is %lu \n",j,(unsigned long)time_each_stt[j]*convert_tick2nano);
	PRINTF("Battery: Number of CPU ticks in state %d is %lu \n",stt_couter,time_each_stt[stt_couter]);
  }
  
    // Print
	printf("Log: Residual (microAh); %lu; \n",(unsigned long) (batt.q1_0));
	//PRINTF("Battery: Bound charge at next time interval is %lu (microAh) \n",(unsigned long) (batt.q2_0));
	printf("Log: Energy Total consumption (microA); %lu;  \n",(unsigned long) (total_consumption));
	printf("Log: Energy Periodic consumption (microA); %d;  \n",(int) (periodic_consumption));
	printf("Energy: CPU ticks %lu, LPM %lu, Tx %lu, Rx %lu \n", stats_com.cpu, stats_com.lpm, stats_com.transmit, stats_com.listen);

  
}

long double get_battery_charge(){
    return batt.q1_0;	
}

double get_max_charge(){
    return batt.qmax;	
}


PROCESS_THREAD(battery_process, ev, data)
{
  static struct etimer periodic;
  clock_time_t *period;
  PROCESS_BEGIN();

  period = data;

  if(period == NULL) {
    PROCESS_EXIT();
  }
  etimer_set(&periodic, *period);

  while(1) {
    PROCESS_WAIT_UNTIL(etimer_expired(&periodic));
    etimer_reset(&periodic);
    update_battery();
  }

  PROCESS_END();
}
