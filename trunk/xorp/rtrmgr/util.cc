// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001-2005 International Computer Science Institute
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

#ident "$XORP: xorp/rtrmgr/util.cc,v 1.14 2005/07/01 18:14:44 pavlin Exp $"


#include <list>

#include "rtrmgr_module.h"

#include "libxorp/xorp.h"
#include "libxorp/xlog.h"
#include "libxorp/debug.h"

#include "util.hh"


static string s_cfg_root;
static string s_bin_root;
static string s_boot_file;

list<string>
split(const string& s, char ch)
{
    list<string> parts;
    string s2 = s;
    size_t ix;

    ix = s2.find(ch);
    while (ix != string::npos) {
	parts.push_back(s2.substr(0, ix));
	s2 = s2.substr(ix + 1, s2.size() - ix);
	ix = s2.find(ch);
    }
    if (!s2.empty())
	parts.push_back(s2);
    return parts;
}

/**
 * Find the directory of executable program.
 *
 * If the supplied string looks like a qualified path then this
 * function returns the directory component, otherwise it tries to
 * locate the named program name in the directories listed in the PATH
 * environment variable.
 * Note that in our search we do not consider the XORP root directory.
 *
 * @param program_name the name of the program to find.
 *
 * @return directory name of executable program on success, empty string
 * on failure.
 */
static string
find_executable_program_dir(const string& program_name)
{
    debug_msg("%s\n", program_name.c_str());

    if (program_name.size() >= MAXPATHLEN)
	return string("");		// Error: invalid program name

    //
    // Look for trailing slash in program_name
    //
    string::size_type slash = program_name.rfind('/');
    if (slash != string::npos) {
	string path = program_name.substr(0, slash);
	return path;
    }

    //
    // Go through the PATH environment variable and find the program location
    //
    const char* s = getenv("PATH");
    while (s != NULL && *s != '\0') {
	const char* e = strchr(s, ':');
	string path;
	if (e != NULL) {
	    path = string(s, e);
	    s = e + 1;
	} else {
	    path = string(s);
	    s = NULL;
	}
	string complete_path = path + "/" + program_name;
	if (access(complete_path.c_str(), X_OK) == 0) {
	    return path;
	}
    }

    return string("");			// Error: nothing found
}

static string
xorp_real_path(const string& path)
{
    debug_msg("path: %s\n", path.c_str());

    char rp[MAXPATHLEN];
    const char* prp = realpath(path.c_str(), rp);
    if (prp != NULL) {
	debug_msg("return %s\n", prp);
	return string(prp);
    }
    // XLOG_WARNING("realpath(%s) failed.", path.c_str());
    debug_msg("return %s\n", path.c_str());
    return path;
}

void
xorp_path_init(const char* argv0)
{
    const char* xr = getenv("XORP_ROOT");
    if (xr != NULL) {
	s_bin_root = xr;
	s_cfg_root = xr;
	s_boot_file = s_cfg_root + "/rtrmgr/config.boot";
	return;
    }

    string current_root = find_executable_program_dir(string(argv0));
    XLOG_ASSERT(! current_root.empty());
    current_root += "/..";
    current_root = xorp_real_path(current_root);

    debug_msg("current_root: %s\n", current_root.c_str());

    string build_root = xorp_real_path(XORP_BUILD_ROOT);
    debug_msg("build_root:   %s\n", build_root.c_str());
    if (current_root == build_root) {
	s_bin_root = build_root;
	s_cfg_root = xorp_real_path(XORP_SRC_ROOT);
	s_boot_file = s_cfg_root + "/rtrmgr/config.boot";

	debug_msg("s_bin_root:   %s\n", s_bin_root.c_str());
	debug_msg("s_cfg_root:   %s\n", s_cfg_root.c_str());
	debug_msg("s_boot_file:  %s\n", s_boot_file.c_str());

	return;
    }

    string install_root = xorp_real_path(XORP_INSTALL_ROOT);
    s_bin_root = install_root;
    s_cfg_root = install_root;
    s_boot_file = s_cfg_root + "/config.boot";

    debug_msg("s_bin_root:   %s\n", s_bin_root.c_str());
    debug_msg("s_cfg_root:   %s\n", s_cfg_root.c_str());
    debug_msg("s_boot_file:  %s\n", s_boot_file.c_str());
}

const string&
xorp_binary_root_dir()
{
    return s_bin_root;
}

const string&
xorp_config_root_dir()
{
    return s_cfg_root;
}

string
xorp_template_dir()
{
    return s_cfg_root + string("/etc/templates");
}

string
xorp_xrl_targets_dir()
{
    return s_cfg_root + string("/xrl/targets");
}

string
xorp_boot_file()
{
    return s_boot_file;
}

const char*
xorp_basename(const char* argv0)
{
    const char* p = strrchr(argv0, '/');
    if (p) {
	return p + 1;
    }
    return argv0;
}

string&
unquote(string& s)
{
    if (s.length() >= 2 && s[0] == '"' && s[s.size() - 1] == '"') {
	s = s.substr(1, s.size() - 2);
    }
    return s;
}

string
unquote(const string& s)
{
    if (s.length() >= 2 && s[0] == '"' && s[s.size() - 1] == '"') {
	return s.substr(1, s.size() - 2);
    }
    return s;
}

string
strip_empty_spaces(const string& s)
{
    string res = s;

    // Strip the heading and trailing empty spaces
    while (!res.empty()) {
	size_t len = res.length();
	if ((res[0] == ' ') || (res[0] == '\t')) {
	    res = res.substr(1, len - 1);
	    continue;
	}
	if ((res[len - 1] == ' ') || (res[len - 1] == '\t')) {
	    res = res.substr(0, res.length() - 1);
	    continue;
	}
	break;
    }

    return res;
}

bool
has_empty_space(const string& s)
{
    string::size_type space;

    space = s.find(' ');
    if (space == string::npos)
	space = s.find('\t');

    if (space != string::npos)
	return (true);

    return (false);
}

string
find_executable_filename(const string& program_filename)
{
    string executable_filename;
    struct stat statbuf;

    if (program_filename.size() == 0) {
	return string("");			// Error
    }

    //
    // TODO: take care of the commented-out access() calls below (by BMS).
    //
    // Comment out the access() calls for now -- xorpsh does not
    // like them, when running under sudo -u xorp (euid?) and
    // as a result xorpsh fails to start up.
    // Consider checking for it in configure.in and shipping
    // our own if we can't find it on the system.
    //
    if (program_filename[0] == '/') {
	// Absolute path name
	if (stat(program_filename.c_str(), &statbuf) == 0 &&
	    // access(program_filename.c_str(), X_OK) == 0 &&
	    S_ISREG(statbuf.st_mode)) {
	    executable_filename = program_filename;
	    return executable_filename;
	}
	return string("");			// Error
    }

    // Relative path name
    string xorp_root_dir = xorp_binary_root_dir();

    list<string> path;
    path.push_back(xorp_root_dir);

    // Expand path
    const char* p = getenv("PATH");
    if (p != NULL) {
	list<string> l2 = split(p, ':');
	path.splice(path.end(), l2);
    }

    // Search each path component
    while (!path.empty()) {
	string full_path_executable = path.front() + "/" + program_filename;
	if (stat(full_path_executable.c_str(), &statbuf) == 0 &&
	    // access(program_filename.c_str(), X_OK) == 0 &&
	    S_ISREG(statbuf.st_mode)) {
	    executable_filename = full_path_executable;
	    return executable_filename;
	}
	path.pop_front();
    }
    return string("");				// Error
}

void
find_executable_filename_and_arguments(const string& program_request,
				       string& executable_filename,
				       string& program_arguments)
{
    executable_filename = strip_empty_spaces(program_request);
    program_arguments = "";

    string::size_type space;
    space = executable_filename.find(' ');
    if (space == string::npos)
	space = executable_filename.find('\t');

    if (space != string::npos) {
	program_arguments = executable_filename.substr(space + 1);
	executable_filename = executable_filename.substr(0, space);
    }

    executable_filename = find_executable_filename(executable_filename);
    if (executable_filename.empty())
	program_arguments = "";
}
