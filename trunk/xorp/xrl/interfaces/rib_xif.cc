/*
 * Copyright (c) 2001-2003 International Computer Science Institute
 * See LICENSE file for licensing, conditions, and warranties on use.
 *
 * DO NOT EDIT THIS FILE - IT IS PROGRAMMATICALLY GENERATED
 *
 * Generated by '../scripts/clnt-gen'.
 */

#ident "$XORP: xorp/xrl/interfaces/rib_xif.cc,v 1.6 2003/03/10 23:01:30 hodson Exp $"

#include "rib_xif.hh"

bool
XrlRibV0p1Client::send_enable_rib(
	const char*	the_tgt, 
	const CB0&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/enable_rib");
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall0, cb));
}


/* Unmarshall enable_rib */
void
XrlRibV0p1Client::unmarshall0(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB0		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e, 0, 0);
	return;
    } else if (a && a->size() != 2) {
	XLOG_ERROR("Wrong number of arguments (%u != 2)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0);
	return;
    }
    bool fail;
    string reason;
    try {
	a->get("fail", fail);
	a->get("reason", reason);
    } catch (const XrlArgs::XrlAtomNotFound&) {
	XLOG_ERROR("Atom not found");
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0);
	return;
    }
    cb->dispatch(e, &fail, &reason);
}

bool
XrlRibV0p1Client::send_disable_rib(
	const char*	the_tgt, 
	const CB1&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/disable_rib");
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall1, cb));
}


/* Unmarshall disable_rib */
void
XrlRibV0p1Client::unmarshall1(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB1		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e, 0, 0);
	return;
    } else if (a && a->size() != 2) {
	XLOG_ERROR("Wrong number of arguments (%u != 2)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0);
	return;
    }
    bool fail;
    string reason;
    try {
	a->get("fail", fail);
	a->get("reason", reason);
    } catch (const XrlArgs::XrlAtomNotFound&) {
	XLOG_ERROR("Atom not found");
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0);
	return;
    }
    cb->dispatch(e, &fail, &reason);
}

bool
XrlRibV0p1Client::send_start_rib(
	const char*	the_tgt, 
	const CB2&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/start_rib");
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall2, cb));
}


/* Unmarshall start_rib */
void
XrlRibV0p1Client::unmarshall2(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB2		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e, 0, 0);
	return;
    } else if (a && a->size() != 2) {
	XLOG_ERROR("Wrong number of arguments (%u != 2)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0);
	return;
    }
    bool fail;
    string reason;
    try {
	a->get("fail", fail);
	a->get("reason", reason);
    } catch (const XrlArgs::XrlAtomNotFound&) {
	XLOG_ERROR("Atom not found");
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0);
	return;
    }
    cb->dispatch(e, &fail, &reason);
}

bool
XrlRibV0p1Client::send_stop_rib(
	const char*	the_tgt, 
	const CB3&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/stop_rib");
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall3, cb));
}


/* Unmarshall stop_rib */
void
XrlRibV0p1Client::unmarshall3(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB3		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e, 0, 0);
	return;
    } else if (a && a->size() != 2) {
	XLOG_ERROR("Wrong number of arguments (%u != 2)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0);
	return;
    }
    bool fail;
    string reason;
    try {
	a->get("fail", fail);
	a->get("reason", reason);
    } catch (const XrlArgs::XrlAtomNotFound&) {
	XLOG_ERROR("Atom not found");
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0);
	return;
    }
    cb->dispatch(e, &fail, &reason);
}

bool
XrlRibV0p1Client::send_add_rib_client(
	const char*	the_tgt, 
	const string&	target_name, 
	const uint32_t&	family, 
	const bool&	is_unicast, 
	const bool&	is_multicast, 
	const CB4&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/add_rib_client");
    x.args().add("target_name", target_name);
    x.args().add("family", family);
    x.args().add("is_unicast", is_unicast);
    x.args().add("is_multicast", is_multicast);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall4, cb));
}


/* Unmarshall add_rib_client */
void
XrlRibV0p1Client::unmarshall4(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB4		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e, 0, 0);
	return;
    } else if (a && a->size() != 2) {
	XLOG_ERROR("Wrong number of arguments (%u != 2)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0);
	return;
    }
    bool fail;
    string reason;
    try {
	a->get("fail", fail);
	a->get("reason", reason);
    } catch (const XrlArgs::XrlAtomNotFound&) {
	XLOG_ERROR("Atom not found");
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0);
	return;
    }
    cb->dispatch(e, &fail, &reason);
}

bool
XrlRibV0p1Client::send_delete_rib_client(
	const char*	the_tgt, 
	const string&	target_name, 
	const uint32_t&	family, 
	const bool&	is_unicast, 
	const bool&	is_multicast, 
	const CB5&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/delete_rib_client");
    x.args().add("target_name", target_name);
    x.args().add("family", family);
    x.args().add("is_unicast", is_unicast);
    x.args().add("is_multicast", is_multicast);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall5, cb));
}


/* Unmarshall delete_rib_client */
void
XrlRibV0p1Client::unmarshall5(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB5		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e, 0, 0);
	return;
    } else if (a && a->size() != 2) {
	XLOG_ERROR("Wrong number of arguments (%u != 2)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0);
	return;
    }
    bool fail;
    string reason;
    try {
	a->get("fail", fail);
	a->get("reason", reason);
    } catch (const XrlArgs::XrlAtomNotFound&) {
	XLOG_ERROR("Atom not found");
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0);
	return;
    }
    cb->dispatch(e, &fail, &reason);
}

bool
XrlRibV0p1Client::send_enable_rib_client(
	const char*	the_tgt, 
	const string&	target_name, 
	const uint32_t&	family, 
	const bool&	is_unicast, 
	const bool&	is_multicast, 
	const CB6&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/enable_rib_client");
    x.args().add("target_name", target_name);
    x.args().add("family", family);
    x.args().add("is_unicast", is_unicast);
    x.args().add("is_multicast", is_multicast);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall6, cb));
}


/* Unmarshall enable_rib_client */
void
XrlRibV0p1Client::unmarshall6(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB6		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e, 0, 0);
	return;
    } else if (a && a->size() != 2) {
	XLOG_ERROR("Wrong number of arguments (%u != 2)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0);
	return;
    }
    bool fail;
    string reason;
    try {
	a->get("fail", fail);
	a->get("reason", reason);
    } catch (const XrlArgs::XrlAtomNotFound&) {
	XLOG_ERROR("Atom not found");
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0);
	return;
    }
    cb->dispatch(e, &fail, &reason);
}

bool
XrlRibV0p1Client::send_disable_rib_client(
	const char*	the_tgt, 
	const string&	target_name, 
	const uint32_t&	family, 
	const bool&	is_unicast, 
	const bool&	is_multicast, 
	const CB7&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/disable_rib_client");
    x.args().add("target_name", target_name);
    x.args().add("family", family);
    x.args().add("is_unicast", is_unicast);
    x.args().add("is_multicast", is_multicast);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall7, cb));
}


/* Unmarshall disable_rib_client */
void
XrlRibV0p1Client::unmarshall7(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB7		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e, 0, 0);
	return;
    } else if (a && a->size() != 2) {
	XLOG_ERROR("Wrong number of arguments (%u != 2)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0);
	return;
    }
    bool fail;
    string reason;
    try {
	a->get("fail", fail);
	a->get("reason", reason);
    } catch (const XrlArgs::XrlAtomNotFound&) {
	XLOG_ERROR("Atom not found");
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0);
	return;
    }
    cb->dispatch(e, &fail, &reason);
}

bool
XrlRibV0p1Client::send_no_fea(
	const char*	the_tgt, 
	const CB8&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/no_fea");
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall8, cb));
}


/* Unmarshall no_fea */
void
XrlRibV0p1Client::unmarshall8(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB8		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_add_igp_table4(
	const char*	the_tgt, 
	const string&	protocol, 
	const bool&	unicast, 
	const bool&	multicast, 
	const CB9&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/add_igp_table4");
    x.args().add("protocol", protocol);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall9, cb));
}


/* Unmarshall add_igp_table4 */
void
XrlRibV0p1Client::unmarshall9(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB9		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_add_igp_table6(
	const char*	the_tgt, 
	const string&	protocol, 
	const bool&	unicast, 
	const bool&	multicast, 
	const CB10&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/add_igp_table6");
    x.args().add("protocol", protocol);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall10, cb));
}


/* Unmarshall add_igp_table6 */
void
XrlRibV0p1Client::unmarshall10(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB10		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_delete_igp_table4(
	const char*	the_tgt, 
	const string&	protocol, 
	const bool&	unicast, 
	const bool&	multicast, 
	const CB11&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/delete_igp_table4");
    x.args().add("protocol", protocol);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall11, cb));
}


/* Unmarshall delete_igp_table4 */
void
XrlRibV0p1Client::unmarshall11(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB11		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_delete_igp_table6(
	const char*	the_tgt, 
	const string&	protocol, 
	const bool&	unicast, 
	const bool&	multicast, 
	const CB12&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/delete_igp_table6");
    x.args().add("protocol", protocol);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall12, cb));
}


/* Unmarshall delete_igp_table6 */
void
XrlRibV0p1Client::unmarshall12(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB12		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_add_egp_table4(
	const char*	the_tgt, 
	const string&	protocol, 
	const bool&	unicast, 
	const bool&	multicast, 
	const CB13&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/add_egp_table4");
    x.args().add("protocol", protocol);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall13, cb));
}


/* Unmarshall add_egp_table4 */
void
XrlRibV0p1Client::unmarshall13(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB13		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_add_egp_table6(
	const char*	the_tgt, 
	const string&	protocol, 
	const bool&	unicast, 
	const bool&	multicast, 
	const CB14&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/add_egp_table6");
    x.args().add("protocol", protocol);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall14, cb));
}


/* Unmarshall add_egp_table6 */
void
XrlRibV0p1Client::unmarshall14(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB14		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_delete_egp_table4(
	const char*	the_tgt, 
	const string&	protocol, 
	const bool&	unicast, 
	const bool&	multicast, 
	const CB15&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/delete_egp_table4");
    x.args().add("protocol", protocol);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall15, cb));
}


/* Unmarshall delete_egp_table4 */
void
XrlRibV0p1Client::unmarshall15(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB15		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_delete_egp_table6(
	const char*	the_tgt, 
	const string&	protocol, 
	const bool&	unicast, 
	const bool&	multicast, 
	const CB16&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/delete_egp_table6");
    x.args().add("protocol", protocol);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall16, cb));
}


/* Unmarshall delete_egp_table6 */
void
XrlRibV0p1Client::unmarshall16(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB16		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_add_route4(
	const char*	the_tgt, 
	const string&	protocol, 
	const bool&	unicast, 
	const bool&	multicast, 
	const IPv4Net&	network, 
	const IPv4&	nexthop, 
	const uint32_t&	metric, 
	const CB17&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/add_route4");
    x.args().add("protocol", protocol);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    x.args().add("network", network);
    x.args().add("nexthop", nexthop);
    x.args().add("metric", metric);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall17, cb));
}


/* Unmarshall add_route4 */
void
XrlRibV0p1Client::unmarshall17(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB17		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_add_route6(
	const char*	the_tgt, 
	const string&	protocol, 
	const bool&	unicast, 
	const bool&	multicast, 
	const IPv6Net&	network, 
	const IPv6&	nexthop, 
	const uint32_t&	metric, 
	const CB18&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/add_route6");
    x.args().add("protocol", protocol);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    x.args().add("network", network);
    x.args().add("nexthop", nexthop);
    x.args().add("metric", metric);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall18, cb));
}


/* Unmarshall add_route6 */
void
XrlRibV0p1Client::unmarshall18(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB18		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_replace_route4(
	const char*	the_tgt, 
	const string&	protocol, 
	const bool&	unicast, 
	const bool&	multicast, 
	const IPv4Net&	network, 
	const IPv4&	nexthop, 
	const uint32_t&	metric, 
	const CB19&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/replace_route4");
    x.args().add("protocol", protocol);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    x.args().add("network", network);
    x.args().add("nexthop", nexthop);
    x.args().add("metric", metric);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall19, cb));
}


/* Unmarshall replace_route4 */
void
XrlRibV0p1Client::unmarshall19(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB19		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_replace_route6(
	const char*	the_tgt, 
	const string&	protocol, 
	const bool&	unicast, 
	const bool&	multicast, 
	const IPv6Net&	network, 
	const IPv6&	nexthop, 
	const uint32_t&	metric, 
	const CB20&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/replace_route6");
    x.args().add("protocol", protocol);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    x.args().add("network", network);
    x.args().add("nexthop", nexthop);
    x.args().add("metric", metric);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall20, cb));
}


/* Unmarshall replace_route6 */
void
XrlRibV0p1Client::unmarshall20(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB20		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_delete_route4(
	const char*	the_tgt, 
	const string&	protocol, 
	const bool&	unicast, 
	const bool&	multicast, 
	const IPv4Net&	network, 
	const CB21&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/delete_route4");
    x.args().add("protocol", protocol);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    x.args().add("network", network);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall21, cb));
}


/* Unmarshall delete_route4 */
void
XrlRibV0p1Client::unmarshall21(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB21		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_delete_route6(
	const char*	the_tgt, 
	const string&	protocol, 
	const bool&	unicast, 
	const bool&	multicast, 
	const IPv6Net&	network, 
	const CB22&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/delete_route6");
    x.args().add("protocol", protocol);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    x.args().add("network", network);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall22, cb));
}


/* Unmarshall delete_route6 */
void
XrlRibV0p1Client::unmarshall22(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB22		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_lookup_route4(
	const char*	the_tgt, 
	const IPv4&	addr, 
	const bool&	unicast, 
	const bool&	multicast, 
	const CB23&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/lookup_route4");
    x.args().add("addr", addr);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall23, cb));
}


/* Unmarshall lookup_route4 */
void
XrlRibV0p1Client::unmarshall23(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB23		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e, 0);
	return;
    } else if (a && a->size() != 1) {
	XLOG_ERROR("Wrong number of arguments (%u != 1)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS(), 0);
	return;
    }
    IPv4 nexthop;
    try {
	a->get("nexthop", nexthop);
    } catch (const XrlArgs::XrlAtomNotFound&) {
	XLOG_ERROR("Atom not found");
	cb->dispatch(XrlError::BAD_ARGS(), 0);
	return;
    }
    cb->dispatch(e, &nexthop);
}

bool
XrlRibV0p1Client::send_lookup_route6(
	const char*	the_tgt, 
	const IPv6&	addr, 
	const bool&	unicast, 
	const bool&	multicast, 
	const CB24&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/lookup_route6");
    x.args().add("addr", addr);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall24, cb));
}


/* Unmarshall lookup_route6 */
void
XrlRibV0p1Client::unmarshall24(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB24		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e, 0);
	return;
    } else if (a && a->size() != 1) {
	XLOG_ERROR("Wrong number of arguments (%u != 1)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS(), 0);
	return;
    }
    IPv6 nexthop;
    try {
	a->get("nexthop", nexthop);
    } catch (const XrlArgs::XrlAtomNotFound&) {
	XLOG_ERROR("Atom not found");
	cb->dispatch(XrlError::BAD_ARGS(), 0);
	return;
    }
    cb->dispatch(e, &nexthop);
}

bool
XrlRibV0p1Client::send_new_vif(
	const char*	the_tgt, 
	const string&	name, 
	const CB25&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/new_vif");
    x.args().add("name", name);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall25, cb));
}


/* Unmarshall new_vif */
void
XrlRibV0p1Client::unmarshall25(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB25		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_add_vif_addr4(
	const char*	the_tgt, 
	const string&	name, 
	const IPv4&	addr, 
	const IPv4Net&	subnet, 
	const CB26&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/add_vif_addr4");
    x.args().add("name", name);
    x.args().add("addr", addr);
    x.args().add("subnet", subnet);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall26, cb));
}


/* Unmarshall add_vif_addr4 */
void
XrlRibV0p1Client::unmarshall26(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB26		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_add_vif_addr6(
	const char*	the_tgt, 
	const string&	name, 
	const IPv6&	addr, 
	const IPv6Net&	subnet, 
	const CB27&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/add_vif_addr6");
    x.args().add("name", name);
    x.args().add("addr", addr);
    x.args().add("subnet", subnet);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall27, cb));
}


/* Unmarshall add_vif_addr6 */
void
XrlRibV0p1Client::unmarshall27(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB27		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_redist_enable4(
	const char*	the_tgt, 
	const string&	from, 
	const string&	to, 
	const bool&	unicast, 
	const bool&	multicast, 
	const CB28&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/redist_enable4");
    x.args().add("from", from);
    x.args().add("to", to);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall28, cb));
}


/* Unmarshall redist_enable4 */
void
XrlRibV0p1Client::unmarshall28(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB28		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_redist_enable6(
	const char*	the_tgt, 
	const string&	from, 
	const string&	to, 
	const bool&	unicast, 
	const bool&	multicast, 
	const CB29&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/redist_enable6");
    x.args().add("from", from);
    x.args().add("to", to);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall29, cb));
}


/* Unmarshall redist_enable6 */
void
XrlRibV0p1Client::unmarshall29(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB29		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_redist_disable4(
	const char*	the_tgt, 
	const string&	from, 
	const string&	to, 
	const bool&	unicast, 
	const bool&	multicast, 
	const CB30&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/redist_disable4");
    x.args().add("from", from);
    x.args().add("to", to);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall30, cb));
}


/* Unmarshall redist_disable4 */
void
XrlRibV0p1Client::unmarshall30(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB30		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_redist_disable6(
	const char*	the_tgt, 
	const string&	from, 
	const string&	to, 
	const bool&	unicast, 
	const bool&	multicast, 
	const CB31&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/redist_disable6");
    x.args().add("from", from);
    x.args().add("to", to);
    x.args().add("unicast", unicast);
    x.args().add("multicast", multicast);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall31, cb));
}


/* Unmarshall redist_disable6 */
void
XrlRibV0p1Client::unmarshall31(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB31		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_register_interest4(
	const char*	the_tgt, 
	const string&	target, 
	const IPv4&	addr, 
	const CB32&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/register_interest4");
    x.args().add("target", target);
    x.args().add("addr", addr);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall32, cb));
}


/* Unmarshall register_interest4 */
void
XrlRibV0p1Client::unmarshall32(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB32		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e, 0, 0, 0, 0, 0, 0);
	return;
    } else if (a && a->size() != 6) {
	XLOG_ERROR("Wrong number of arguments (%u != 6)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0, 0, 0, 0, 0);
	return;
    }
    bool resolves;
    IPv4 base_addr;
    uint32_t prefix;
    uint32_t realprefix;
    IPv4 nexthop;
    uint32_t metric;
    try {
	a->get("resolves", resolves);
	a->get("base_addr", base_addr);
	a->get("prefix", prefix);
	a->get("realprefix", realprefix);
	a->get("nexthop", nexthop);
	a->get("metric", metric);
    } catch (const XrlArgs::XrlAtomNotFound&) {
	XLOG_ERROR("Atom not found");
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0, 0, 0, 0, 0);
	return;
    }
    cb->dispatch(e, &resolves, &base_addr, &prefix, &realprefix, &nexthop, &metric);
}

bool
XrlRibV0p1Client::send_deregister_interest4(
	const char*	the_tgt, 
	const string&	target, 
	const IPv4&	addr, 
	const uint32_t&	prefix, 
	const CB33&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/deregister_interest4");
    x.args().add("target", target);
    x.args().add("addr", addr);
    x.args().add("prefix", prefix);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall33, cb));
}


/* Unmarshall deregister_interest4 */
void
XrlRibV0p1Client::unmarshall33(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB33		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}

bool
XrlRibV0p1Client::send_register_interest6(
	const char*	the_tgt, 
	const string&	target, 
	const IPv6&	addr, 
	const CB34&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/register_interest6");
    x.args().add("target", target);
    x.args().add("addr", addr);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall34, cb));
}


/* Unmarshall register_interest6 */
void
XrlRibV0p1Client::unmarshall34(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB34		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e, 0, 0, 0, 0, 0, 0);
	return;
    } else if (a && a->size() != 6) {
	XLOG_ERROR("Wrong number of arguments (%u != 6)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0, 0, 0, 0, 0);
	return;
    }
    bool resolves;
    IPv6 base_addr;
    uint32_t prefix;
    uint32_t realprefix;
    IPv6 nexthop;
    uint32_t metric;
    try {
	a->get("resolves", resolves);
	a->get("base_addr", base_addr);
	a->get("prefix", prefix);
	a->get("realprefix", realprefix);
	a->get("nexthop", nexthop);
	a->get("metric", metric);
    } catch (const XrlArgs::XrlAtomNotFound&) {
	XLOG_ERROR("Atom not found");
	cb->dispatch(XrlError::BAD_ARGS(), 0, 0, 0, 0, 0, 0);
	return;
    }
    cb->dispatch(e, &resolves, &base_addr, &prefix, &realprefix, &nexthop, &metric);
}

bool
XrlRibV0p1Client::send_deregister_interest6(
	const char*	the_tgt, 
	const string&	target, 
	const IPv6&	addr, 
	const uint32_t&	prefix, 
	const CB35&	cb
)
{
    Xrl x(the_tgt, "rib/0.1/deregister_interest6");
    x.args().add("target", target);
    x.args().add("addr", addr);
    x.args().add("prefix", prefix);
    return _sender->send(x, callback(this, &XrlRibV0p1Client::unmarshall35, cb));
}


/* Unmarshall deregister_interest6 */
void
XrlRibV0p1Client::unmarshall35(
	const XrlError&	e, 
	XrlArgs*	a, 
	CB35		cb
)
{
    if (e != XrlError::OKAY()) {
	cb->dispatch(e);
	return;
    } else if (a && a->size() != 0) {
	XLOG_ERROR("Wrong number of arguments (%u != 0)", (uint32_t)a->size());
	cb->dispatch(XrlError::BAD_ARGS());
	return;
    }
    cb->dispatch(e);
}
