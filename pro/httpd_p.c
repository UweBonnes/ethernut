
#include "httpd_p.h"
#include <sys/heap.h>

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


/*!
* \brief Release request info structure.
* \internal
* \param req Pointer to the info structure. If NULL, nothing
*            is released.
*/
void DestroyRequestInfo(REQUEST * req)
{
	if (req) {
		if (req->req_url)
			NutHeapFree(req->req_url);
		if (req->req_query)
			NutHeapFree(req->req_query);
		if (req->req_type)
			NutHeapFree(req->req_type);
		if (req->req_cookie)
			NutHeapFree(req->req_cookie);
		if (req->req_auth)
			NutHeapFree(req->req_auth);
		if (req->req_agent)
			NutHeapFree(req->req_agent);
		if (req->req_qptrs)
			NutHeapFree(req->req_qptrs);
		if (req->req_referer)
			NutHeapFree(req->req_referer);
		if (req->req_host)
			NutHeapFree(req->req_host);
		NutHeapFree(req);
	}
}
