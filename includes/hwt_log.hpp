#pragma once

#include <stdio.h>

FILE *log ()
{
    static FILE *log_file = fopen ( "logs/log.html", "w" );

    return log_file;
}