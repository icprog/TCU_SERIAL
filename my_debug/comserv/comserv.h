#ifndef		TDM_SI_INCLUDED_COMSERV_H
#define		TDM_SI_INCLUDED_COMSERV_H


//#ifdef __cplusplus
//extern "C" {
//#endif

#define TDM_COMSERVAPI extern


#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include "tdmalloc.h"
#include	"tdarray.h"
#include	"tdstr.h"
#include	"tdstring.h"

#define		TDM_BETWEEN(a,b1,b2)		((a >= TDM_MINVAL(b1,b2)) && (a <= TDM_MAXVAL(b1,b2)))

#endif		/*TDM_SI_INCLUDED_COMSERV_H*/

//#ifdef __cplusplus
//}
//#endif
