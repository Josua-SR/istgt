/*
 * Copyright © 2017-2019 The OpenEBS Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at

 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef	REPLICATION_LOG_H
#define	REPLICATION_LOG_H

#include <sys/time.h>
#include <stdio.h>
#include <time.h>

enum replication_log_level {
	LOG_LEVEL_ERR,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_MAX,
};

int replication_log_level;

#define	repl_log(lvl, fmt, ...)						\
do {									\
	struct timeval _tv;						\
	struct tm *_tinfo;						\
	unsigned int _ms;						\
	char _l[512];							\
	int _off = 0;							\
									\
	if (replication_log_level < lvl)				\
		break;							\
									\
	/* Create timestamp prefix */					\
	gettimeofday(&_tv, NULL);					\
	_tinfo = localtime(&_tv.tv_sec);				\
	_ms = _tv.tv_usec / 1000;					\
	strftime(_l, sizeof (_l), "%Y-%m-%d/%H:%M:%S.", _tinfo);	\
	_off += 20;							\
	snprintf(_l+ _off, sizeof (_l) - _off, "%03u ", _ms);		\
	_off += 4;							\
									\
	snprintf(_l + _off, sizeof (_l) - _off, fmt, ##__VA_ARGS__);	\
	fprintf(stderr, "%s\n", _l);					\
} while (0);

#define	REPLICA_LOG(fmt, ...)						\
{									\
	repl_log(LOG_LEVEL_INFO, "%-18.18s:%4d: %-20.20s: " fmt,	\
		    __func__, __LINE__, tinfo, ##__VA_ARGS__);		\
}

#define	REPLICA_NOTICELOG(fmt, ...)					\
{									\
	repl_log(LOG_LEVEL_INFO, "%-18.18s:%4d: %-20.20s: " fmt,	\
		    __func__, __LINE__, tinfo, ##__VA_ARGS__);		\
}

#define	REPLICA_ERRLOG(fmt, ...)					\
{									\
	repl_log(LOG_LEVEL_ERR, "%-18.18s:%4d: %-20.20s: " fmt,		\
		    __func__, __LINE__, tinfo, ##__VA_ARGS__);		\
}
#define	REPLICA_WARNLOG(fmt, ...)					\
{									\
	repl_log(LOG_LEVEL_ERR, "%-18.18s:%4d: %-20.20s: " fmt,		\
		    __func__, __LINE__, tinfo, ##__VA_ARGS__);		\
}

#ifdef	DEBUG
#define	REPLICA_DEBUGLOG(fmt, ...)					\
{									\
	repl_log(LOG_LEVEL_DEBUG, "%-18.18s:%4d: %-20.20s: " fmt,	\
		    __func__, __LINE__, tinfo, ##__VA_ARGS__);		\
}
#else
#define	REPLICA_DEBUGLOG(fmt, ...)
#endif

#endif /* REPLICATION_LOG_H */
