/*
 * housekeep.h
 *
 *  Created on: Jul 16, 2024
 *      Author: Matej
 */

#ifndef ADDONS_INC_HOUSEKEEP_H_
#define ADDONS_INC_HOUSEKEEP_H_

#include "stdint.h"

uint8_t check_for_settings_changes();
void housekeeping_tasks(void);
void start_housekeeping(void);

#endif /* ADDONS_INC_HOUSEKEEP_H_ */
