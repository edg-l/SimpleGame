#include "settings.h"
#include <engine/io.h>
#include <engine/logger.h>
#include <engine/textbuffer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static List *settings = NULL;

static void free_settings(void *value) {
	Setting *setting = value;

	free(setting->name);
	if (setting->type == STRING) {
		free(setting->value.s);
		free(setting->defvalue.s);
	}
	free(setting);
}

void engine_settings_init() { settings = engine_list_create_fn(free_settings); }

void engine_settings_quit() {
 engine_list_free(settings);
	settings = NULL;
}

void engine_settings_add_int(const char *name, int defvalue, int min, int max) {
	Setting *setting = malloc(sizeof(Setting));

	setting->type = INTEGER;
	setting->min = min;
	setting->max = max;

	size_t name_len = strlen(name) + 1;

	setting->name = malloc(name_len);
	memset(setting->name, 0, name_len);
	strcpy(setting->name, name);

	setting->defvalue.i = defvalue;
	setting->value.i = defvalue;
 engine_list_push_back(settings, setting, sizeof(Setting));
}

void engine_settings_add_float(const char *name, float defvalue, float min,
						float max) {
	Setting *setting = malloc(sizeof(Setting));

	setting->type = FLOAT;
	setting->min = min;
	setting->max = max;

	size_t name_len = strlen(name) + 1;

	setting->name = malloc(name_len);
	memset(setting->name, 0, name_len);
	strcpy(setting->name, name);

	setting->defvalue.f = defvalue;
	setting->value.f = defvalue;
 engine_list_push_back(settings, setting, sizeof(Setting));
}

void engine_settings_add_str(const char *name, char *defvalue, int min, int max) {
	Setting *setting = malloc(sizeof(Setting));

	setting->min = min;
	setting->max = max;

	size_t name_len = strlen(name) + 1;

	setting->type = STRING;
	setting->name = malloc(name_len);
	memset(setting->name, 0, name_len);
	strcpy(setting->name, name);

	setting->value.s = malloc(sizeof(char) * ((size_t)max + 1));
	setting->defvalue.s = malloc(sizeof(char) * ((size_t)max + 1));
	memset(setting->value.s, 0, sizeof(char) * ((size_t)max + 1));
	memset(setting->defvalue.s, 0, sizeof(char) * ((size_t)max + 1));

	strcpy(setting->defvalue.s, defvalue);
	strcpy(setting->value.s, defvalue);
 engine_list_push_back(settings, setting, sizeof(Setting));
}

Setting *settings_get(const char *name) {
	ListValue *current = settings->head;

	while (current) {
		Setting *setting = current->value;

		if (strcmp(setting->name, name) == 0) {
			return setting;
		}

		current = current->next;
	}
	return NULL;
}

void engine_settings_save(const char *name) {
 engine_log_write(LOG_INFO, "Saving settings.\n");

	TextBuffer *tb = tb_create(0);

	ListValue *current = settings->head;

	while (current) {
		Setting *setting = current->value;

		tb_add(tb, setting->name);
		tb_add(tb, ":");

		char typestr[12];
		sprintf(typestr, "%d", (int)setting->type);
		tb_add(tb, typestr);

		tb_add(tb, "=");

		char str[12];
		char fstr[64];

		switch (setting->type) {
		case INTEGER:
			sprintf(str, "%d", setting->value.i);
			tb_add(tb, str);
			break;
		case FLOAT:
			snprintf(fstr, sizeof(fstr), "%f", setting->value.f);
			tb_add(tb, fstr);
			break;
		case STRING:
			tb_add(tb, setting->value.s);
			break;
		}
		tb_add(tb, "\n");
		current = current->next;
	}

 engine_io_save(name, tb->buffer);
	tb_free(tb);
}

void engine_settings_set_int(const char *name, int value) {
	Setting *setting = engine_settings_get(name);
	if (setting && value >= setting->min && value <= setting->max)
		setting->value.i = value;
}

void engine_settings_set_float(const char *name, float value) {
	Setting *setting = engine_settings_get(name);
	if (setting && value >= setting->min && value <= setting->max)
		setting->value.f = value;
}
void engine_settings_set_str(const char *name, char *value) {
	Setting *setting = engine_settings_get(name);

	size_t len = strlen(value) + 1;

	if (setting && len >= setting->min && len <= setting->max)
		strcpy(setting->value.s, value);
}

int engine_settings_get_int(const char *name) {
	Setting *setting = engine_settings_get(name);
	if (setting)
		return setting->value.i;

 engine_log_write(LOG_WARNING, "Tried to get value from a setting that doesn't exist.");
	return 0;
}

float engine_settings_get_float(const char *name) {
	Setting *setting = engine_settings_get(name);
	if (setting)
		return setting->value.f;

 engine_log_write(LOG_WARNING, "Tried to get value from a setting that doesn't exist.");
	return 0;
}

char *settings_get_str(const char *name) {
	Setting *setting = engine_settings_get(name);
	if (setting)
		return setting->value.s;

 engine_log_write(LOG_WARNING, "Tried to get value from a setting that doesn't exist.");
	return 0;
}

void engine_settings_load(const char *name) {
	char *s = engine_io_load_app(name);

 engine_log_write(LOG_INFO, "Loading settings.\n");

	char *sname = strtok(s, ":");
	while (sname) {
		char *stype = strtok(NULL, "=");
		char *svalue = strtok(NULL, "\n");

		int type = atoi(stype);

		if (type == INTEGER) {
			int value = atoi(svalue);
		 engine_settings_set_int(sname, value);
		} else if (type == INTEGER) {
			float value = (float)atof(svalue);
		 engine_settings_set_float(sname, value);
		} else if (type == STRING) {
		 engine_settings_set_str(sname, svalue);
		}

		sname = strtok(NULL, ":");
	}

	free(s);
}
