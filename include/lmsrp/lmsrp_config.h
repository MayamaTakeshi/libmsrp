/*
 * lmsrp_config.h
 *
 *  Created on: Oct 13, 2021
 *      Author: amneiht
 */

#ifndef LMSRP_LMSRP_CONFIG_H_
#define LMSRP_LMSRP_CONFIG_H_
#include <string.h>
/**
 * suport for minimum requit . the library only need pjlib.a library
 */
#define LMSRP_ONLY_PJLIB 0
#define LMSRP_LOW_MEMORY 1

# ifndef LMSRP_DEBUG
#	define  LMSRP_DEBUG 0
# endif
#define __LNAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif /* LMSRP_LMSRP_CONFIG_H_ */
