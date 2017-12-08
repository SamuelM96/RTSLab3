/*
 * genfsk_defs.h
 *
 *  Created on: 24 Nov 2017
 *      Author: Samuel Mott
 */

#ifndef GENFSK_DEFS_H_
#define GENFSK_DEFS_H_

#define gRadioOpcode1 (0xAA)
#define gRadioOpcode2 (0xBB)
//#define SERVER
//#define NODEID 122

//#define RX
#define TX

#if defined(TX) && defined(RX)
#error "Cannot have device both transmit and receive"
#endif

#if !defined(TX) && !defined(RX)
#error "Must define device to either transmit or receive"
#endif

#endif /* GENFSK_DEFS_H_ */
