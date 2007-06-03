/*!
 * Copyright (C) 2001-2005 by egnite Software GmbH. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 */

/*!
 * $Log$
 * Revision 1.1  2007/06/03 08:48:17  haraldkipp
 * New application sample demonstrates calendar functions.
 *
 */

#include <dev/board.h>
#include <dev/debug.h>

#include <sys/version.h>
#include <sys/timer.h>

#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <time.h>

/*!
 * \example caltime/caltime.c
 *
 * Demonstrates Nut/OS date and time functions, which had been contributed
 * by Oliver Schulz.
 */
static char *version = "1.0";

/* Used for ASCII Art Animation. */
static char *rotor = "|/-\\";

static char *weekday_name[7] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

/*
 * Print content of tm structure.
 */
static void PrintDateTime(CONST struct _tm *stm)
{
    printf("%s, %04d/%02d/%02d, %02d:%02d:%02d%s"
        , weekday_name[stm->tm_wday]
        , stm->tm_year + 1900, stm->tm_mon + 1, stm->tm_mday
        , stm->tm_hour, stm->tm_min, stm->tm_sec
        , stm->tm_isdst ? " DST" : ""
        );
}

/*
 * Query calendar date from user.
 *
 * Returns 0 on success, -1 otherwise.
 */
static int EnterDate(struct _tm *stm)
{
    int year;
    int mon;
    int mday;
    int n;

    printf("Enter date, use format YYYY/MM/DD (%04d/%02d/%02d): ", 
        stm->tm_year + 1900, stm->tm_mon + 1, stm->tm_mday);
    n = scanf("%d/%d/%d", &year, &mon, &mday);
    if (n >= 1) {
        printf("%04d", year);
        if (year < 1970 || year > 2038) {
            printf("\nBad year: %d is not within range 1970..2038\n", year);
            return -1;
        }
        stm->tm_year = year - 1900;
    }
    if (n >= 2) {
        printf("/%02d", mon);
        if (mon < 1 || mon > 31) {
            printf("\nBad month: %d is not within range 1..12\n", mon);
            return -1;
        }
        stm->tm_mon = mon - 1;
    }
    if (n >= 3) {
        printf("/%02d", mday);
        if (mday < 1 || mday > 31) {
            printf("\nBad day: %d is not within range 1..31\n", mday);
            return -1;
        }
        stm->tm_mday = mday;
    }
    putchar('\n');

    return 0;
}

/*
 * Query time of day from user.
 *
 * Returns 0 on success, -1 otherwise.
 */
static int EnterTime(struct _tm *stm)
{
    int hour;
    int minute;
    int second;
    int n;

    printf("Enter time, use 24h format HH:MM:SS (%02d:%02d:%02d): ", 
        stm->tm_hour, stm->tm_min, stm->tm_sec);
    n = scanf("%d:%d:%d", &hour, &minute, &second);
    if (n >= 1) {
        printf("%02d", hour);
        if (hour < 0 || hour > 23) {
            printf("\nBad hour: %d is not within range 0..23\n", hour);
            return -1;
        }
        stm->tm_hour = hour;
    }
    if (n >= 2) {
        printf(":%02d", minute);
        if (minute < 0 || minute > 59) {
            printf("\nBad minute: %d is not within range 0..59\n", minute);
            return -1;
        }
        stm->tm_min = minute;
    }
    if (n >= 3) {
        printf(":%02d", second);
        if (second < 0 || second > 59) {
            printf("\nBad second: %d is not within range 0..59\n", second);
            return -1;
        }
        stm->tm_sec = second;
    }
    putchar('\n');

    return 0;
}

/*
 * Query time difference from user, specified in hours and minutes.
 *
 * 'init' specifies the default number of seconds.
 *
 * Returns the number of seconds.
 */
static long EnterTimeDiff(long init)
{
    long hours;
    long minutes;
    long seconds;
    int n;

    seconds = init;
    minutes = seconds / 60L;
    hours = minutes / 60L;
    minutes = abs(minutes % 60L);
    printf("Enter time difference in format HH:MM (%+03ld:%02ld): ", hours, minutes);
    n = scanf("%ld:%ld", &hours, &minutes);
    if (n >= 1) {
        printf("%+03ld", hours);
        if (hours < -12 || hours > 12) {
            printf("\nBad hours: %ld is not within range -12..+12\n", hours);
            return init;
        }
    }
    if (n >= 2) {
        printf("%02ld", minutes);
        if (minutes < 0 || minutes > 59) {
            printf("\nBad minutes: %ld is not within range 0..59\n", minutes);
            return init;
        }
    }
    putchar('\n');

    return (hours * 60L + minutes) * 60L;
}

/*
 * Display the elapsed seconds of the epoch.
 *
 * The value is constantly updated until the user presses a key.
 */
static void DisplaySeconds(void)
{
    int i = 0;

    while (!kbhit()) {
        printf(" [%c] Seconds since epoch: %ld\r", rotor[++i & 3], (long)time(NULL));
        NutSleep(200);
    }
    putchar('\n');
}

/*
 * Display the coordinated universal time.
 *
 * The value is constantly updated until the user presses a key.
 */
static void DisplayZuluTime(void)
{
    time_t secs;
    struct _tm *gmt;
    int i = 0;

    while (!kbhit()) {
        secs = time(NULL);
        gmt = gmtime(&secs);
        printf(" [%c] Universal time: ", rotor[++i & 3]);
        PrintDateTime(gmt);
        printf("    \r");
        NutSleep(500);
    }
    putchar('\n');
}

/*
 * Display the local time.
 *
 * The value is constantly updated until the user presses a key.
 */
static void DisplayLocalTime(void)
{
    time_t tt;
    struct _tm *ltm;
    int i = 0;

    while (!kbhit()) {
        /* Get local time and print it. */
        tt = time(NULL);
        ltm = localtime(&tt);
        printf(" [%c] Local time: ", rotor[++i & 3]);
        PrintDateTime(ltm);

        /* Calculate the offset from UTC in minutes. */
        tt = _timezone;
        if (ltm->tm_isdst > 0) {
            tt += _dstbias;
        }
        tt /= -60L;

        /* Print UTC offset in format HH:MM. */
        printf(" UTC%+03ld:%02ld   \r", tt / 60L, abs(tt) % 60L);
        NutSleep(200);
    }
    putchar('\n');
}

/*
 * Display the week day of a queried calendar date.
 *
 * mktime() updates the structure members tm_yday and tm_wday.
 * This can be used to determine the week day name of any given
 * date.
 */
static void CalcWeekDay(void)
{
    struct _tm date;
    time_t secs;
    
    /* Use current local time as default. */
    time(&secs);
    memcpy(&date, localtime(&secs), sizeof(date));
    /* Query date and print week day name if we got a valid entry. */
    if (EnterDate(&date) == 0) {
        mktime(&date);
        puts(weekday_name[date.tm_wday]);
    }
}

/*
 * Query user for a new time zone offset.
 */
static void SetTimeZone(void)
{
    /* Nut/OS uses a global variable to store the current TZ offset. */
    _timezone = EnterTimeDiff(_timezone);
}

/*
 * Query user for a new system time.
 */
static void SetLocalTime(void)
{
    struct _tm ltm;
    time_t now;

    /* Use current local time as default. */
    time(&now);
    memcpy(&ltm, localtime(&now), sizeof(ltm));

    /* Query date and time. */
    if (EnterDate(&ltm) == 0 && EnterTime(&ltm) == 0) {
        /* Let mktime determine whether DST is in effect. */
        ltm.tm_isdst = -1;
        /* mktime expects local time and returns seconds since the epoch. */
        now = mktime(&ltm);
        /* stime expects seconds since the epoch. */
        stime(&now);
    }
}

/*
 * Application entry.
 */
int main(void)
{
    u_long baud = 115200;
    int cmd;

    /* Use UART device for stdin and stdout. */
    NutRegisterDevice(&DEV_UART, 0, 0);
    freopen(DEV_UART_NAME, "w", stdout);
    freopen(DEV_UART_NAME, "r", stdin);    
    _ioctl(_fileno(stdout), UART_SETSPEED, &baud);
    printf("\n\nCalendar Time %s running on Nut/OS %s\n", version, NutVersionString());

#ifdef RTC_CHIP
    /* Register and query hardware RTC, if available. */
    printf("Registering RTC hardware...");
    if (NutRegisterRtc(&RTC_CHIP)) {
        puts("failed");
    } else {
        u_long rtc_stat;

        NutRtcGetStatus(&rtc_stat);
        if (rtc_stat & RTC_STATUS_PF) {
            puts("power failure");
        }
        else {
            puts("OK");
        }
    }
#endif

    for (;;) {
        /* Print command menu. */
        puts("\n  0 - Display seconds counter");
        puts("  1 - Display universal time");
        puts("  2 - Display local time");
        puts("  3 - Calculate weekday");
        puts("  S - Set local time");
        puts("  Y - Toggle DST calculation");
        puts("  Z - Set timezone");

        printf("What is thy bidding, my master? ");

        /* Flush input buffer. */
        while (kbhit()) {
            cmd = getchar();
        }

        /* Get the next command. */
        cmd = getchar();
        putchar(cmd);
        putchar('\n');

        /* Process the command. */
        switch(cmd) {
        case '0':
            DisplaySeconds();
            break;
        case '1':
            DisplayZuluTime();
            break;
        case '2':
            DisplayLocalTime();
            break;
        case '3':
            CalcWeekDay();
            break;
        case 'S':
        case 's':
            SetLocalTime();
            break;
        case 'Y':
        case 'y':
            /* Nut/OS uses a global variable to enable/disable DST. 
               Toggle the current status and display the result. */
            _daylight = _daylight == 0;
            printf("DST calculation %sabled\n", _daylight ? "en" : "dis");
            break;
        case 'Z':
        case 'z':
            SetTimeZone();
            break;
        }
    }
    return 0;
}

