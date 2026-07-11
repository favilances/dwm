/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;
static const unsigned int snap      = 32;
static const int showbar            = 1;
static const int topbar             = 1;
static const char *fonts[]          = {
	"JetBrainsMono Nerd Font:size=10",
	"Noto Sans:size=10",
	"monospace:size=10"
};
static const char dmenufont[]       = "JetBrainsMono Nerd Font:size=10";
static const char launcherlabel[]   = " noirLang ";
static const char col_bg[]          = "#111318";
static const char col_panel[]       = "#171a20";
static const char col_border[]      = "#303642";
static const char col_text[]        = "#d8dee9";
static const char col_dim[]         = "#8d96a8";
static const char col_accent[]      = "#3584e4";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_dim,   col_bg,    col_border },
	[SchemeSel]  = { col_text,  col_panel, col_accent },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" };

static const Rule rules[] = {
	/* class           instance  title  tags mask  isfloating  monitor */
	{ "Gimp",          NULL,     NULL,  1 << 7,    1,          -1 },
	{ "Firefox",       NULL,     NULL,  1 << 1,    0,          -1 },
	{ "Brave-browser", NULL,     NULL,  1 << 1,    0,          -1 },
	{ "zen-browser",   NULL,     NULL,  1 << 1,    0,          -1 },
	{ "Code",          NULL,     NULL,  1 << 2,    0,          -1 },
	{ "Zed",           NULL,     NULL,  1 << 2,    0,          -1 },
	{ "Pavucontrol",   NULL,     NULL,  0,         1,          -1 },
	{ "Blueman-manager", NULL,   NULL,  0,         1,          -1 },
	{ "Nm-connection-editor", NULL, NULL, 0,       1,          -1 },
	{ "flameshot",     NULL,     NULL,  0,         1,          -1 },
	{ "Spectacle",     NULL,     NULL,  0,         1,          -1 },
	{ "Gazelle",       NULL,     NULL,  0,         1,          -1 },
	{ "Bluetuith",     NULL,     NULL,  0,         1,          -1 },
};

/* layout(s) */
static const float mfact     = 0.55;
static const int nmaster     = 1;
static const int resizehints = 1;
static const int lockfullscreen = 1;
static const int refreshrate = 120;

static const Layout layouts[] = {
	/* symbol  arrange function */
	{ "",      tile },
	{ "",      monocle },
	{ "",      NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY, view,       {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY, tag,        {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY, toggleview, {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY, toggletag,  {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0";
static const char *launchercmd[]      = { "noir-launcher", "menu", NULL };
static const char *dmenucmd[]         = { "noir-launcher", "run", NULL };
static const char *termcmd[]          = { "alacritty", NULL };
static const char *filecmd[]          = { "noir-files", NULL };
static const char *screenshotcmd[]    = { "noir-screenshot", "gui", NULL };
static const char *screenshotguicmd[] = { "noir-screenshot", "gui", NULL };
static const char *screenshotareacmd[]= { "noir-screenshot", "area", NULL };
static const char *screenshotwincmd[] = { "noir-screenshot", "window", NULL };
static const char *powercmd[]         = { "noir-power", "shutdown", NULL };
static const char *powermenucmd[]     = { "noir-power", "menu", NULL };
static const char *logoutcmd[]        = { "noir-power", "logout", NULL };
static const char *suspendcmd[]       = { "noir-power", "suspend", NULL };
static const char *networkcmd[]       = { "noir-network", "settings", NULL };
static const char *networkmenucmd[]   = { "noir-network", "menu", NULL };
static const char *wifitogglecmd[]    = { "noir-network", "wifi-toggle", NULL };
static const char *bluetoothcmd[]     = { "noir-bluetooth", "settings", NULL };
static const char *bluetoothmenucmd[] = { "noir-bluetooth", "menu", NULL };
static const char *bttogglecmd[]      = { "noir-bluetooth", "toggle", NULL };
static const char *batterycmd[]       = { "noir-battery", "details", NULL };
static const char *volumecmd[]        = { "noir-volume", "settings", NULL };
static const char *volumemenucmd[]    = { "noir-volume", "menu", NULL };
static const char *volupcmd[]         = { "noir-volume", "up", NULL };
static const char *voldowncmd[]       = { "noir-volume", "down", NULL };
static const char *volmutecmd[]       = { "noir-volume", "toggle", NULL };
static const char *clockcmd[]         = { "noir-clock", "calendar", NULL };

static const Key keys[] = {
	/* GNOME-like shortcuts captured from gsettings */
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_space,  togglewindowstrip, {0} },
	{ MODKEY,                       XK_a,      spawn,          {.v = launchercmd } },
	{ Mod1Mask,                     XK_F2,     spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_e,      spawn,          {.v = filecmd } },
	{ MODKEY,                       XK_b,      spawn,          SHCMD("zen-browser --new-window") },
	{ MODKEY|ShiftMask,             XK_b,      spawn,          SHCMD("zen-browser --private-window") },
	{ MODKEY|ControlMask,           XK_b,      spawn,          SHCMD("zen-browser --blank-window") },
	{ MODKEY|ShiftMask,             XK_w,      spawn,          SHCMD("zapzap") },
	{ MODKEY|ShiftMask,             XK_g,      spawn,          SHCMD("brave-origin-beta --app='https://chatgpt.com'") },
	{ MODKEY|ShiftMask,             XK_z,      spawn,          SHCMD("zeditor") },
	{ MODKEY|ShiftMask,             XK_c,      spawn,          SHCMD("code") },
	{ MODKEY|ControlMask,           XK_g,      spawn,          SHCMD("$HOME/.local/bin/dev-workspace") },
	{ MODKEY|ShiftMask,             XK_t,      spawn,          SHCMD("xdg-email") },
	{ MODKEY,                       XK_F1,     spawn,          SHCMD("xdg-open https://wiki.archlinux.org/title/Dwm") },
	{ MODKEY|ControlMask,           XK_comma,  spawn,          SHCMD("xfce4-settings-manager || pavucontrol") },
	{ MODKEY,                       XK_p,      spawn,          SHCMD("arandr") },
	{ 0,                            XF86XK_Display, spawn,     SHCMD("arandr") },
	{ MODKEY,                       XK_l,      spawn,          SHCMD("slock") },
	{ MODKEY,                       XK_n,      spawn,          SHCMD("dunstctl history-pop || true") },
	{ MODKEY,                       XK_v,      spawn,          SHCMD("dunstctl history-pop || true") },
	{ MODKEY,                       XK_s,      spawn,          {.v = networkcmd } },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = screenshotareacmd } },
	{ MODKEY|ShiftMask,             XK_y,      togglebar,      {0} },
	{ 0,                            XK_Print,  spawn,          {.v = screenshotcmd } },
	{ MODKEY,                       XK_Print,  spawn,          {.v = screenshotguicmd } },
	{ Mod1Mask,                     XK_Print,  spawn,          {.v = screenshotwincmd } },
	{ ControlMask|ShiftMask|Mod1Mask, XK_r,    spawn,          {.v = screenshotcmd } },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ Mod1Mask,                     XK_F4,     killclient,     {0} },
	{ ControlMask|Mod1Mask,         XK_Delete, spawn,          {.v = logoutcmd } },

	/* dwm navigation and layout */
	{ Mod1Mask,                     XK_Tab,    togglewindowstrip, {0} },
	{ Mod1Mask|ShiftMask,           XK_Tab,    focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_Tab,    togglewindowstrip, {0} },
	{ MODKEY|ShiftMask,             XK_Tab,    focusstack,     {.i = -1 } },
	{ Mod1Mask,                     XK_Escape, focusstack,     {.i = +1 } },
	{ Mod1Mask|ShiftMask,           XK_Escape, focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_j,      shiftview,      {.i = +1 } },
	{ MODKEY|ControlMask,           XK_k,      shiftview,      {.i = -1 } },
	{ MODKEY|ControlMask,           XK_Down,   shiftview,      {.i = +1 } },
	{ MODKEY|ControlMask,           XK_Up,     shiftview,      {.i = -1 } },
	{ MODKEY|ControlMask,           XK_KP_Down, shiftview,    {.i = +1 } },
	{ MODKEY|ControlMask,           XK_KP_Up, shiftview,      {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_Right,  setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Left,   setmfact,       {.f = -0.05} },
	{ MODKEY|ControlMask,           XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY|ControlMask,           XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ControlMask,           XK_Left,   setmfact,       {.f = -0.05} },
	{ MODKEY|ControlMask,           XK_Right,  setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_z,      zoom,           {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      togglefullscreen, {0} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_grave,  togglewindowstrip, {0} },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ Mod1Mask,                     XK_F7,     movemouse,      {0} },
	{ Mod1Mask,                     XK_F8,     resizemouse,    {0} },
	{ MODKEY,                       XK_End,    view,           {.ui = 1 << 9 } },
	{ MODKEY|ShiftMask,             XK_End,    tag,            {.ui = 1 << 9 } },

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	TAGKEYS(                        XK_0,                      9)

	/* hardware/media keys */
	{ 0,                            XF86XK_AudioRaiseVolume, spawn, {.v = volupcmd } },
	{ 0,                            XF86XK_AudioLowerVolume, spawn, {.v = voldowncmd } },
	{ 0,                            XF86XK_AudioMute,        spawn, {.v = volmutecmd } },
	{ ControlMask,                  XF86XK_AudioRaiseVolume, spawn, {.v = volupcmd } },
	{ ControlMask,                  XF86XK_AudioLowerVolume, spawn, {.v = voldowncmd } },
	{ ShiftMask,                    XF86XK_AudioRaiseVolume, spawn, {.v = volupcmd } },
	{ ShiftMask,                    XF86XK_AudioLowerVolume, spawn, {.v = voldowncmd } },
	{ Mod1Mask,                     XF86XK_AudioRaiseVolume, spawn, {.v = volupcmd } },
	{ Mod1Mask,                     XF86XK_AudioLowerVolume, spawn, {.v = voldowncmd } },
	{ 0,                            XF86XK_AudioMicMute,     spawn, SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
	{ 0,                            XF86XK_AudioPlay,        spawn, SHCMD("playerctl play-pause") },
	{ ControlMask,                  XF86XK_AudioPlay,        spawn, SHCMD("playerctl play-pause") },
	{ 0,                            XF86XK_AudioPause,       spawn, SHCMD("playerctl pause") },
	{ 0,                            XF86XK_AudioNext,        spawn, SHCMD("playerctl next") },
	{ ControlMask,                  XF86XK_AudioNext,        spawn, SHCMD("playerctl next") },
	{ 0,                            XF86XK_AudioPrev,        spawn, SHCMD("playerctl previous") },
	{ ControlMask,                  XF86XK_AudioPrev,        spawn, SHCMD("playerctl previous") },
	{ 0,                            XF86XK_AudioStop,        spawn, SHCMD("playerctl stop") },
	{ 0,                            XF86XK_MonBrightnessUp,  spawn, SHCMD("brightnessctl set +5%") },
	{ 0,                            XF86XK_MonBrightnessDown, spawn, SHCMD("brightnessctl set 5%-") },
	{ ShiftMask,                    XF86XK_MonBrightnessUp,  spawn, SHCMD("brightnessctl set +1%") },
	{ ShiftMask,                    XF86XK_MonBrightnessDown, spawn, SHCMD("brightnessctl set 1%-") },
	{ 0,                            XF86XK_WLAN,             spawn, {.v = wifitogglecmd } },
	{ 0,                            XF86XK_Bluetooth,        spawn, {.v = bttogglecmd } },
	{ 0,                            XF86XK_Battery,          spawn, {.v = batterycmd } },
	{ 0,                            XF86XK_PowerOff,         spawn, {.v = powermenucmd } },
	{ 0,                            XF86XK_Sleep,            spawn, {.v = suspendcmd } },
	{ 0,                            XF86XK_Calculator,       spawn, SHCMD("gnome-calculator") },
	{ 0,                            XF86XK_Mail,             spawn, SHCMD("xdg-email") },
	{ 0,                            XF86XK_WWW,              spawn, SHCMD("zen-browser --new-window") },
	{ 0,                            XF86XK_Tools,            spawn, SHCMD("brave-origin-beta --app='https://gemini.google.com'") },

	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
static const Button buttons[] = {
	/* click                event mask  button   function        argument */
	{ ClkLauncher,          0,          Button1, spawn,          {.v = launchercmd } },
	{ ClkLauncher,          0,          Button3, spawn,          {.v = dmenucmd } },
	{ ClkLtSymbol,          0,          Button1, setlayout,      {0} },
	{ ClkLtSymbol,          0,          Button3, setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,          Button2, zoom,           {0} },
	{ ClkStatusClock,       0,          Button1, spawn,          {.v = clockcmd } },
	{ ClkStatusVolume,      0,          Button1, spawn,          {.v = volumecmd } },
	{ ClkStatusVolume,      0,          Button2, spawn,          {.v = volmutecmd } },
	{ ClkStatusVolume,      0,          Button3, spawn,          {.v = volumemenucmd } },
	{ ClkStatusVolume,      0,          Button4, spawn,          {.v = volupcmd } },
	{ ClkStatusVolume,      0,          Button5, spawn,          {.v = voldowncmd } },
	{ ClkStatusBattery,     0,          Button1, spawn,          {.v = batterycmd } },
	{ ClkStatusBluetooth,   0,          Button1, spawn,          {.v = bluetoothcmd } },
	{ ClkStatusBluetooth,   0,          Button3, spawn,          {.v = bluetoothmenucmd } },
	{ ClkStatusNetwork,     0,          Button1, spawn,          {.v = networkcmd } },
	{ ClkStatusNetwork,     0,          Button2, spawn,          {.v = wifitogglecmd } },
	{ ClkStatusNetwork,     0,          Button3, spawn,          {.v = networkmenucmd } },
	{ ClkStatusPower,       0,          Button1, spawn,          {.v = powercmd } },
	{ ClkStatusPower,       0,          Button3, spawn,          {.v = powermenucmd } },
	{ ClkStatusText,        0,          Button2, spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,     Button1, movemouse,      {0} },
	{ ClkClientWin,         MODKEY,     Button2, togglefloating, {0} },
	{ ClkClientWin,         MODKEY,     Button3, resizemouse,    {0} },
	{ ClkTagBar,            0,          Button1, view,           {0} },
	{ ClkTagBar,            0,          Button3, toggleview,     {0} },
	{ ClkTagBar,            MODKEY,     Button1, tag,            {0} },
	{ ClkTagBar,            MODKEY,     Button3, toggletag,      {0} },
};
