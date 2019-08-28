// SPDX-License-Identifier: GPL-3.0-or-later
/* SPDX-FileCopyrightText: Marek Lindner <mareklindner@neomailbox.ch>
 */

#include "commandline.h"

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "flash.h"
#include "router_images.h"
#include "socket.h"

#ifndef SOURCE_VERSION
#define SOURCE_VERSION "2018.0+"
#endif

static void usage(const char *prgname)
{
	fprintf(stderr, "Usage:\n");

	fprintf(stderr, "%s interface image\tflash router with given image\n",
		prgname);
	fprintf(stderr, "%s -v\t\t\tprints version information\n", prgname);

	fprintf(stderr, "\nOne or multiple images of the following type can be specified:\n");
	router_images_print_desc();

	fprintf(stderr, "\nThe interface has to be one of the devices that are part of the supported device list which follows.\nYou can either specify its name or the interface number.\n");
	socket_print_all_ifaces();
}

int main(int argc, char* argv[])
{
	bool print_help = false;
	bool print_version = false;
	int c;
	char *iface = NULL;
	int ret = -1;
	bool load_embedded = true;
	const char *progname = "ap51-flash";
	static struct option long_options[] = {
		{"help",	no_argument,	0,	'h'},
		{"version",	no_argument,	0,	'v'},
		{}
	};

	while ((c = getopt_long(argc, argv, "hv", long_options, NULL)) != -1) {
		switch (c) {
		case 'h':
			print_help = true;
			ret = 0;
			break;
		case 'v':
			print_version = true;
			ret = 0;
			break;
		case '?':
			print_help = true;
			break;
		}
	}

	progname = argv[0];
	if (print_help) {
		usage(progname);
		goto out;
	}

	if (print_version) {
#if defined(EMBEDDED_DESC)
		printf("ap51-flash (%s) [embedded: %s]\n", SOURCE_VERSION,
		       EMBEDDED_DESC);
#else
		printf("ap51-flash (%s)\n", SOURCE_VERSION);
#endif
		return 0;
	}

	if (argc < 2) {
		fprintf(stderr, "Error - no interface specified\n");
		usage(progname);
		goto out;
	}

	if (strlen(argv[1]) < 3)
		iface = socket_find_iface_by_index(argv[1]);

	if (!iface)
		iface = argv[1];

	argc -= 2;
	argv += 2;

	router_images_init();

	while (argc > 0) {
		ret = router_images_verify_path(argv[0]);
		if (ret < 0)
			goto out;

		argc -= 1;
		argv += 1;
		load_embedded = false;
	}

	if (load_embedded) {
		router_images_init_embedded();
	} else {
#if defined(EMBED_UBOOT) || defined(EMBED_UBNT) || defined(EMBED_CI) || defined(EMBED_CE) || defined(EMBED_ZYXEL)
		printf("Embedded image disabled\n");
#endif
	}

#if defined(DEBUG)
	printf("Listening on interface: %s\n", iface);
#endif

	if (!router_images_available()) {
		fprintf(stderr, "Error - no images specified\n");
		usage(progname);
		goto out;
	}

	ret = flash_start(iface);

out:
	return ret;
}
