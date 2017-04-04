/*
 * Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *      Erbium (Er) REST Engine example.
 * \author
 *      Matthias Kovatsch <kovatsch@inf.ethz.ch>
 * 
 * ARIKER AT DEI.UC.PT
 */
 
 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sys/etimer.h"
#include "contiki.h"
#include "contiki-net.h"
#include "rest-engine.h"
#include "er-coap-observe.h"
#include "apps/powertrace/powertrace.h"
#include "sys/battery_kinetic.h" //riker
#include <node-id.h>

#if PLATFORM_HAS_BUTTON
#include "dev/button-sensor.h"
#endif

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINT6ADDR(addr) PRINTF("[%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x]", ((uint8_t *)addr)[0], ((uint8_t *)addr)[1], ((uint8_t *)addr)[2], ((uint8_t *)addr)[3], ((uint8_t *)addr)[4], ((uint8_t *)addr)[5], ((uint8_t *)addr)[6], ((uint8_t *)addr)[7], ((uint8_t *)addr)[8], ((uint8_t *)addr)[9], ((uint8_t *)addr)[10], ((uint8_t *)addr)[11], ((uint8_t *)addr)[12], ((uint8_t *)addr)[13], ((uint8_t *)addr)[14], ((uint8_t *)addr)[15])
#define PRINTLLADDR(lladdr) PRINTF("[%02x:%02x:%02x:%02x:%02x:%02x]", (lladdr)->addr[0], (lladdr)->addr[1], (lladdr)->addr[2], (lladdr)->addr[3], (lladdr)->addr[4], (lladdr)->addr[5])
#else
#define PRINTF(...)
#define PRINT6ADDR(addr)
#define PRINTLLADDR(addr)
#endif

extern resource_t res_DataAggregation, res_event,   res_separate;


PROCESS(er_example_server, "CoAP Server with Data-Aggregation");
AUTOSTART_PROCESSES(&er_example_server);

PROCESS_THREAD(er_example_server, ev, data)
{
	
	// Battery Settings
	unsigned seconds=60*5;// warning: if this variable is changed, then the kinect variable the count the minutes should be changed
	static struct etimer add_obs_timer;
	double fixed_perc_energy = 0.2;// 0 - 1
	unsigned variation = 2;//0 - 99


	PROCESS_BEGIN();
	PRINTF("Ticks per second: %u\n", RTIMER_SECOND);
	//powertrace_start(CLOCK_SECOND * 60);

	// Start battery
	battery_start(CLOCK_SECOND * seconds, seconds, fixed_perc_energy, variation);
	printf("Value of battery interval %u \n", CLOCK_SECOND * 60 * 5);
	
	
	PROCESS_PAUSE();

	PRINTF("CoAP Server with Data-Aggregation\n");


	/* Initialize the REST engine. */
	rest_init_engine();

	rest_activate_resource(&res_DataAggregation, "Data-Aggregation");


	// RIKER code: 
	etimer_set(&add_obs_timer, CLOCK_SECOND*50); // set timer to add the observers. It is not the interval to send messages
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&add_obs_timer));
	/* chamada para add observer */
	static uint16_t addr_test[8];
	//uint16_t port_test=60052;
	uip_ipaddr_t dest_addr;

	addr_test[0] = 0xaaaa;
	addr_test[4] = 0x200;
	addr_test[5] = 0x7401;
	addr_test[6] = 0x0000;
	addr_test[7] = 0x0001;

	uip_ip6addr(&dest_addr, addr_test[0], addr_test[1], addr_test[2],
	addr_test[3], addr_test[4],addr_test[5],addr_test[6],addr_test[7]);

	//printf("Calling coap_add_observer \n");
	coap_add_observer(
	&dest_addr, 
	0,
	0, 
	0,
	"Data-Aggregation");



	while(1) {
		PROCESS_WAIT_EVENT();
		#if PLATFORM_HAS_BUTTON
		if(ev == sensors_event && data == &button_sensor) {
			PRINTF("*******BUTTON*******\n");

			/* Call the event_handler for this application-specific event. */
			res_event.trigger();

			/* Also call the separate response example handler. */
			res_separate.resume();
		}
		#endif /* PLATFORM_HAS_BUTTON */
	}                             /* while (1) */

	PROCESS_END();
}
