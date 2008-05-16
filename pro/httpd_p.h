#ifndef _HTTPD_P_H_
#define _HTTPD_P_H_

/*
*  W A R N I N G
*  -------------
*
* This file is not part of the Ethernut API.  It exists purely as an
* implementation detail.  This header file may change from version to
* version without notice, or even be removed.
*
* We mean it.
*/


#include <sys/types.h>
#include <pro/httpd.h>

void DestroyRequestInfo(REQUEST * req);

#endif
