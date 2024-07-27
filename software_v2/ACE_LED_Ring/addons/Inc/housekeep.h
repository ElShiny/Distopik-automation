/*
 * housekeep.h
 *
 *  Created on: Jul 16, 2024
 *      Author: Matej
 */

#ifndef ADDONS_INC_HOUSEKEEP_H_
#define ADDONS_INC_HOUSEKEEP_H_

#include "stdint.h"


void housekeeping_tasks_callback(void);
void start_housekeeping(void);
void stop_housekeeping(void);

uint8_t check_for_settings_changes(void);
void reset_settings_changed(void);

#endif /* ADDONS_INC_HOUSEKEEP_H_ */
