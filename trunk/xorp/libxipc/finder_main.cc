// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001-2003 International Computer Science Institute
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software")
// to deal in the Software without restriction, subject to the conditions
// listed in the XORP LICENSE file. These conditions include: you must
// preserve this copyright notice, and you cannot mention the copyright
// holders in advertising related to the Software without their permission.
// The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
// notice is a summary of the XORP LICENSE file; the license in that file is
// legally binding.

#ident "$XORP: xorp/devnotes/template.cc,v 1.2 2003/01/16 19:08:48 mjh Exp $"

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>

#include <setjmp.h>

#include "finder_module.h"

#include "libxorp/xorp.h"
#include "libxorp/xlog.h"
#include "libxorp/eventloop.hh"

#include "sockutil.hh"
#include "finder_ng.hh"

static jmp_buf tidy_exit;

static void
trigger_exit(int signo)
{
    fprintf(stderr, "Finder caught signal: %d\n", signo);
    longjmp(tidy_exit, -1);
}

static void
install_signal_traps(sig_t func)
{
    signal(SIGINT, func);
    signal(SIGTERM, func);
}

static bool
print_twirl()
{
    static const char t[] = { '\\', '|', '/', '-' };
    static const size_t nt = sizeof(t) / sizeof(t[0]);
    static size_t n = 0;
    static char erase = '\0';

    printf("%c%c", erase, t[n % nt]); fflush(stdout);
    n++;
    erase = '\b';
    return true;
}

static void
usage()
{
    fprintf(stderr, "Usage: finder [-hv] "
	    "[-a <allowed host>] "
	    "[-n <allowed net>] "
	    "[-p <port>] "
	    "[-i <interface>]\n");
}

static bool
valid_interface(const IPv4& addr)
{
    uint32_t naddr = if_count();
    uint16_t any_up = 0;

    for (uint32_t n = 1; n <= naddr; n++) {
	string name;
	in_addr if_addr;
	uint16_t flags;

	if (if_probe(n, name, if_addr, flags) == false) 
	    continue;

	any_up |= (flags & IFF_UP);

	if (IPv4(if_addr) == addr && flags & IFF_UP) {
	    return true;
	}
    }

    if (IPv4::ANY() == addr && any_up)
	return true;

    return false;
}

static void
finder_main(int argc, char* const argv[])
{
    bool		run_verbose = false;
    IPv4		bind_addr = IPv4::ANY();
    uint16_t		bind_port = FINDER_TCP_DEFAULT_PORT;
    FinderNG::Addr4List	permitted_addrs;
    FinderNG::Net4List	permitted_nets;

    int ch;
    while ((ch = getopt(argc, argv, "a:i:n:p:hv")) != -1) {
	switch (ch) {
	case 'a':
	    //
	    // User is specifying an IPv4 address to accept finder
	    // connections from.
	    //
	    try {
		permitted_addrs.push_back(IPv4(optarg));
	    } catch (const InvalidString&) {
		fprintf(stderr, "%s is not a valid IPv4 address.\n", optarg);
		usage();
		exit(-1);
	    }
	    break;
	case 'i':
	    //
	    // User is specifying which interface to bind finder to
	    //
	    try {
		bind_addr = IPv4(optarg);
		if (valid_interface(bind_addr) == false) {
		    fprintf(stderr,
			    "%s is not the address of an active interface.\n",
			    optarg);
		    exit(-1);
		}
	    } catch (const InvalidString&) {
		fprintf(stderr, "%s is not a valid interface address.\n",
			optarg);
		usage();
		exit(-1);
	    }
	    break;
	case 'n':
	    //
	    // User is specifying a network address to accept finder
	    // connections from.
	    //
	    try {
		permitted_nets.push_back(IPv4Net(optarg));
	    } catch (const InvalidString&) {
		fprintf(stderr, "%s is not a valid IPv4 network.\n", optarg);
		usage();
		exit(-1);
	    }
	    break;
	case 'p':
	    bind_port = static_cast<uint16_t>(atoi(optarg));
	    if (bind_port == 0) {
		fprintf(stderr, "0 is not a valid port.\n");
		exit(-1);
	    }
	    break;
	case 'v':
	    run_verbose = true;
	    break;
	case 'h':
	default:
	    usage();
	    exit(-1);
	}
    }

    argc -= optind;
    argv += optind;

    if (argc != 0) {
	usage();
	exit(-1);
    }

    //
    // Add listening address to list of accepted addresses
    //
    permitted_addrs.push_back(bind_addr);
    
    install_signal_traps(trigger_exit);
    XorpUnexpectedHandler x(xorp_unexpected_handler);
    try {
	EventLoop e;
	FinderNG finder(e, bind_addr, bind_port);

	// TODO associate finder instance with xrl handler

	finder.add_permitted_addrs(permitted_addrs);
	finder.add_permitted_nets(permitted_nets);
	
	XorpTimer twirl;
	if (run_verbose)
	    twirl = e.new_periodic(250, callback(print_twirl));
	
	if (setjmp(tidy_exit) == 0) {
	    for (;;) {
		e.run();
	    }
	}
    } catch (const InvalidPort& i) {
	fprintf(stderr, "%s: a finder may already be running.\n",
		i.why().c_str());
    } catch (...) {
	xorp_catch_standard_exceptions();
    }
    install_signal_traps(SIG_DFL);
}

int
main(int argc, char * const argv[])
{
    //
    // Initialize and start xlog
    //
    xlog_init(argv[0], NULL);
    xlog_set_verbose(XLOG_VERBOSE_LOW);		// Least verbose messages
    // XXX: verbosity of the error messages temporary increased
    xlog_level_set_verbose(XLOG_LEVEL_ERROR, XLOG_VERBOSE_HIGH);
    xlog_add_default_output();
    xlog_start();

    finder_main(argc, argv);

    //
    // Gracefully stop and exit xlog
    //
    xlog_stop();
    xlog_exit();

    return 0;
}

