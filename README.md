# GMT Timezone plugin

![](https://img.shields.io/badge/version-0.2.3-green) ![](https://img.shields.io/badge/project-inactive-red) 

This is timezone plugin. You can set any timezone for your SA-MP server. And some useful function.

#### Important
*This plugin was originally created by [Fro1sha](https://forum.sa-mp.com/member.php?u=72857 "Fro1sha"). I added the plugin to GitHub as the SA-MP forum is no longer available forever.*

------------

### Natives
```pawn
native gm_SetTimeZone(hour = 0, minute = 0);
native gm_gettime(&hour = 0, &minute = 0, &second = 0);
native gm_getdate(&year=0, &month = 0, &day = 0, &wday = 0);

native gm_strftime(const string[], dest[], size = sizeof(dest));
native gm_timestamp();
native gm_mktime(hour, minute, second, year, month, day);
native gm_timestamp_to_date(timestamp, &hour = 0, &minute = 0, &second = 0, &year = 0, &month = 0, &day = 0);

native gm_GetMonthDay(month, year = 1990);
native gm_ConvertSeconds(time, &hour = 0, &minute = 0, &second = 0, &year = 0, &month = 0, &day = 0);
```

### How to use
```pawn
// Auto replacement default pawn time function
#define AUTO_SETUP
#include <gmtime>
```

### Small example
```pawn
#define AUTO_SETUP
#include <gmtime>
 
main
{
    // Set timezone to +0
    SetTimeZone( 0 );
    
    new
        hour, minute, second,
        year, month, day, 
        wday // week day 0-6
    ;
    
    printf( "Cur TimeStamp(gettime) %d", gettime( hour, minute, second ) );
    printf( "Hour: %d Minute: %d Seconds: %d", hour, minute, second );
    
    printf( "Day(getdate) %d", getdate( year, month, day, wday ) );
    printf( "Year: %d Month: %d Day: %d WeekDay: %d", year, month, day, wday );
    
    new
        buffer[ 80 ]
    ;
    
    // More details:
    // http://www.cplusplus.com/reference/clibrary/ctime/strftime/
    strftime( "Now it's %I:%M%p.", buffer ); 
    print( buffer );
    
    // TimeStamp time
    printf( "Cur TimeStamp(timestamp) %d", timestamp(  ) );
    
    // Converting date and time in TimeStamp
    printf( "mktime: %d", mktime( hour, minute, second, year, month, day ) );
    
    // Converting TimeStamp in date and time
    timestamp_to_date( timestamp(  ), hour, minute, second, year, month, day );
    print( "timestamp_to_date:" );
    printf( "Hour: %d Minute: %d Seconds: %d", hour, minute, second );
    printf( "Year: %d Month: %d Day: %d", year, month, day );
    
    // Return the number of days in that month
    printf( "Month(%d) Days: %d", month, GetMonthDay( month, year ) );
    
    // Converting seconds in date and time
    ConvertSeconds( 65486, hour, minute, second, year, month, day );
    print( "ConvertSeconds:" );
    printf( "Hour: %d Minute: %d Seconds: %d", hour, minute, second );
    printf( "Year: %d Month: %d Day: %d", year, month, day );
}
```