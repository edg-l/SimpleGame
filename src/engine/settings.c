#include "settings.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <engine/logger.h>
#include <engine/io.h>

static List *settings = NULL;

static void free_settings(void *value) {
	Setting *setting = value;

	free(setting->name);
	if(setting->type == STRING) {
		free(setting->value.s);
		free(setting->defvalue.s);
	}
	free(setting);
}

void settings_init() {
	settings = list_create_fn(free_settings);
}

void settings_quit() {
	list_free(settings);
	settings = NULL;
}

void settings_add_int(const char *name, int defvalue, int min, int max) {
	Setting *setting = malloc(sizeof(Setting));

	setting->type = INTEGER;
	setting->min = min;
	setting->max = max;

	int name_len = strlen(name) + 1;

	setting->name = malloc(name_len);
	memset(setting->name, 0, name_len);
	strncpy(setting->name, name, name_len);

	setting->defvalue.i = defvalue;
	setting->value.i = defvalue;
	list_push_back(settings, setting, sizeof(Setting));
}

void settings_add_float(const char *name, float defvalue, float min, float max) {
	Setting *setting = malloc(sizeof(Setting));

	setting->type = FLOAT;
	setting->min = min;
	setting->max = max;

	int name_len = strlen(name) + 1;

	setting->name = malloc(name_len);
	memset(setting->name, 0, name_len);
	strncpy(setting->name, name, name_len);

	setting->defvalue.f = defvalue;
	setting->value.f = defvalue;
	list_push_back(settings, setting, sizeof(Setting));
}

void settings_add_str(const char *name, char *defvalue, int min, int max) {
	Setting *setting = malloc(sizeof(Setting));

	setting->min = min;
	setting->max = max;

	int name_len = strlen(name) + 1;

	setting->type = STRING;
	setting->name = malloc(name_len);
	memset(setting->name, 0, name_len);
	strncpy(setting->name, name, name_len);

	setting->value.s = malloc(sizeof(char) * max + 1);
	setting->defvalue.s = malloc(sizeof(char) * max + 1);
	memset(setting->value.s, 0, sizeof(char) * max + 1);
	memset(setting->defvalue.s, 0, sizeof(char) * max + 1);

	memcpy(setting->defvalue.s, defvalue, strlen(defvalue));
	memcpy(setting->value.s, defvalue, strlen(defvalue));
	list_push_back(settings, setting, sizeof(Setting));
}

Setting *settings_get(const char *name) {
	ListValue *current = settings->head;

	while(current) {
		Setting *setting = current->value;

		if(strcmp(setting->name, name) == 0) {
			return setting;
		}

		current = current->next;
	}
	return NULL;
}


void settings_save(const char *name) {
	log_write(LOG_INFO, "Saving settings.\n");
	int current_size = 512;
	int used_size = 0;

	char *s = malloc(sizeof(char) * current_size);
	memset(s, 0, sizeof(char) * current_size);

	ListValue *current = settings->head;

	while(current) {
		Setting *setting = current->value;

		strncat(s, setting->name, strlen(setting->name));

		strcat(s, ":");

		char typestr[12];
		sprintf(typestr, "%d", (int)setting->type);
		strcat(s, typestr);

		strcat(s, "=");

		char str[12];
		char fstr[64];

		switch(setting->type) {
			case INTEGER:
				sprintf(str, "%d", setting->value.i);
				strcat(s, str);
				break;
			case FLOAT:
				snprintf(fstr, sizeof(fstr), "%f", setting->value.f);
				strcat(s, fstr);
				break;
			case STRING:
				strncat(s, setting->value.s, strlen(setting->value.s));
				break;
		}
		strcat(s, "\n");
		current = current->next;
	}

	io_save(name, s);
}

void settings_set_int(const char *name, int value) {
	Setting *setting = settings_get(name);
	if(setting && value >= setting->min && value <= setting->max)
		setting->value.i = value;
}

void settings_set_float(const char *name, float value) {
	Setting *setting = settings_get(name);
	if(setting && value >= setting->min && value <= setting->max)
		setting->value.f = value;
}
void settings_set_str(const char *name, char *value) {
	Setting *setting = settings_get(name);

	size_t len = strlen(value) + 1;

	if(setting && len >= setting->min && len <= setting->max)
		strncpy(setting->value.s, value, len);
}

int settings_get_int(const char *name) {
	Setting *setting = settings_get(name);
	if(setting)
		return setting->value.i;

	log_write(LOG_WARNING, "Tried to get value from a setting that doesn't exist.");
	return 0;
}

float settings_get_float(const char *name) {
	Setting *setting = settings_get(name);
	if(setting)
		return setting->value.f;

	log_write(LOG_WARNING, "Tried to get value from a setting that doesn't exist.");
	return 0;
}

char *settings_get_str(const char *name) {
	Setting *setting = settings_get(name);
	if(setting)
		return setting->value.s;

	log_write(LOG_WARNING, "Tried to get value from a setting that doesn't exist.");
	return 0;
}

void settings_load(const char *name) {
	char *s = io_load_app(name);

	log_write(LOG_INFO, "Loading settings.\n");

	char *sname = strtok(s, ":");
	while(sname) {
		char *stype = strtok(NULL, "=");
		char *svalue = strtok(NULL, "\n");

		int type = atoi(stype);

		if(type == INTEGER) {
			int value = atoi(svalue);
			settings_set_int(sname, value);
		}
		else if(type == INTEGER) {
			float value = atof(svalue);
			settings_set_float(sname, value);
		}
		else if(type == STRING) {
			settings_set_str(sname, svalue);
		}

		sname = strtok(NULL, ":");
	}

	free(s);
}
