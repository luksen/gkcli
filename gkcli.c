#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <gnome-keyring.h>

#define MAX_PW_LEN 1025

static char *password;
static char *application;

static GnomeKeyringPasswordSchema schema =  {
	GNOME_KEYRING_ITEM_GENERIC_SECRET,
	{
		{"name", GNOME_KEYRING_ATTRIBUTE_TYPE_STRING},
		{"application", GNOME_KEYRING_ATTRIBUTE_TYPE_STRING},
		{NULL, 0}
	}
};

static void die(char *fmt, ...) {
	va_list args;
	va_start(args,fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
	fflush(stderr);
	exit(EXIT_FAILURE);
}

static void die_result(GnomeKeyringResult result) {
	switch (result) {
		case (GNOME_KEYRING_RESULT_DENIED):
			die("access denied");
		case (GNOME_KEYRING_RESULT_NO_KEYRING_DAEMON):
			die("no keyring daemon");
		case (GNOME_KEYRING_RESULT_BAD_ARGUMENTS):
			die("bad arguments");
		case (GNOME_KEYRING_RESULT_IO_ERROR):
			die("problem communicating with keyring daemon");
		case (GNOME_KEYRING_RESULT_CANCELLED):
			die("operation didn't complete");
		case (GNOME_KEYRING_RESULT_NO_MATCH):
			die("no such key");
		default:
			die("unknown error");
	}
}

static void die_usage() {
	die("usage: %s (set|get|del) <name>\n\n"
	"Use Gnome Keyring in shell scripts. Only secrets stored with %s can be\n"
	"manipulated. Passwords are cut off after %d characters.", 
	application, application, MAX_PW_LEN - 1);
}

static GnomeKeyringResult set(char *name, char *password) {
	return gnome_keyring_store_password_sync(
			&schema,
			GNOME_KEYRING_DEFAULT,
			name,
			password,
			"name", name,
			"application", application,
			NULL);
}

static GnomeKeyringResult get(char *name, char **password) {
	return gnome_keyring_find_password_sync(
			&schema,
			password,
			"name", name,
			"application", application,
			NULL);
}

static GnomeKeyringResult del(char *name) {
	return gnome_keyring_delete_password_sync(
			&schema,
			"name", name,
			"application", application,
			NULL);
}

int main(int argc, char **argv) {
	application = argv[0];
	if (argc != 3)
		die_usage();
	if (!strncmp(argv[1], "set", 4)) {
		password = malloc(MAX_PW_LEN);
		if (!password)
			die("memory error");
		if (!fgets(password, MAX_PW_LEN, stdin)) {
			gnome_keyring_free_password(password);
			die("failed reading stdin");
		}
		GnomeKeyringResult result = set(argv[2], password);
		if (result != GNOME_KEYRING_RESULT_OK) {
			gnome_keyring_free_password(password);
			die_result(result);
		}
		gnome_keyring_free_password(password);
		return EXIT_SUCCESS;
	}
	if (!strncmp(argv[1], "get", 4)) {
		GnomeKeyringResult result = get(argv[2], &password);
		if (result != GNOME_KEYRING_RESULT_OK) {
			gnome_keyring_free_password(password);
			die_result(result);
		}
		printf("%s", password);
		gnome_keyring_free_password(password);
		return EXIT_SUCCESS;
	}
	if (!strncmp(argv[1], "del", 4)) {
		GnomeKeyringResult result = del(argv[2]);
		if (result != GNOME_KEYRING_RESULT_OK)
			die_result(result);
		return EXIT_SUCCESS;
	}
	die_usage(argv[0]);
}
