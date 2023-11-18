#pragma once

struct optparse {
	char **argv;
	int permute;
	int optind;
	int optopt;
	char *optarg;
	//char errmsg[64];
	int subopt;
};

enum optparse_argtype {
	OPTPARSE_NONE, OPTPARSE_REQUIRED, OPTPARSE_OPTIONAL
};

struct optparse_long {
	const char *longname;
	int shortname;
	enum optparse_argtype argtype;
};

/**
 * Initializes the parser state.
 */
void optparse_init(struct optparse *options, char **argv);

/**
 * Read the next option in the argv array.
 * @param optstring a getopt()-formatted option string.
 * @return the next option character, -1 for done, or '?' for error
 *
 * Just like getopt(), a character followed by no colons means no
 * argument. One colon means the option has a required argument. Two
 * colons means the option takes an optional argument.
 */
int optparse(struct optparse *options, const char *optstring);

#ifdef OPTPARSE_LONG
/**
* Handles GNU-style long options in addition to getopt() options.
* This works a lot like GNU's getopt_long(). The last option in
* longopts must be all zeros, marking the end of the array. The
* longindex argument may be NULL.
*/
int optparse_long(struct optparse *options,
				  const struct optparse_long *longopts,
				  int *longindex);

#endif
/**
 * Used for stepping over non-option arguments.
 * @return the next non-option argument, or NULL for no more arguments
 *
 * Argument parsing can continue with optparse() after using this
 * function. That would be used to parse the options for the
 * subcommand returned by optparse_arg(). This function allows you to
 * ignore the value of optind.
 */
char* optparse_arg(struct optparse *options);

/* Implementation */
#ifdef OPTPARSE_IMPLEMENTATION

#define OPTPARSE_MSG_INVALID "invalid option"
#define OPTPARSE_MSG_MISSING "option requires an argument"
#define OPTPARSE_MSG_TOOMANY "option takes no arguments"

static int optparse_error(struct optparse *options, const char *msg, const char *data) {
//	unsigned p = 0;
//	const char *sep = " -- '";
//	while (*msg)
//		options->errmsg[p++] = *msg++;
//	while (*sep)
//		options->errmsg[p++] = *sep++;
//	while (p < sizeof(options->errmsg) - 2 && *data)
//		options->errmsg[p++] = *data++;
//	options->errmsg[p++] = '\'';
//	options->errmsg[p++] = '\0';
	return '?';
}

void optparse_init(struct optparse *options, char **argv) {
	options->argv = argv;
	options->permute = 1;
	options->optind = argv[0] != 0;
	options->subopt = 0;
	options->optarg = 0;
	//options->errmsg[0] = '\0';
}

static int optparse_is_dashdash(const char *arg) {
	return arg != 0 && arg[0] == '-' && arg[1] == '-' && arg[2] == '\0';
}

static int optparse_is_shortopt(const char *arg) {
	return arg != 0 && arg[0] == '-' && arg[1] != '-' && arg[1] != '\0';
}
#ifdef OPTPARSE_LONG
static int optparse_is_longopt(const char *arg) {
	return arg != 0 && arg[0] == '-' && arg[1] == '-' && arg[2] != '\0';
}
#endif

static void optparse_permute(struct optparse *options, int index) {
	char *nonoption = options->argv[index];
	int i;
	for (i = index; i < options->optind - 1; i++)
		options->argv[i] = options->argv[i + 1];
	options->argv[options->optind - 1] = nonoption;
}

static int optparse_argtype(const char *optstring, char c) {
	int count = OPTPARSE_NONE;
	if (c == ':')
		return -1;
	for (; *optstring && c != *optstring; optstring++);
	if (!*optstring)
		return -1;
	if (optstring[1] == ':')
		count += optstring[2] == ':' ? 2 : 1;
	return count;
}

int optparse(struct optparse *options, const char *optstring) {
	int type;
	char *next;
	char *option = options->argv[options->optind];
	//options->errmsg[0] = '\0';
	options->optopt = 0;
	options->optarg = 0;
	if (option == 0) {
		return -1;
	} else if (optparse_is_dashdash(option)) {
		options->optind++; /* consume "--" */
		return -1;
	} else if (!optparse_is_shortopt(option)) {
		if (options->permute) {
			int index = options->optind++;
			int r = optparse(options, optstring);
			optparse_permute(options, index);
			options->optind--;
			return r;
		} else {
			return -1;
		}
	}
	option += options->subopt + 1;
	options->optopt = option[0];
	type = optparse_argtype(optstring, option[0]);
	next = options->argv[options->optind + 1];
	switch (type) {
case -1: {
	char str[2] = {0, 0};
	str[0] = option[0];
	options->optind++;
	return optparse_error(options, OPTPARSE_MSG_INVALID, str);
		 }
case OPTPARSE_NONE:
	if (option[1]) {
		options->subopt++;
	} else {
		options->subopt = 0;
		options->optind++;
	}
	return option[0];
case OPTPARSE_REQUIRED:
	options->subopt = 0;
	options->optind++;
	if (option[1]) {
		options->optarg = option + 1;
	} else if (next != 0) {
		options->optarg = next;
		options->optind++;
	} else {
		char str[2] = {0, 0};
		str[0] = option[0];
		options->optarg = 0;
		return optparse_error(options, OPTPARSE_MSG_MISSING, str);
	}
	return option[0];
case OPTPARSE_OPTIONAL:
	options->subopt = 0;
	options->optind++;
	if (option[1])
		options->optarg = option + 1;
	else
		options->optarg = 0;
	return option[0];
	}
	return 0;
}

char* optparse_arg(struct optparse *options) {
	char *option = options->argv[options->optind];
	options->subopt = 0;
	if (option != 0)
		options->optind++;
	return option;
}

#ifdef OPTPARSE_LONG
static int optparse_longopts_end(const struct optparse_long *longopts, int i) {
	return !longopts[i].longname && !longopts[i].shortname;
}

static void optparse_from_long(const struct optparse_long *longopts, char *optstring){
	char *p = optstring;
	int i;
	for (i = 0; !optparse_longopts_end(longopts, i); i++) {
		if (longopts[i].shortname && longopts[i].shortname < 127) {
			int a;
			*p++ = longopts[i].shortname;
			for (a = 0; a < (int)longopts[i].argtype; a++)
				*p++ = ':';
		}
	}
	*p = '\0';
}

/* Unlike strcmp(), handles options containing "=". */
static int optparse_longopts_match(const char *longname, const char *option) {
	const char *a = option, *n = longname;
	if (longname == 0)
		return 0;
	for (; *a && *n && *a != '='; a++, n++)
		if (*a != *n)
			return 0;
	return *n == '\0' && (*a == '\0' || *a == '=');
}

/* Return the part after "=", or NULL. */
static char *optparse_longopts_arg(char *option) {
	for (; *option && *option != '='; option++);
	if (*option == '=')
		return option + 1;
	else
		return 0;
}

static int optparse_long_fallback(struct optparse *options, const struct optparse_long *longopts, int *longindex) {
	int result;
	char optstring[96 * 3 + 1]; /* 96 ASCII printable characters */
	optparse_from_long(longopts, optstring);
	result = optparse(options, optstring);
	if (longindex != 0) {
		*longindex = -1;
		if (result != -1) {
			int i;
			for (i = 0; !optparse_longopts_end(longopts, i); i++)
				if (longopts[i].shortname == options->optopt)
					*longindex = i;
		}
	}
	return result;
}


int optparse_long(struct optparse *options, const struct optparse_long *longopts, int *longindex) {
	int i;
	char *option = options->argv[options->optind];
	if (option == 0) {
		return -1;
	} else if (optparse_is_dashdash(option)) {
		options->optind++; /* consume "--" */
		return -1;
	} else if (optparse_is_shortopt(option)) {
		return optparse_long_fallback(options, longopts, longindex);
	} else if (!optparse_is_longopt(option)) {
		if (options->permute) {
			int index = options->optind++;
			int r = optparse_long(options, longopts, longindex);
			optparse_permute(options, index);
			options->optind--;
			return r;
		} else {
			return -1;
		}
	}

	/* Parse as long option. */
	options->errmsg[0] = '\0';
	options->optopt = 0;
	options->optarg = 0;
	option += 2; /* skip "--" */
	options->optind++;
	for (i = 0; !optparse_longopts_end(longopts, i); i++) {
		const char *name = longopts[i].longname;
		if (optparse_longopts_match(name, option)) {
			char *arg;
			if (longindex)
				*longindex = i;
			options->optopt = longopts[i].shortname;
			arg = optparse_longopts_arg(option);
			if (longopts[i].argtype == OPTPARSE_NONE && arg != 0) {
				return optparse_error(options, OPTPARSE_MSG_TOOMANY, name);
			} if (arg != 0) {
				options->optarg = arg;
			} else if (longopts[i].argtype == OPTPARSE_REQUIRED) {
				options->optarg = options->argv[options->optind];
				if (options->optarg == 0)
					return optparse_error(options, OPTPARSE_MSG_MISSING, name);
				else
					options->optind++;
			}
			return options->optopt;
		}
	}
	return optparse_error(options, OPTPARSE_MSG_INVALID, option);
}
#endif /*OPTPARSE_LONG*/

#endif /* OPTPARSE_IMPLEMENTATION */
