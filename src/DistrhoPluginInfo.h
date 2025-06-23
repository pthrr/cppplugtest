#ifndef DISTRHO_PLUGIN_INFO_H_INCLUDED
#define DISTRHO_PLUGIN_INFO_H_INCLUDED

#define DISTRHO_PLUGIN_BRAND "DPF Example"
#define DISTRHO_PLUGIN_NAME "Linear Amplifier"
#define DISTRHO_PLUGIN_URI "https://github.com/example/linear-amplifier"
#define DISTRHO_PLUGIN_CLAP_ID "com.example.linear-amplifier"

#define DISTRHO_PLUGIN_HAS_UI 0
#define DISTRHO_PLUGIN_IS_RT_SAFE 1
#define DISTRHO_PLUGIN_IS_SYNTH 0
#define DISTRHO_PLUGIN_NUM_INPUTS 2
#define DISTRHO_PLUGIN_NUM_OUTPUTS 2
#define DISTRHO_PLUGIN_WANT_PROGRAMS 0
#define DISTRHO_PLUGIN_WANT_STATE 0
#define DISTRHO_PLUGIN_WANT_TIMEPOS 0

enum Parameters { kParameterGain = 0, kParameterCount };

#endif
