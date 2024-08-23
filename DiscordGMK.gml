#define DiscordInit
///(file_with_id_in_it)
// in your game's directory should be a .txt file with written in it client id of your discord app

var dllname
{dllname="plugins\Discord\Discord.gmk.dll"}

globalvar ActivityListen,ActivityPlaying,ActivityStreaming,ActivityWatching;
ActivityPlaying		=0;
ActivityStreaming	=1;
ActivityListen		=2;
ActivityWatching	=3;

global.dll_discord_initialize 		= external_define(dllname, "initialize"		, dll_cdecl, ty_real, 1, ty_real);
global.dll_discord_callback 		= external_define(dllname, "callback"		, dll_cdecl, ty_real, 0);
global.dll_discord_setActivity 		= external_define(dllname, "setActivity"	, dll_cdecl, ty_real, 3, ty_string, ty_string, ty_string);
global.dll_discord_setActivityType 	= external_define(dllname, "setActivityType", dll_cdecl, ty_real, 1,ty_real);
global.dll_discord_clearActivity 	= external_define(dllname, "clearActivity"	, dll_cdecl, ty_real, 0);

var f;f=file_text_open_read(argument0)
var ret;ret=external_call(global.dll_discord_initialize,file_text_read_real(f))
file_text_close(f)
return ret;

#define DiscordCallback
///()
//once per frame, man

return external_call(global.dll_discord_callback);

#define DiscordSetActivity
///(name,state_text,large_text)

return external_call(global.dll_discord_setActivity,argument0,argument1,argument2)

#define DiscordSetActivityType
///(type)

return external_call(global.dll_discord_setActivityType,argument0)

#define DiscordClearActivity
///()

return external_call(global.dll_discord_clearActivity)
