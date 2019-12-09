#ifndef ENGINE_SETTINGS_H
#define ENGINE_SETTINGS_H

#include <engine/list.h>

typedef enum SettingsType {
	INTEGER,
	FLOAT,
	STRING
} SettingsType;

typedef union SettingValue {
	int i;
	float f;
	char *s;
} SettingValue;

typedef struct Setting {
	SettingsType type;
	char *name;
	SettingValue value;
	SettingValue defvalue;
	float min;
	float max;
} Setting;

void engine_settings_init();
void engine_settings_quit();

void engine_settings_add_int(const char *name, int defvalue, int min, int max);
void engine_settings_add_float(const char *name, float defvalue, float min, float max);
void engine_settings_add_str(const char *name, char *defvalue, int min, int max);

void engine_settings_set_int(const char *name, int value);
void engine_settings_set_float(const char *name, float value);
void engine_settings_set_str(const char *name, char *value);

Setting *engine_settings_get(const char *name);

int engine_settings_get_int(const char *name);
float engine_settings_get_float(const char *name);
char *engine_settings_get_str(const char *name);

void engine_settings_save(const char *name);

void engine_settings_load(const char *name);
#endif
