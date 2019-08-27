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

void settings_init();
void settings_quit();

void settings_add_int(const char *name, int defvalue, int min, int max);
void settings_add_float(const char *name, float defvalue, float min, float max);
void settings_add_str(const char *name, char *defvalue, int min, int max);

void settings_set_int(const char *name, int value);
void settings_set_float(const char *name, float value);
void settings_set_str(const char *name, char *value);

Setting *settings_get(const char *name);

int settings_get_int(const char *name);
float settings_get_float(const char *name);
char *settings_get_str(const char *name);

void settings_save(const char *name);

void settings_load(const char *name);
#endif
