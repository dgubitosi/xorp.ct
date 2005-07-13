// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-
// vim:set sts=4 ts=8:

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

#ident "$XORP: xorp/rtrmgr/config_operators.cc,v 1.1 2005/07/08 20:54:20 mjh Exp $"

#include "rtrmgr_module.h"
#include "rtrmgr_error.hh"
#include "config_operators.hh"


string operator_to_str(ConfigOperator op)
{
    switch (op) {
    case OP_NONE:
	XLOG_UNREACHABLE();
    case OP_EQ:
	return string("==");
    case OP_NE:
	return string("!=");
    case OP_LT:
	return string("<");
    case OP_LTE:
	return string("<=");
    case OP_GT:
	return string(">");
    case OP_GTE:
	return string(">=");
    case OP_ASSIGN:
	return string(":");
    case OP_ADD:
	return string("add");
    case OP_SUB:
	return string("sub");
    case OP_DEL:
	return string("del");
    }
    XLOG_UNREACHABLE();
}

ConfigOperator lookup_operator(const string& s)
{
    if (s == "==") {
	return OP_EQ;
    } else if (s == "!=") {
	return OP_NE;
    } else if (s == "<") {
	return OP_LT;
    } else if (s == "<=") {
	return OP_LTE;
    } else if (s == ">") {
	return OP_GT;
    } else if (s == ">=") {
	return OP_GTE;
    } else if (s == ":") {
	return OP_ASSIGN;
    } else if (s == "=") {
	return OP_ASSIGN;
    } else if (s == "+") {
	return OP_ADD;
    } else if (s == "add") {
	return OP_ADD;
    } else if (s == "-") {
	return OP_SUB;
    } else if (s == "sub") {
	return OP_SUB;
    } else if (s == "del") {
	return OP_DEL;
    }

    /*something's wrong*/
    string errmsg;
    errmsg = "Bad operator " + s;
    xorp_throw(ParseError, errmsg);
}
