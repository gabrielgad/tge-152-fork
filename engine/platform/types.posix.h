//-----------------------------------------------------------------------------
// Torque Game Engine 
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _TYPESPOSIX_H_
#define _TYPESPOSIX_H_

#include <stddef.h>

#define FN_CDECL     ///< Calling convention

// size_t is needed to overload new
// On 64-bit systems, size_t is 64-bit, so dsize_t must match
typedef size_t dsize_t;


/** Platform dependent file date-time structure.  The defination of this structure
  * will likely be different for each OS platform.
  */
typedef S32 FileTime;


#ifndef NULL
#  define NULL (0)
#endif


#endif //_TYPESPOSIX_H_
