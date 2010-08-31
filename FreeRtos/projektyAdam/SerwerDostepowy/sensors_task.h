/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher
 * Copyright: GPL V2
 * See http://www.gnu.org/licenses/gpl.html
 *
 * Ethernet remote device and sensor
 * UDP and HTTP interface
        url looks like this http://baseurl/password/command
        or http://baseurl/password/
 *
 * Chip type           : Atmega88 or Atmega168 or Atmega328 with ENC28J60
 * Note: there is a version number in the text. Search for tuxgraphics
 *********************************************/
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/pgmspace.h>

#include "main.h"

#ifndef SENSORS_TASK_H
#define SENSORS_TASK_H

void sensorsTask(void *pvParameters);

#endif
