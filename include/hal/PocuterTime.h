#ifndef POCUTER_DISABLE_TIME
#ifndef POCUTERTIME_H
#define POCUTERTIME_H
#include <time.h>
#define TIME_ZONES_COUNT 461
/**
* @brief access the Pocuter real time clock
*  
*/
class PocuterTime {
    public:
        enum TIMEERROR {
           TIMEERROR_OK = 0,
           TIMEERROR_FAILED = 1,
          
           
           TIMEERROR_UNKNOWN
           
          
       };
       struct pocuterTimezone {
           const char* name;
           const char* timezoneString;
       };
#define TIME_ZONES_COUNT 461
       const pocuterTimezone c_tz[TIME_ZONES_COUNT] = {
            { .name = "Africa/Abidjan", .timezoneString = "GMT0" },
            { .name = "Africa/Accra", .timezoneString = "GMT0" },
            { .name = "Africa/Addis_Ababa", .timezoneString = "EAT-3" },
            { .name = "Africa/Algiers", .timezoneString = "CET-1" },
            { .name = "Africa/Asmara", .timezoneString = "EAT-3" },
            { .name = "Africa/Bamako", .timezoneString = "GMT0" },
            { .name = "Africa/Bangui", .timezoneString = "WAT-1" },
            { .name = "Africa/Banjul", .timezoneString = "GMT0" },
            { .name = "Africa/Bissau", .timezoneString = "GMT0" },
            { .name = "Africa/Blantyre", .timezoneString = "CAT-2" },
            { .name = "Africa/Brazzaville", .timezoneString = "WAT-1" },
            { .name = "Africa/Bujumbura", .timezoneString = "CAT-2" },
            { .name = "Africa/Cairo", .timezoneString = "EET-2" },
            { .name = "Africa/Casablanca", .timezoneString = "<+01>-1" },
            { .name = "Africa/Ceuta", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Africa/Conakry", .timezoneString = "GMT0" },
            { .name = "Africa/Dakar", .timezoneString = "GMT0" },
            { .name = "Africa/Dar_es_Salaam", .timezoneString = "EAT-3" },
            { .name = "Africa/Djibouti", .timezoneString = "EAT-3" },
            { .name = "Africa/Douala", .timezoneString = "WAT-1" },
            { .name = "Africa/El_Aaiun", .timezoneString = "<+01>-1" },
            { .name = "Africa/Freetown", .timezoneString = "GMT0" },
            { .name = "Africa/Gaborone", .timezoneString = "CAT-2" },
            { .name = "Africa/Harare", .timezoneString = "CAT-2" },
            { .name = "Africa/Johannesburg", .timezoneString = "SAST-2" },
            { .name = "Africa/Juba", .timezoneString = "CAT-2" },
            { .name = "Africa/Kampala", .timezoneString = "EAT-3" },
            { .name = "Africa/Khartoum", .timezoneString = "CAT-2" },
            { .name = "Africa/Kigali", .timezoneString = "CAT-2" },
            { .name = "Africa/Kinshasa", .timezoneString = "WAT-1" },
            { .name = "Africa/Lagos", .timezoneString = "WAT-1" },
            { .name = "Africa/Libreville", .timezoneString = "WAT-1" },
            { .name = "Africa/Lome", .timezoneString = "GMT0" },
            { .name = "Africa/Luanda", .timezoneString = "WAT-1" },
            { .name = "Africa/Lubumbashi", .timezoneString = "CAT-2" },
            { .name = "Africa/Lusaka", .timezoneString = "CAT-2" },
            { .name = "Africa/Malabo", .timezoneString = "WAT-1" },
            { .name = "Africa/Maputo", .timezoneString = "CAT-2" },
            { .name = "Africa/Maseru", .timezoneString = "SAST-2" },
            { .name = "Africa/Mbabane", .timezoneString = "SAST-2" },
            { .name = "Africa/Mogadishu", .timezoneString = "EAT-3" },
            { .name = "Africa/Monrovia", .timezoneString = "GMT0" },
            { .name = "Africa/Nairobi", .timezoneString = "EAT-3" },
            { .name = "Africa/Ndjamena", .timezoneString = "WAT-1" },
            { .name = "Africa/Niamey", .timezoneString = "WAT-1" },
            { .name = "Africa/Nouakchott", .timezoneString = "GMT0" },
            { .name = "Africa/Ouagadougou", .timezoneString = "GMT0" },
            { .name = "Africa/Porto-Novo", .timezoneString = "WAT-1" },
            { .name = "Africa/Sao_Tome", .timezoneString = "GMT0" },
            { .name = "Africa/Tripoli", .timezoneString = "EET-2" },
            { .name = "Africa/Tunis", .timezoneString = "CET-1" },
            { .name = "Africa/Windhoek", .timezoneString = "CAT-2" },
            { .name = "America/Adak", .timezoneString = "HST10HDT,M3.2.0,M11.1.0" },
            { .name = "America/Anchorage", .timezoneString = "AKST9AKDT,M3.2.0,M11.1.0" },
            { .name = "America/Anguilla", .timezoneString = "AST4" },
            { .name = "America/Antigua", .timezoneString = "AST4" },
            { .name = "America/Araguaina", .timezoneString = "<-03>3" },
            { .name = "America/Argentina/Buenos_Aires", .timezoneString = "<-03>3" },
            { .name = "America/Argentina/Catamarca", .timezoneString = "<-03>3" },
            { .name = "America/Argentina/Cordoba", .timezoneString = "<-03>3" },
            { .name = "America/Argentina/Jujuy", .timezoneString = "<-03>3" },
            { .name = "America/Argentina/La_Rioja", .timezoneString = "<-03>3" },
            { .name = "America/Argentina/Mendoza", .timezoneString = "<-03>3" },
            { .name = "America/Argentina/Rio_Gallegos", .timezoneString = "<-03>3" },
            { .name = "America/Argentina/Salta", .timezoneString = "<-03>3" },
            { .name = "America/Argentina/San_Juan", .timezoneString = "<-03>3" },
            { .name = "America/Argentina/San_Luis", .timezoneString = "<-03>3" },
            { .name = "America/Argentina/Tucuman", .timezoneString = "<-03>3" },
            { .name = "America/Argentina/Ushuaia", .timezoneString = "<-03>3" },
            { .name = "America/Aruba", .timezoneString = "AST4" },
            { .name = "America/Asuncion", .timezoneString = "<-04>4<-03>,M10.1.0/0,M3.4.0/0" },
            { .name = "America/Atikokan", .timezoneString = "EST5" },
            { .name = "America/Bahia", .timezoneString = "<-03>3" },
            { .name = "America/Bahia_Banderas", .timezoneString = "CST6CDT,M4.1.0,M10.5.0" },
            { .name = "America/Barbados", .timezoneString = "AST4" },
            { .name = "America/Belem", .timezoneString = "<-03>3" },
            { .name = "America/Belize", .timezoneString = "CST6" },
            { .name = "America/Blanc-Sablon", .timezoneString = "AST4" },
            { .name = "America/Boa_Vista", .timezoneString = "<-04>4" },
            { .name = "America/Bogota", .timezoneString = "<-05>5" },
            { .name = "America/Boise", .timezoneString = "MST7MDT,M3.2.0,M11.1.0" },
            { .name = "America/Cambridge_Bay", .timezoneString = "MST7MDT,M3.2.0,M11.1.0" },
            { .name = "America/Campo_Grande", .timezoneString = "<-04>4" },
            { .name = "America/Cancun", .timezoneString = "EST5" },
            { .name = "America/Caracas", .timezoneString = "<-04>4" },
            { .name = "America/Cayenne", .timezoneString = "<-03>3" },
            { .name = "America/Cayman", .timezoneString = "EST5" },
            { .name = "America/Chicago", .timezoneString = "CST6CDT,M3.2.0,M11.1.0" },
            { .name = "America/Chihuahua", .timezoneString = "MST7MDT,M4.1.0,M10.5.0" },
            { .name = "America/Costa_Rica", .timezoneString = "CST6" },
            { .name = "America/Creston", .timezoneString = "MST7" },
            { .name = "America/Cuiaba", .timezoneString = "<-04>4" },
            { .name = "America/Curacao", .timezoneString = "AST4" },
            { .name = "America/Danmarkshavn", .timezoneString = "GMT0" },
            { .name = "America/Dawson", .timezoneString = "MST7" },
            { .name = "America/Dawson_Creek", .timezoneString = "MST7" },
            { .name = "America/Denver", .timezoneString = "MST7MDT,M3.2.0,M11.1.0" },
            { .name = "America/Detroit", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/Dominica", .timezoneString = "AST4" },
            { .name = "America/Edmonton", .timezoneString = "MST7MDT,M3.2.0,M11.1.0" },
            { .name = "America/Eirunepe", .timezoneString = "<-05>5" },
            { .name = "America/El_Salvador", .timezoneString = "CST6" },
            { .name = "America/Fortaleza", .timezoneString = "<-03>3" },
            { .name = "America/Fort_Nelson", .timezoneString = "MST7" },
            { .name = "America/Glace_Bay", .timezoneString = "AST4ADT,M3.2.0,M11.1.0" },
            { .name = "America/Godthab", .timezoneString = "<-03>3<-02>,M3.5.0/-2,M10.5.0/-1" },
            { .name = "America/Goose_Bay", .timezoneString = "AST4ADT,M3.2.0,M11.1.0" },
            { .name = "America/Grand_Turk", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/Grenada", .timezoneString = "AST4" },
            { .name = "America/Guadeloupe", .timezoneString = "AST4" },
            { .name = "America/Guatemala", .timezoneString = "CST6" },
            { .name = "America/Guayaquil", .timezoneString = "<-05>5" },
            { .name = "America/Guyana", .timezoneString = "<-04>4" },
            { .name = "America/Halifax", .timezoneString = "AST4ADT,M3.2.0,M11.1.0" },
            { .name = "America/Havana", .timezoneString = "CST5CDT,M3.2.0/0,M11.1.0/1" },
            { .name = "America/Hermosillo", .timezoneString = "MST7" },
            { .name = "America/Indiana/Indianapolis", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/Indiana/Knox", .timezoneString = "CST6CDT,M3.2.0,M11.1.0" },
            { .name = "America/Indiana/Marengo", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/Indiana/Petersburg", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/Indiana/Tell_City", .timezoneString = "CST6CDT,M3.2.0,M11.1.0" },
            { .name = "America/Indiana/Vevay", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/Indiana/Vincennes", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/Indiana/Winamac", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/Inuvik", .timezoneString = "MST7MDT,M3.2.0,M11.1.0" },
            { .name = "America/Iqaluit", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/Jamaica", .timezoneString = "EST5" },
            { .name = "America/Juneau", .timezoneString = "AKST9AKDT,M3.2.0,M11.1.0" },
            { .name = "America/Kentucky/Louisville", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/Kentucky/Monticello", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/Kralendijk", .timezoneString = "AST4" },
            { .name = "America/La_Paz", .timezoneString = "<-04>4" },
            { .name = "America/Lima", .timezoneString = "<-05>5" },
            { .name = "America/Los_Angeles", .timezoneString = "PST8PDT,M3.2.0,M11.1.0" },
            { .name = "America/Lower_Princes", .timezoneString = "AST4" },
            { .name = "America/Maceio", .timezoneString = "<-03>3" },
            { .name = "America/Managua", .timezoneString = "CST6" },
            { .name = "America/Manaus", .timezoneString = "<-04>4" },
            { .name = "America/Marigot", .timezoneString = "AST4" },
            { .name = "America/Martinique", .timezoneString = "AST4" },
            { .name = "America/Matamoros", .timezoneString = "CST6CDT,M3.2.0,M11.1.0" },
            { .name = "America/Mazatlan", .timezoneString = "MST7MDT,M4.1.0,M10.5.0" },
            { .name = "America/Menominee", .timezoneString = "CST6CDT,M3.2.0,M11.1.0" },
            { .name = "America/Merida", .timezoneString = "CST6CDT,M4.1.0,M10.5.0" },
            { .name = "America/Metlakatla", .timezoneString = "AKST9AKDT,M3.2.0,M11.1.0" },
            { .name = "America/Mexico_City", .timezoneString = "CST6CDT,M4.1.0,M10.5.0" },
            { .name = "America/Miquelon", .timezoneString = "<-03>3<-02>,M3.2.0,M11.1.0" },
            { .name = "America/Moncton", .timezoneString = "AST4ADT,M3.2.0,M11.1.0" },
            { .name = "America/Monterrey", .timezoneString = "CST6CDT,M4.1.0,M10.5.0" },
            { .name = "America/Montevideo", .timezoneString = "<-03>3" },
            { .name = "America/Montreal", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/Montserrat", .timezoneString = "AST4" },
            { .name = "America/Nassau", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/New_York", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/Nipigon", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/Nome", .timezoneString = "AKST9AKDT,M3.2.0,M11.1.0" },
            { .name = "America/Noronha", .timezoneString = "<-02>2" },
            { .name = "America/North_Dakota/Beulah", .timezoneString = "CST6CDT,M3.2.0,M11.1.0" },
            { .name = "America/North_Dakota/Center", .timezoneString = "CST6CDT,M3.2.0,M11.1.0" },
            { .name = "America/North_Dakota/New_Salem", .timezoneString = "CST6CDT,M3.2.0,M11.1.0" },
            { .name = "America/Nuuk", .timezoneString = "<-03>3<-02>,M3.5.0/-2,M10.5.0/-1" },
            { .name = "America/Ojinaga", .timezoneString = "MST7MDT,M3.2.0,M11.1.0" },
            { .name = "America/Panama", .timezoneString = "EST5" },
            { .name = "America/Pangnirtung", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/Paramaribo", .timezoneString = "<-03>3" },
            { .name = "America/Phoenix", .timezoneString = "MST7" },
            { .name = "America/Port-au-Prince", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/Port_of_Spain", .timezoneString = "AST4" },
            { .name = "America/Porto_Velho", .timezoneString = "<-04>4" },
            { .name = "America/Puerto_Rico", .timezoneString = "AST4" },
            { .name = "America/Punta_Arenas", .timezoneString = "<-03>3" },
            { .name = "America/Rainy_River", .timezoneString = "CST6CDT,M3.2.0,M11.1.0" },
            { .name = "America/Rankin_Inlet", .timezoneString = "CST6CDT,M3.2.0,M11.1.0" },
            { .name = "America/Recife", .timezoneString = "<-03>3" },
            { .name = "America/Regina", .timezoneString = "CST6" },
            { .name = "America/Resolute", .timezoneString = "CST6CDT,M3.2.0,M11.1.0" },
            { .name = "America/Rio_Branco", .timezoneString = "<-05>5" },
            { .name = "America/Santarem", .timezoneString = "<-03>3" },
            { .name = "America/Santiago", .timezoneString = "<-04>4<-03>,M9.1.6/24,M4.1.6/24" },
            { .name = "America/Santo_Domingo", .timezoneString = "AST4" },
            { .name = "America/Sao_Paulo", .timezoneString = "<-03>3" },
            { .name = "America/Scoresbysund", .timezoneString = "<-01>1<+00>,M3.5.0/0,M10.5.0/1" },
            { .name = "America/Sitka", .timezoneString = "AKST9AKDT,M3.2.0,M11.1.0" },
            { .name = "America/St_Barthelemy", .timezoneString = "AST4" },
            { .name = "America/St_Johns", .timezoneString = "NST3:30NDT,M3.2.0,M11.1.0" },
            { .name = "America/St_Kitts", .timezoneString = "AST4" },
            { .name = "America/St_Lucia", .timezoneString = "AST4" },
            { .name = "America/St_Thomas", .timezoneString = "AST4" },
            { .name = "America/St_Vincent", .timezoneString = "AST4" },
            { .name = "America/Swift_Current", .timezoneString = "CST6" },
            { .name = "America/Tegucigalpa", .timezoneString = "CST6" },
            { .name = "America/Thule", .timezoneString = "AST4ADT,M3.2.0,M11.1.0" },
            { .name = "America/Thunder_Bay", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/Tijuana", .timezoneString = "PST8PDT,M3.2.0,M11.1.0" },
            { .name = "America/Toronto", .timezoneString = "EST5EDT,M3.2.0,M11.1.0" },
            { .name = "America/Tortola", .timezoneString = "AST4" },
            { .name = "America/Vancouver", .timezoneString = "PST8PDT,M3.2.0,M11.1.0" },
            { .name = "America/Whitehorse", .timezoneString = "MST7" },
            { .name = "America/Winnipeg", .timezoneString = "CST6CDT,M3.2.0,M11.1.0" },
            { .name = "America/Yakutat", .timezoneString = "AKST9AKDT,M3.2.0,M11.1.0" },
            { .name = "America/Yellowknife", .timezoneString = "MST7MDT,M3.2.0,M11.1.0" },
            { .name = "Antarctica/Casey", .timezoneString = "<+11>-11" },
            { .name = "Antarctica/Davis", .timezoneString = "<+07>-7" },
            { .name = "Antarctica/DumontDUrville", .timezoneString = "<+10>-10" },
            { .name = "Antarctica/Macquarie", .timezoneString = "AEST-10AEDT,M10.1.0,M4.1.0/3" },
            { .name = "Antarctica/Mawson", .timezoneString = "<+05>-5" },
            { .name = "Antarctica/McMurdo", .timezoneString = "NZST-12NZDT,M9.5.0,M4.1.0/3" },
            { .name = "Antarctica/Palmer", .timezoneString = "<-03>3" },
            { .name = "Antarctica/Rothera", .timezoneString = "<-03>3" },
            { .name = "Antarctica/Syowa", .timezoneString = "<+03>-3" },
            { .name = "Antarctica/Troll", .timezoneString = "<+00>0<+02>-2,M3.5.0/1,M10.5.0/3" },
            { .name = "Antarctica/Vostok", .timezoneString = "<+06>-6" },
            { .name = "Arctic/Longyearbyen", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Asia/Aden", .timezoneString = "<+03>-3" },
            { .name = "Asia/Almaty", .timezoneString = "<+06>-6" },
            { .name = "Asia/Amman", .timezoneString = "EET-2EEST,M2.5.4/24,M10.5.5/1" },
            { .name = "Asia/Anadyr", .timezoneString = "<+12>-12" },
            { .name = "Asia/Aqtau", .timezoneString = "<+05>-5" },
            { .name = "Asia/Aqtobe", .timezoneString = "<+05>-5" },
            { .name = "Asia/Ashgabat", .timezoneString = "<+05>-5" },
            { .name = "Asia/Atyrau", .timezoneString = "<+05>-5" },
            { .name = "Asia/Baghdad", .timezoneString = "<+03>-3" },
            { .name = "Asia/Bahrain", .timezoneString = "<+03>-3" },
            { .name = "Asia/Baku", .timezoneString = "<+04>-4" },
            { .name = "Asia/Bangkok", .timezoneString = "<+07>-7" },
            { .name = "Asia/Barnaul", .timezoneString = "<+07>-7" },
            { .name = "Asia/Beirut", .timezoneString = "EET-2EEST,M3.5.0/0,M10.5.0/0" },
            { .name = "Asia/Bishkek", .timezoneString = "<+06>-6" },
            { .name = "Asia/Brunei", .timezoneString = "<+08>-8" },
            { .name = "Asia/Chita", .timezoneString = "<+09>-9" },
            { .name = "Asia/Choibalsan", .timezoneString = "<+08>-8" },
            { .name = "Asia/Colombo", .timezoneString = "<+0530>-5:30" },
            { .name = "Asia/Damascus", .timezoneString = "EET-2EEST,M3.5.5/0,M10.5.5/0" },
            { .name = "Asia/Dhaka", .timezoneString = "<+06>-6" },
            { .name = "Asia/Dili", .timezoneString = "<+09>-9" },
            { .name = "Asia/Dubai", .timezoneString = "<+04>-4" },
            { .name = "Asia/Dushanbe", .timezoneString = "<+05>-5" },
            { .name = "Asia/Famagusta", .timezoneString = "EET-2EEST,M3.5.0/3,M10.5.0/4" },
            { .name = "Asia/Gaza", .timezoneString = "EET-2EEST,M3.4.4/48,M10.5.5/1" },
            { .name = "Asia/Hebron", .timezoneString = "EET-2EEST,M3.4.4/48,M10.5.5/1" },
            { .name = "Asia/Ho_Chi_Minh", .timezoneString = "<+07>-7" },
            { .name = "Asia/Hong_Kong", .timezoneString = "HKT-8" },
            { .name = "Asia/Hovd", .timezoneString = "<+07>-7" },
            { .name = "Asia/Irkutsk", .timezoneString = "<+08>-8" },
            { .name = "Asia/Jakarta", .timezoneString = "WIB-7" },
            { .name = "Asia/Jayapura", .timezoneString = "WIT-9" },
            { .name = "Asia/Jerusalem", .timezoneString = "IST-2IDT,M3.4.4/26,M10.5.0" },
            { .name = "Asia/Kabul", .timezoneString = "<+0430>-4:30" },
            { .name = "Asia/Kamchatka", .timezoneString = "<+12>-12" },
            { .name = "Asia/Karachi", .timezoneString = "PKT-5" },
            { .name = "Asia/Kathmandu", .timezoneString = "<+0545>-5:45" },
            { .name = "Asia/Khandyga", .timezoneString = "<+09>-9" },
            { .name = "Asia/Kolkata", .timezoneString = "IST-5:30" },
            { .name = "Asia/Krasnoyarsk", .timezoneString = "<+07>-7" },
            { .name = "Asia/Kuala_Lumpur", .timezoneString = "<+08>-8" },
            { .name = "Asia/Kuching", .timezoneString = "<+08>-8" },
            { .name = "Asia/Kuwait", .timezoneString = "<+03>-3" },
            { .name = "Asia/Macau", .timezoneString = "CST-8" },
            { .name = "Asia/Magadan", .timezoneString = "<+11>-11" },
            { .name = "Asia/Makassar", .timezoneString = "WITA-8" },
            { .name = "Asia/Manila", .timezoneString = "PST-8" },
            { .name = "Asia/Muscat", .timezoneString = "<+04>-4" },
            { .name = "Asia/Nicosia", .timezoneString = "EET-2EEST,M3.5.0/3,M10.5.0/4" },
            { .name = "Asia/Novokuznetsk", .timezoneString = "<+07>-7" },
            { .name = "Asia/Novosibirsk", .timezoneString = "<+07>-7" },
            { .name = "Asia/Omsk", .timezoneString = "<+06>-6" },
            { .name = "Asia/Oral", .timezoneString = "<+05>-5" },
            { .name = "Asia/Phnom_Penh", .timezoneString = "<+07>-7" },
            { .name = "Asia/Pontianak", .timezoneString = "WIB-7" },
            { .name = "Asia/Pyongyang", .timezoneString = "KST-9" },
            { .name = "Asia/Qatar", .timezoneString = "<+03>-3" },
            { .name = "Asia/Qyzylorda", .timezoneString = "<+05>-5" },
            { .name = "Asia/Riyadh", .timezoneString = "<+03>-3" },
            { .name = "Asia/Sakhalin", .timezoneString = "<+11>-11" },
            { .name = "Asia/Samarkand", .timezoneString = "<+05>-5" },
            { .name = "Asia/Seoul", .timezoneString = "KST-9" },
            { .name = "Asia/Shanghai", .timezoneString = "CST-8" },
            { .name = "Asia/Singapore", .timezoneString = "<+08>-8" },
            { .name = "Asia/Srednekolymsk", .timezoneString = "<+11>-11" },
            { .name = "Asia/Taipei", .timezoneString = "CST-8" },
            { .name = "Asia/Tashkent", .timezoneString = "<+05>-5" },
            { .name = "Asia/Tbilisi", .timezoneString = "<+04>-4" },
            { .name = "Asia/Tehran", .timezoneString = "<+0330>-3:30<+0430>,J79/24,J263/24" },
            { .name = "Asia/Thimphu", .timezoneString = "<+06>-6" },
            { .name = "Asia/Tokyo", .timezoneString = "JST-9" },
            { .name = "Asia/Tomsk", .timezoneString = "<+07>-7" },
            { .name = "Asia/Ulaanbaatar", .timezoneString = "<+08>-8" },
            { .name = "Asia/Urumqi", .timezoneString = "<+06>-6" },
            { .name = "Asia/Ust-Nera", .timezoneString = "<+10>-10" },
            { .name = "Asia/Vientiane", .timezoneString = "<+07>-7" },
            { .name = "Asia/Vladivostok", .timezoneString = "<+10>-10" },
            { .name = "Asia/Yakutsk", .timezoneString = "<+09>-9" },
            { .name = "Asia/Yangon", .timezoneString = "<+0630>-6:30" },
            { .name = "Asia/Yekaterinburg", .timezoneString = "<+05>-5" },
            { .name = "Asia/Yerevan", .timezoneString = "<+04>-4" },
            { .name = "Atlantic/Azores", .timezoneString = "<-01>1<+00>,M3.5.0/0,M10.5.0/1" },
            { .name = "Atlantic/Bermuda", .timezoneString = "AST4ADT,M3.2.0,M11.1.0" },
            { .name = "Atlantic/Canary", .timezoneString = "WET0WEST,M3.5.0/1,M10.5.0" },
            { .name = "Atlantic/Cape_Verde", .timezoneString = "<-01>1" },
            { .name = "Atlantic/Faroe", .timezoneString = "WET0WEST,M3.5.0/1,M10.5.0" },
            { .name = "Atlantic/Madeira", .timezoneString = "WET0WEST,M3.5.0/1,M10.5.0" },
            { .name = "Atlantic/Reykjavik", .timezoneString = "GMT0" },
            { .name = "Atlantic/South_Georgia", .timezoneString = "<-02>2" },
            { .name = "Atlantic/Stanley", .timezoneString = "<-03>3" },
            { .name = "Atlantic/St_Helena", .timezoneString = "GMT0" },
            { .name = "Australia/Adelaide", .timezoneString = "ACST-9:30ACDT,M10.1.0,M4.1.0/3" },
            { .name = "Australia/Brisbane", .timezoneString = "AEST-10" },
            { .name = "Australia/Broken_Hill", .timezoneString = "ACST-9:30ACDT,M10.1.0,M4.1.0/3" },
            { .name = "Australia/Currie", .timezoneString = "AEST-10AEDT,M10.1.0,M4.1.0/3" },
            { .name = "Australia/Darwin", .timezoneString = "ACST-9:30" },
            { .name = "Australia/Eucla", .timezoneString = "<+0845>-8:45" },
            { .name = "Australia/Hobart", .timezoneString = "AEST-10AEDT,M10.1.0,M4.1.0/3" },
            { .name = "Australia/Lindeman", .timezoneString = "AEST-10" },
            { .name = "Australia/Lord_Howe", .timezoneString = "<+1030>-10:30<+11>-11,M10.1.0,M4.1.0" },
            { .name = "Australia/Melbourne", .timezoneString = "AEST-10AEDT,M10.1.0,M4.1.0/3" },
            { .name = "Australia/Perth", .timezoneString = "AWST-8" },
            { .name = "Australia/Sydney", .timezoneString = "AEST-10AEDT,M10.1.0,M4.1.0/3" },
            { .name = "Europe/Amsterdam", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Andorra", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Astrakhan", .timezoneString = "<+04>-4" },
            { .name = "Europe/Athens", .timezoneString = "EET-2EEST,M3.5.0/3,M10.5.0/4" },
            { .name = "Europe/Belgrade", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Berlin", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Bratislava", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Brussels", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Bucharest", .timezoneString = "EET-2EEST,M3.5.0/3,M10.5.0/4" },
            { .name = "Europe/Budapest", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Busingen", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Chisinau", .timezoneString = "EET-2EEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Copenhagen", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Dublin", .timezoneString = "IST-1GMT0,M10.5.0,M3.5.0/1" },
            { .name = "Europe/Gibraltar", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Guernsey", .timezoneString = "GMT0BST,M3.5.0/1,M10.5.0" },
            { .name = "Europe/Helsinki", .timezoneString = "EET-2EEST,M3.5.0/3,M10.5.0/4" },
            { .name = "Europe/Isle_of_Man", .timezoneString = "GMT0BST,M3.5.0/1,M10.5.0" },
            { .name = "Europe/Istanbul", .timezoneString = "<+03>-3" },
            { .name = "Europe/Jersey", .timezoneString = "GMT0BST,M3.5.0/1,M10.5.0" },
            { .name = "Europe/Kaliningrad", .timezoneString = "EET-2" },
            { .name = "Europe/Kiev", .timezoneString = "EET-2EEST,M3.5.0/3,M10.5.0/4" },
            { .name = "Europe/Kirov", .timezoneString = "<+03>-3" },
            { .name = "Europe/Lisbon", .timezoneString = "WET0WEST,M3.5.0/1,M10.5.0" },
            { .name = "Europe/Ljubljana", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/London", .timezoneString = "GMT0BST,M3.5.0/1,M10.5.0" },
            { .name = "Europe/Luxembourg", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Madrid", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Malta", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Mariehamn", .timezoneString = "EET-2EEST,M3.5.0/3,M10.5.0/4" },
            { .name = "Europe/Minsk", .timezoneString = "<+03>-3" },
            { .name = "Europe/Monaco", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Moscow", .timezoneString = "MSK-3" },
            { .name = "Europe/Oslo", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Paris", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Podgorica", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Prague", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Riga", .timezoneString = "EET-2EEST,M3.5.0/3,M10.5.0/4" },
            { .name = "Europe/Rome", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Samara", .timezoneString = "<+04>-4" },
            { .name = "Europe/San_Marino", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Sarajevo", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Saratov", .timezoneString = "<+04>-4" },
            { .name = "Europe/Simferopol", .timezoneString = "MSK-3" },
            { .name = "Europe/Skopje", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Sofia", .timezoneString = "EET-2EEST,M3.5.0/3,M10.5.0/4" },
            { .name = "Europe/Stockholm", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Tallinn", .timezoneString = "EET-2EEST,M3.5.0/3,M10.5.0/4" },
            { .name = "Europe/Tirane", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Ulyanovsk", .timezoneString = "<+04>-4" },
            { .name = "Europe/Uzhgorod", .timezoneString = "EET-2EEST,M3.5.0/3,M10.5.0/4" },
            { .name = "Europe/Vaduz", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Vatican", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Vienna", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Vilnius", .timezoneString = "EET-2EEST,M3.5.0/3,M10.5.0/4" },
            { .name = "Europe/Volgograd", .timezoneString = "<+03>-3" },
            { .name = "Europe/Warsaw", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Zagreb", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Europe/Zaporozhye", .timezoneString = "EET-2EEST,M3.5.0/3,M10.5.0/4" },
            { .name = "Europe/Zurich", .timezoneString = "CET-1CEST,M3.5.0,M10.5.0/3" },
            { .name = "Indian/Antananarivo", .timezoneString = "EAT-3" },
            { .name = "Indian/Chagos", .timezoneString = "<+06>-6" },
            { .name = "Indian/Christmas", .timezoneString = "<+07>-7" },
            { .name = "Indian/Cocos", .timezoneString = "<+0630>-6:30" },
            { .name = "Indian/Comoro", .timezoneString = "EAT-3" },
            { .name = "Indian/Kerguelen", .timezoneString = "<+05>-5" },
            { .name = "Indian/Mahe", .timezoneString = "<+04>-4" },
            { .name = "Indian/Maldives", .timezoneString = "<+05>-5" },
            { .name = "Indian/Mauritius", .timezoneString = "<+04>-4" },
            { .name = "Indian/Mayotte", .timezoneString = "EAT-3" },
            { .name = "Indian/Reunion", .timezoneString = "<+04>-4" },
            { .name = "Pacific/Apia", .timezoneString = "<+13>-13" },
            { .name = "Pacific/Auckland", .timezoneString = "NZST-12NZDT,M9.5.0,M4.1.0/3" },
            { .name = "Pacific/Bougainville", .timezoneString = "<+11>-11" },
            { .name = "Pacific/Chatham", .timezoneString = "<+1245>-12:45<+1345>,M9.5.0/2:45,M4.1.0/3:45" },
            { .name = "Pacific/Chuuk", .timezoneString = "<+10>-10" },
            { .name = "Pacific/Easter", .timezoneString = "<-06>6<-05>,M9.1.6/22,M4.1.6/22" },
            { .name = "Pacific/Efate", .timezoneString = "<+11>-11" },
            { .name = "Pacific/Enderbury", .timezoneString = "<+13>-13" },
            { .name = "Pacific/Fakaofo", .timezoneString = "<+13>-13" },
            { .name = "Pacific/Fiji", .timezoneString = "<+12>-12<+13>,M11.2.0,M1.2.3/99" },
            { .name = "Pacific/Funafuti", .timezoneString = "<+12>-12" },
            { .name = "Pacific/Galapagos", .timezoneString = "<-06>6" },
            { .name = "Pacific/Gambier", .timezoneString = "<-09>9" },
            { .name = "Pacific/Guadalcanal", .timezoneString = "<+11>-11" },
            { .name = "Pacific/Guam", .timezoneString = "ChST-10" },
            { .name = "Pacific/Honolulu", .timezoneString = "HST10" },
            { .name = "Pacific/Kiritimati", .timezoneString = "<+14>-14" },
            { .name = "Pacific/Kosrae", .timezoneString = "<+11>-11" },
            { .name = "Pacific/Kwajalein", .timezoneString = "<+12>-12" },
            { .name = "Pacific/Majuro", .timezoneString = "<+12>-12" },
            { .name = "Pacific/Marquesas", .timezoneString = "<-0930>9:30" },
            { .name = "Pacific/Midway", .timezoneString = "SST11" },
            { .name = "Pacific/Nauru", .timezoneString = "<+12>-12" },
            { .name = "Pacific/Niue", .timezoneString = "<-11>11" },
            { .name = "Pacific/Norfolk", .timezoneString = "<+11>-11<+12>,M10.1.0,M4.1.0/3" },
            { .name = "Pacific/Noumea", .timezoneString = "<+11>-11" },
            { .name = "Pacific/Pago_Pago", .timezoneString = "SST11" },
            { .name = "Pacific/Palau", .timezoneString = "<+09>-9" },
            { .name = "Pacific/Pitcairn", .timezoneString = "<-08>8" },
            { .name = "Pacific/Pohnpei", .timezoneString = "<+11>-11" },
            { .name = "Pacific/Port_Moresby", .timezoneString = "<+10>-10" },
            { .name = "Pacific/Rarotonga", .timezoneString = "<-10>10" },
            { .name = "Pacific/Saipan", .timezoneString = "ChST-10" },
            { .name = "Pacific/Tahiti", .timezoneString = "<-10>10" },
            { .name = "Pacific/Tarawa", .timezoneString = "<+12>-12" },
            { .name = "Pacific/Tongatapu", .timezoneString = "<+13>-13" },
            { .name = "Pacific/Wake", .timezoneString = "<+12>-12" },
            { .name = "Pacific/Wallis", .timezoneString = "<+12>-12" },
            { .name = "Etc/GMT", .timezoneString = "GMT0" },
            { .name = "Etc/GMT-0", .timezoneString = "GMT0" },
            { .name = "Etc/GMT-1", .timezoneString = "<+01>-1" },
            { .name = "Etc/GMT-2", .timezoneString = "<+02>-2" },
            { .name = "Etc/GMT-3", .timezoneString = "<+03>-3" },
            { .name = "Etc/GMT-4", .timezoneString = "<+04>-4" },
            { .name = "Etc/GMT-5", .timezoneString = "<+05>-5" },
            { .name = "Etc/GMT-6", .timezoneString = "<+06>-6" },
            { .name = "Etc/GMT-7", .timezoneString = "<+07>-7" },
            { .name = "Etc/GMT-8", .timezoneString = "<+08>-8" },
            { .name = "Etc/GMT-9", .timezoneString = "<+09>-9" },
            { .name = "Etc/GMT-10", .timezoneString = "<+10>-10" },
            { .name = "Etc/GMT-11", .timezoneString = "<+11>-11" },
            { .name = "Etc/GMT-12", .timezoneString = "<+12>-12" },
            { .name = "Etc/GMT-13", .timezoneString = "<+13>-13" },
            { .name = "Etc/GMT-14", .timezoneString = "<+14>-14" },
            { .name = "Etc/GMT0", .timezoneString = "GMT0" },
            { .name = "Etc/GMT+0", .timezoneString = "GMT0" },
            { .name = "Etc/GMT+1", .timezoneString = "<-01>1" },
            { .name = "Etc/GMT+2", .timezoneString = "<-02>2" },
            { .name = "Etc/GMT+3", .timezoneString = "<-03>3" },
            { .name = "Etc/GMT+4", .timezoneString = "<-04>4" },
            { .name = "Etc/GMT+5", .timezoneString = "<-05>5" },
            { .name = "Etc/GMT+6", .timezoneString = "<-06>6" },
            { .name = "Etc/GMT+7", .timezoneString = "<-07>7" },
            { .name = "Etc/GMT+8", .timezoneString = "<-08>8" },
            { .name = "Etc/GMT+9", .timezoneString = "<-09>9" },
            { .name = "Etc/GMT+10", .timezoneString = "<-10>10" },
            { .name = "Etc/GMT+11", .timezoneString = "<-11>11" },
            { .name = "Etc/GMT+12", .timezoneString = "<-12>12" },
            { .name = "Etc/UCT", .timezoneString = "UTC0" },
            { .name = "Etc/UTC", .timezoneString = "UTC0" },
            { .name = "Etc/Greenwich", .timezoneString = "GMT0" },
            { .name = "Etc/Universal", .timezoneString = "UTC0" },
            { .name = "Etc/Zulu", .timezoneString = "UTC0" }
           
       };
        /**
        * @brief changes the time zone
        * 
        * @param tz a pointer to the timezone you want to use
        * @param save (optional) if true, the time zone is stored on the SDCard. Therefore it will still be set after a reboot.
        * 
        * @return 
        *     - TIMEERROR_OK everything works well
        *     - TIMEERROR_FAILED could not set the timezone
        */
       virtual TIMEERROR setTimezone(const pocuterTimezone* tz, bool save = true) = 0;
       
       
       /**
        * @brief get the local time as tm struct.
        * 
        * @param time (out) a pointer to the time struct 
        * 
        * @return 
        *     - TIMEERROR_OK everything works well
        *     - TIMEERROR_FAILED could not set the timezone
        */
       virtual TIMEERROR getLocalTime(tm* time) = 0;
       
       /**
        * @brief get the gmt time as tm struct.
        * 
        * @param time (out) a pointer to the time struct 
        * 
        * @return 
        *     - TIMEERROR_OK everything works well
        *     - TIMEERROR_FAILED could not set the timezone
        */
       virtual TIMEERROR getGMTTime(tm* time) = 0;
       
       /**
        * @brief set the local time
        * 
        * @param time a pointer to the time struct with the local time
        * 
        * @return 
        *     - TIMEERROR_OK everything works well
        *     - TIMEERROR_FAILED could not set the timezone
        */
       virtual TIMEERROR setLocalTime(tm* time) = 0;
       
       /**
        * @brief get the current time zone
        * 
        * @return 
        *     - const char* a zero terminated string, the name of the timezone
        */
       virtual const char* getCurrentTimezone() = 0;
       
       /**
        * @brief set the address of the time server
        * 
        * @param timeServer a string with the timeserver ( "pool.ntp.org" for example )
        * 
        * @return 
        *     - TIMEERROR_OK everything works well
        *     - TIMEERROR_FAILED could not set the timezone
        */
       virtual TIMEERROR setTimeServer(const char* timeServer) = 0;
       
        /**
        * @brief activate / deactivate the timeserver connection
        * 
        * @param on activate / deactivate the timeserver connection
        * 
        * @return 
        *     - TIMEERROR_OK everything works well
        *     - TIMEERROR_FAILED could not set the timezone
        */
       virtual TIMEERROR setTimeServer(bool on) = 0;
       
       
       /**
        * @brief request if a timeserver connection was activated
        * 
        * 
        * @return 
        *     true / false
        *     
        */
       virtual bool isTimeServer() = 0;
       
        /**
        * @brief get the current time server address
        * 
        * 
        * @return 
        *      - const char* a zero terminated string, the name of the timezone
        *     
        */
       
       virtual const char* getTimeServer() = 0;
            
       /**
        * @brief get all available timezones
        * 
        * @param tz (out) a pointer to an array. You must not allocate memory for this.
        * @param count (out) a pointer to an uint16_t. The number of array elements will be set here.
        * 
        * @return 
        *      TIMEERROR_OK
        *     
        */
       
       TIMEERROR getAllTimezones(const pocuterTimezone** tz, uint16_t* count) {
           *tz = c_tz;
           *count = TIME_ZONES_COUNT;
           return TIMEERROR_OK;
       }
       
       
       
    private:

};



#endif /* POCUTERTIME_H */
#endif

