/* appearance */
static const int sloppyfocus               = 1;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const int smartgaps                 = 1;  /* 1 means no outer gap when there is only one window */
static const int smartborders              = 1;
static const int monoclegaps               = 0;  /* 1 means outer gaps in monocle layout */
static const unsigned int borderpx         = 2;  /* border pixel of windows */
static const unsigned int gappih           = 10; /* horiz inner gap between windows */
static const unsigned int gappiv           = 10; /* vert inner gap between windows */
static const unsigned int gappoh           = 10; /* horiz outer gap between windows and screen edge */
static const unsigned int gappov           = 10; /* vert outer gap between windows and screen edge */
static const float bordercolor[]           = {0.5, 0.5, 0.5, 1.0};
static const float focuscolor[]            = {1.0, 0.0, 0.0, 1.0};
/* To conform the xdg-protocol, set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = {0.1, 0.1, 0.1, 1.0};

static const int hide_type = 1;

/* tagging - tagcount must be no greater than 31 */
#define tagcount 9

#define SCRATCHPAD_TITLE "xyzscratchpadxyz"

/* Autostart */
static const char *const autostart[] = {
		"autostart", NULL,
        NULL /* terminate */
};

#define MENU "menuwrapper"
static const Menu menus[] = {
	/* command                            feed function        action function */
	{ MENU " -i -l 5 -p Windows",         menuwinfeed,         menuwinaction    },
	{ MENU " -i -p Layouts",              menulayoutfeed,      menulayoutaction },
};

static const Rule rules[] = {
	/* app_id     title             tags mask  iscentered  isfloating  isfullscreen  monitor  scratchkey */
	/* examples:
	{ "Gimp",     NULL,       0,         0,          1,          -1 },
	{ "firefox",  NULL,       1 << 8,    0,          1,          -1,      0   },
	*/
	{ NULL,       "Media viewer",   0,         0,          0,          1,            -1,       0  },
	{ NULL,       SCRATCHPAD_TITLE, 0,         1,          1,          0,            -1,      's' },
};

/* layout(s) */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },
	{ "[M]",      monocle },
	{ "><>",      NULL },    /* no layout function means floating behavior */
};

/* monitors */
static const MonitorRule monrules[] = {
	/* name       mfact nmaster scale layout       rotate/reflect                x    y */
	/* example of a HiDPI laptop monitor:
	{ "eDP-1",    0.5,  1,      2,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
	*/
	/* defaults */
	{ NULL,       0.55, 1,      1,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
};

/* keyboard */
static const struct xkb_rule_names xkb_rules = {
	/* can specify fields: rules, model, layout, variant, options */
	/* example:
	.options = "ctrl:nocaps",
	*/
	.layout = "us,il",
	.options = "grp:alt_shift_toggle",
};

/* numlock and capslock */
static const int numlock = 1;
static const int capslock = 0;

static const int repeat_rate = 50;
static const int repeat_delay = 300;

/* Trackpad */
static const int tap_to_click = 1;
static const int tap_and_drag = 1;
static const int drag_lock = 1;
static const int natural_scrolling = 0;
static const int disable_while_typing = 1;
static const int left_handed = 0;
static const int middle_button_emulation = 0;
/* You can choose between:
LIBINPUT_CONFIG_SCROLL_NO_SCROLL
LIBINPUT_CONFIG_SCROLL_2FG
LIBINPUT_CONFIG_SCROLL_EDGE
LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
*/
static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

/* You can choose between:
LIBINPUT_CONFIG_CLICK_METHOD_NONE
LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS
LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER
*/
static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;

/* You can choose between:
LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
*/
static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

/* You can choose between:
LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
*/
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
static const double accel_speed = 0.0;
/* You can choose between:
LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
*/
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#define MODKEY WLR_MODIFIER_LOGO
#define WLR_MODIFIER_CTRL_SHIFT WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT

#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                    KEY,            view,            {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL,  KEY,            toggleview,      {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_SHIFT, KEY,            tag,             {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT,KEY,toggletag,  {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* named scratchpads - First arg only serves to match against key in rules*/
static const char *scratchpadcmd[] = { "s", "alacritty", "-t", SCRATCHPAD_TITLE, NULL };

#include "shiftview.c"
#include "keys.h"
static const Key keys[] = {
	/* modifier                  key                 function        argument */
	{ MODKEY,                    Key_slash,          spawn,          SHCMD("menuwrapper -ri") },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_slash,          spawn,          SHCMD("menuwrapper -Ri") },
	{ MODKEY|WLR_MODIFIER_ALT,   Key_slash,          menu,           {.v = &menus[0]} },
	{ MODKEY,                    Key_Return,         spawn,          SHCMD("$TERMINAL") },
	{ MODKEY|WLR_MODIFIER_ALT,   Key_Return,         togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                    Key_Down,           focusstack,     {.i = +1} },
	{ MODKEY,                    Key_Up,             focusstack,     {.i = -1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_Down,           movestack,      {.i = +1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_Up,             movestack,      {.i = -1} },
	{ MODKEY,                    Key_i,              incnmaster,     {.i = +1} },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_i,              incnmaster,     {.i = -1} },
	{ MODKEY|WLR_MODIFIER_ALT,   Key_Left,           setmfact,       {.f = -0.05} },
	{ MODKEY|WLR_MODIFIER_ALT,   Key_Right,          setmfact,       {.f = +0.05} },
	{ MODKEY|WLR_MODIFIER_ALT,   Key_Up,             setcfact,       {.f = +0.15} },
	{ MODKEY|WLR_MODIFIER_ALT,   Key_Down,           setcfact,       {.f = -0.15} },
	{ MODKEY,                    Key_o,              incgaps,        {.i = +1 } },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_o,              incgaps,        {.i = -1 } },
	{ MODKEY|WLR_MODIFIER_ALT,   Key_o,              togglegaps,     {0} },
	{ MODKEY|WLR_MODIFIER_CTRL,  Key_o,              defaultgaps,    {0} },
	{ MODKEY,                    Key_space,          zoom,           {0} },
	{ MODKEY,                    Key_Tab,            view,           {0} },
	{ WLR_MODIFIER_ALT,          Key_Tab,            view,           {0} },
	{ MODKEY|WLR_MODIFIER_CTRL,  Key_Left,           shiftview,      { .i = -1 } },
	{ MODKEY|WLR_MODIFIER_CTRL,  Key_Right,          shiftview,      { .i = 1 } },
	{ MODKEY,                    Key_q,              killclient,     {0} },
	{ MODKEY,                    Key_t,              setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                    Key_u,              setlayout,      {.v = &layouts[1]} },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_f,              togglefloating, {0} },
	{ MODKEY,                    Key_y,              togglesticky,   {0} },
	{ MODKEY,                    Key_f,              togglefullscreen, {0} },
	{ MODKEY,                    Key_0,              view,           {.ui = ~0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_0,              tag,            {.ui = ~0} },
	{ MODKEY,                    Key_Left,           focusmon,       {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY,                    Key_Right,          focusmon,       {.i = WLR_DIRECTION_RIGHT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_Left,           tagmon,         {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_Right,          tagmon,         {.i = WLR_DIRECTION_RIGHT} },
	TAGKEYS(                     Key_1,                       0),
	TAGKEYS(                     Key_2,                       1),
	TAGKEYS(                     Key_3,                       2),
	TAGKEYS(                     Key_4,                       3),
	TAGKEYS(                     Key_5,                       4),
	TAGKEYS(                     Key_6,                       5),
	TAGKEYS(                     Key_7,                       6),
	TAGKEYS(                     Key_8,                       7),
	TAGKEYS(                     Key_9,                       8),
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_e,              quit,           {0} },

	{ MODKEY,                    Key_e,                    spawn,             SHCMD("$FILEGUI") },
	{ MODKEY,                    Key_n,                    spawn,             SHCMD("$TERMINAL -e newsboat && refbar news") },
	{ MODKEY,                    Key_a,                    spawn,             SHCMD("$TERMINAL dmc control") },
	{ MODKEY,                    Key_w,                    spawn,             SHCMD("$BROWSER") },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_w,                    spawn,             SHCMD("$BROWSERP") },
	{ MODKEY,                    Key_semicolon,            spawn,             SHCMD("menurepo $HOME/.local/repo/emoji") },
	{ MODKEY,                    Key_grave,                spawn,             SHCMD("menurepo $HOME/.local/repo/font-awesome-list") },
	{ MODKEY,                    Key_x,                    spawn,             SHCMD("lockscreen") },
	{ MODKEY,                    Key_p,                    spawn,             SHCMD("dmc pause") },
	{ MODKEY,                    Key_comma,                spawn,             SHCMD("dmc prev") },
	{ MODKEY,                    Key_period,               spawn,             SHCMD("dmc next") },
	{ MODKEY,                    Key_bracketleft,          spawn,             SHCMD("dmc backward 5") },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_bracketleft,          spawn,             SHCMD("dmc backward 5") },
	{ MODKEY,                    Key_bracketright,         spawn,             SHCMD("dmc forward 5") },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_bracketright,         spawn,             SHCMD("dmc forward 5") },
	{ MODKEY,                    Key_minus,                spawn,             SHCMD("dmc down 5") },
	{ MODKEY,                    Key_equal,                spawn,             SHCMD("dmc up 5") },
	{ MODKEY,                    Key_m,                    spawn,             SHCMD("dmc toggle") },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_m,                    spawn,             SHCMD("mediaplayer") },
	{ MODKEY,                    Key_v,                    spawn,             SHCMD("clipmgr -s") },
	{ MODKEY,                    Key_F2,                   spawn,             SHCMD("freshscreen") },
	{ MODKEY,                    Key_F3,                   spawn,             SHCMD("wdisplays") },
	{ MODKEY,                    Key_F4,                   spawn,             SHCMD("sysact") },
	{ MODKEY,                    Key_F12,                  spawn,             SHCMD("$TERMINAL -e nmtui") },
	{ MODKEY,                    Key_Insert,               spawn,             SHCMD("$TERMINAL -e notes -n") },
	{ MODKEY,                    Key_Home,                 spawn,             SHCMD("$TERMINAL -e notes -f") },
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_Home,                 spawn,             SHCMD("$TERMINAL -e notes -g") },
	{ MODKEY,                    Key_Print,                spawn,             SHCMD("screenshot") },
	{ 0,                         Key_Print,                spawn,             SHCMD("screenshot -f") },
	{ WLR_MODIFIER_SHIFT,        Key_Print,                spawn,             SHCMD("screenshot -F") },
	{ WLR_MODIFIER_CTRL,         Key_Print,                spawn,             SHCMD("screenshot -a") },
	{ WLR_MODIFIER_CTRL_SHIFT,   Key_Print,                spawn,             SHCMD("screenshot -A") },
	{ 0,                         Key_XF86MonBrightnessDown,spawn,             SHCMD("brightnessctl s 5%-") },
	{ 0,                         Key_XF86MonBrightnessUp,  spawn,             SHCMD("brightnessctl s 5%+") },
	{ 0,                         Key_XF86AudioMicMute,     spawn,             SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
	{ 0,                         Key_XF86AudioPause,       spawn,             SHCMD("dmc pause") },
	{ 0,                         Key_XF86AudioPrev,        spawn,             SHCMD("dmc prev") },
	{ 0,                         Key_XF86AudioNext,        spawn,             SHCMD("dmc next") },
	{ 0,                         Key_XF86AudioLowerVolume, spawn,             SHCMD("dmc down") },
	{ 0,                         Key_XF86AudioRaiseVolume, spawn,             SHCMD("dmc up") },
	{ 0,                         Key_XF86AudioMute,        spawn,             SHCMD("dmc toggle") },
	{ 0,                         Key_XF86AudioPlay,        spawn,             SHCMD("dmc play") },
	/* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,Key_BackSpace, quit, {0} },
#define CHVT(KEY,n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT, KEY, chvt, {.ui = (n)} }
	CHVT(Key_F1, 1), CHVT(Key_F2,  2),  CHVT(Key_F3,  3),  CHVT(Key_F4,  4),
	CHVT(Key_F5, 5), CHVT(Key_F6,  6),  CHVT(Key_F7,  7),  CHVT(Key_F8,  8),
	CHVT(Key_F9, 9), CHVT(Key_F10, 10), CHVT(Key_F11, 11), CHVT(Key_F12, 12),
};

static const Button buttons[] = {
	{ MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
	{ MODKEY, BTN_MIDDLE, togglefloating, {0} },
	{ MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize} },
};
