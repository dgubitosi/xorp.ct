// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-
// vim:set sts=4 ts=8:

// Copyright (c) 2001-2011 XORP, Inc and Others
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License, Version 2, June
// 1991 as published by the Free Software Foundation. Redistribution
// and/or modification of this program under the terms of any other
// version of the GNU General Public License is not permitted.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more details,
// see the GNU General Public License, Version 2, a copy of which can be
// found in the XORP LICENSE.gpl file.
//
// XORP Inc, 2953 Bunker Hill Lane, Suite 204, Santa Clara, CA 95054, USA;
// http://xorp.net


#include "rib_module.h"

#include "libxorp/xorp.h"
#include "libxorp/xlog.h"
#include "libxorp/debug.h"
#include "libxorp/c_format.hh"

#include "register_server.hh"
#include "rib_manager.hh"
#include "rib.hh"


template <typename A>
inline OriginTable<A>*
RIB<A>::find_origin_table(const string& tablename)
{
    typename OriginTableMap::iterator li;

    li = _egp_origin_tables.find(tablename);

    if (li != _egp_origin_tables.end())
	return li->second;

    li = _igp_origin_tables.find(tablename);

    if (li != _igp_origin_tables.end())
	return li->second;

    return NULL;
}

template <typename A>
template <ProtocolType protocol_type>
inline OriginTable<A>*
RIB<A>::find_origin_table_smart(const string& tablename)
{
    switch (protocol_type) {
    case IGP:
	return find_igp_origin_table(tablename);
    case EGP:
	return find_egp_origin_table(tablename);
    }
    return NULL;
}

template <typename A>
inline OriginTable<A>*
RIB<A>::find_igp_origin_table(const string& tablename)
{
    typename OriginTableMap::iterator li;

    li = _igp_origin_tables.find(tablename);

    if (li != _igp_origin_tables.end())
	return li->second;

    return NULL;
}

template <typename A>
inline OriginTable<A>*
RIB<A>::find_egp_origin_table(const string& tablename)
{
    typename OriginTableMap::iterator li;

    li = _egp_origin_tables.find(tablename);

    if (li != _egp_origin_tables.end())
	return li->second;

    return NULL;
}

template <typename A>
inline RedistTable<A>*
RIB<A>::find_redist_table(const string& tablename)
{
    typename RedistTableMap::iterator li;

    li = _redist_tables.find(tablename);

    if (li == _redist_tables.end())
	return NULL;

    return li->second;
}

template <typename A>
inline OriginTable<A>*
RIB<A>::find_table_by_instance(const string& tablename,
			       const string& target_class,
			       const string& target_instance)
{
    typename OriginTableMap::iterator mi;

    mi = _routing_protocol_instances.find(tablename + " "
					  + target_class + " "
					  + target_instance);
    if (mi == _routing_protocol_instances.end()) {
	return NULL;
    }
    return mi->second;
}

template <typename A>
Protocol*
RIB<A>::find_protocol(const string& protocol)
{
    OriginTable<A>* ot = find_origin_table(protocol);

    if (ot)
	return &(ot->protocol());

    return NULL;
}

template <typename A>
inline int
RIB<A>::add_table(OriginTable<A>* table)
{
    const string& tablename = table->tablename();

    switch (table->protocol_type()) {
    case IGP:
	if (find_igp_origin_table(tablename) != NULL) {
	    XLOG_WARNING("add_table: table %s already exists", tablename.c_str());
	    return XORP_ERROR;
	}
	_igp_origin_tables[tablename] = table;
	break;
    case EGP:
	if (find_egp_origin_table(tablename) != NULL) {
	    XLOG_WARNING("add_table: table %s already exists", tablename.c_str());
	    return XORP_ERROR;
	}
	_egp_origin_tables[tablename] = table;
	break;
    default:
	XLOG_UNREACHABLE();
	break;
    }

    return XORP_OK;
}

template <typename A>
inline int
RIB<A>::add_table(RedistTable<A>* table)
{
    const string& tablename = table->tablename();
    if (find_redist_table(tablename) != NULL) {
	XLOG_WARNING("add_table: table %s already exists", tablename.c_str());
	return XORP_ERROR;
    }
    _redist_tables[tablename] = table;
    return XORP_OK;
}

template <typename A>
int
RIB<A>::set_protocol_admin_distance(const string& protocol_name,
				    const uint32_t& admin_distance)
{
    map<string, uint32_t>::iterator mi = _admin_distances.find(protocol_name);
    if (mi != _admin_distances.end()) {
	OriginTable<A>* ot = find_origin_table(protocol_name);
	if (NULL != ot && ot->route_count() > 0) {
	    XLOG_ERROR("May not set an admin distance for protocol \"%s\", "
		       "which has already instantiated an origin table and contains routes.",
		       protocol_name.c_str());
	    return XORP_ERROR;
        } else if (NULL != ot && ot->route_count() == 0) {
            int ret;
            ret = _ext_int_table->change_admin_distance(ot, admin_distance);
            if (ret == XORP_ERROR)
        	return XORP_ERROR;
	}
    }
    _admin_distances[protocol_name] = admin_distance;
    return XORP_OK;
}

template <typename A>
inline uint32_t
RIB<A>::get_protocol_admin_distance(const string& protocol_name)
{
    map<string, uint32_t>::iterator mi;

    mi = _admin_distances.find(protocol_name);
    if (mi == _admin_distances.end()) {
	XLOG_WARNING("Administrative distance of \"%s\" unknown.",
		     protocol_name.c_str());
	return UNKNOWN_ADMIN_DISTANCE;
    }
    return mi->second;
}

template <typename A>
inline IPExternalNextHop<A>*
RIB<A>::create_external_nexthop(const A& addr)
{
    return new IPExternalNextHop<A>(addr);
}

template <typename A>
inline IPPeerNextHop<A>*
RIB<A>::create_peer_nexthop(const A& addr)
{
    return new IPPeerNextHop<A>(addr);
}

// ----------------------------------------------------------------------------
// Naming utility methods

static inline string
redist_tablename(const string& from_table)
{
    return "Redist:" + from_table;
}

// ----------------------------------------------------------------------------
// RIB class

template <typename A>
RIB<A>::RIB(RibTransportType t, RibManager& rib_manager, EventLoop& eventloop)
    : _rib_manager(rib_manager),
      _eventloop(eventloop),
      _final_table(NULL),
      _errors_are_fatal(false),
      _connected_origin_table(NULL),
      _register_table(NULL),
      _policy_redist_table(NULL),
      _policy_connected_table(NULL),
      _ext_int_table(NULL)
{
    if (t == UNICAST) {
	_multicast = false;
    } else if (t == MULTICAST) {
	_multicast = true;
    } else {
	XLOG_FATAL("Unknown RibTransportType.");
    }

    uint32_t static_distance = 1;
    char* v = getenv("XORP_RIB_STATIC_DISTANCE");
    if (v) {
	static_distance = atoi(v);
	XLOG_INFO("Setting 'static' distance to: %u based on XORP_RIB_STATIC_DISTANCE environment variable.",
		  static_distance);
    }

    // TODO: XXX: don't use hard-coded values below!
    _admin_distances["connected"] =        CONNECTED_ADMIN_DISTANCE;
    _admin_distances["static"] =           static_distance;
    _admin_distances["eigrp-summary"] =    5;
    _admin_distances["ebgp"] =            20;
    _admin_distances["eigrp-internal"] =  90;
    _admin_distances["igrp"] =           100;
    _admin_distances["ospf"] =           110;
    _admin_distances["is-is"] =          115;
    _admin_distances["rip"] =            120;
    _admin_distances["eigrp-external"] = 170;
    _admin_distances["ibgp"] =           200;
    _admin_distances["fib2mrib"] =	 254;
    _admin_distances["unknown"] =        UNKNOWN_ADMIN_DISTANCE;
}

template <typename A>
RIB<A>::~RIB()
{
    //clear all route tables

    // This is going to be released with all igp origin tables
    // Just set the pointer to NULL here
    _connected_origin_table = NULL;

    delete _ext_int_table;
    _ext_int_table = NULL;

    while (! _igp_origin_tables.empty()) {
	delete _igp_origin_tables.begin()->second;
	_igp_origin_tables.erase(_igp_origin_tables.begin());
    }

    while (! _egp_origin_tables.empty()) {
	delete _egp_origin_tables.begin()->second;
	_egp_origin_tables.erase(_egp_origin_tables.begin());
    }

    while (! _redist_tables.empty()) {
	delete _redist_tables.begin()->second;
	_redist_tables.erase(_redist_tables.begin());
    }

    delete _register_table;
    _register_table = NULL;

    delete _policy_connected_table;
    _policy_connected_table = NULL;

    delete _policy_redist_table;
    _policy_redist_table = NULL;

    while (_vifs.empty() == false) {
	delete _vifs.begin()->second;
	_vifs.erase(_vifs.begin());
    }

    while (_deleted_vifs.empty() == false) {
	delete _deleted_vifs.begin()->second;
	_deleted_vifs.erase(_deleted_vifs.begin());
    }
}

template <typename A>
list<string>
RIB<A>::registered_protocol_names() const
{
    list<string> names;
    typename OriginTableMap::const_iterator iter;

    for (iter = _igp_origin_tables.begin(); iter != _igp_origin_tables.end(); ++iter)
	names.push_back(iter->first);

    for (iter = _egp_origin_tables.begin(); iter != _egp_origin_tables.end(); ++iter)
	names.push_back(iter->first);

    return (names);
}

template <typename A>
void
RIB<A>::initialize(RegisterServer& register_server)
{
    // Unconditionally plumb ExtInt table if it doesn't exist
    // All wining IGP, EGP and overall routes are cached in ExtInt table,
    // so we could perform faster lookup
    initialize_ext_int();

    initialize_register(register_server);

    initialize_policy_redist();

    //
    // XXX: we must initialize the final RedistTable after the
    // RegistTable has been initialized.
    //
    if (initialize_redist_all("all") != XORP_OK) {
	XLOG_FATAL("Could not initialize all-protocol redistribution "
		   "table for %s",
		   name().c_str());
    }

    if (add_igp_table("connected", "", "") != XORP_OK) {
	XLOG_FATAL("Could not add igp table \"connected\" for %s",
		   name().c_str());
    } else {
	_connected_origin_table = find_igp_origin_table("connected");
	XLOG_ASSERT(_connected_origin_table);
    }
}

template <typename A>
int
RIB<A>::initialize_policy_redist()
{
    XLOG_ASSERT(_register_table != NULL && _policy_redist_table == NULL);

    _policy_redist_table =
	new PolicyRedistTable<A>(_register_table, _rib_manager.xrl_router(),
				 _rib_manager.policy_redist_map(),
				 _multicast);

    XLOG_ASSERT(_final_table == _register_table);
    _final_table = _policy_redist_table;

    return XORP_OK;
}

template <typename A>
int
RIB<A>::initialize_redist_all(const string& all)
{
    XLOG_ASSERT(_policy_redist_table != NULL);

    XLOG_ASSERT(find_redist_table(redist_tablename(all)) == NULL);

    RedistTable<A>* r = new RedistTable<A>(redist_tablename(all), _policy_redist_table);

    if (add_table(r) != XORP_OK) {
	delete r;
	return XORP_ERROR;
    }

    //
    // Set the RedistTable as the final table
    //
    XLOG_ASSERT(_final_table == _policy_redist_table);
    _final_table = r;

    return XORP_OK;
}


template <typename A>
int
RIB<A>::initialize_ext_int()
{
    XLOG_ASSERT(!_ext_int_table);

    _ext_int_table = new ExtIntTable<A>();

    XLOG_ASSERT(_final_table == NULL);

    _final_table = _ext_int_table;

    return XORP_OK;
}

template <typename A>
int
RIB<A>::initialize_register(RegisterServer& register_server)
{
    XLOG_ASSERT(!_register_table);

    _register_table = new RegisterTable<A>("RegisterTable", register_server, _multicast);

    XLOG_ASSERT(_final_table == _ext_int_table);

    _register_table->set_parent(_ext_int_table);
    _ext_int_table->set_next_table(_register_table);
    _final_table = _register_table;

    return XORP_OK;
}

template <typename A>
template <ProtocolType	protocol_type>
int
RIB<A>::new_origin_table(const string&	tablename,
			 const string&	target_class,
			 const string&	target_instance,
			 uint16_t	admin_distance)
{
    OriginTable<A>* ot = NULL;

    ot = new TypedOriginTable<A, protocol_type>(tablename, admin_distance, _eventloop);

    if (ot == NULL || add_table(ot) != XORP_OK) {
	XLOG_WARNING("Could not add origin table %s", tablename.c_str());
	delete ot;
	return XORP_ERROR;
    }

    XLOG_ASSERT(_final_table);

    //
    // Store the XRL target instance, so we know which OriginTable to
    // shutdown if the routing protocol dies.
    //
    if (!target_instance.empty()) {
	_rib_manager.register_interest_in_target(target_class);
	_routing_protocol_instances[tablename + " "
				   + target_class + " "
				   + target_instance] = ot;
    }
    return XORP_OK;
}

template <typename A>
int
RIB<A>::add_connected_route(const RibVif<A>& vif,
			    const IPNet<A>& net,
			    const A& nexthop_addr,
			    const A& peer_addr)
{
    //
    // XXX: the connected routes are added with the
    // best possible metric (0).
    //
    add_route("connected", net, nexthop_addr, "", vif.name(), 0, PolicyTags());

    if (vif.is_p2p() && (peer_addr != A::ZERO()) && (! net.contains(peer_addr))) {
	add_route("connected", IPNet<A>(peer_addr, A::addr_bitlen()),
		  peer_addr, "", vif.name(), 0, PolicyTags());
    }

    return XORP_OK;
}

template <typename A>
int
RIB<A>::delete_connected_route(const RibVif<A>& vif, const IPNet<A>& net,
			       const A& peer_addr)
{
    delete_route("connected", net);

    if (vif.is_p2p() && (peer_addr != A::ZERO()) && (! net.contains(peer_addr))) {
	delete_route("connected", IPNet<A>(peer_addr, A::addr_bitlen()));
    }

    return XORP_OK;
}

template <typename A>
int
RIB<A>::new_vif(const string& vifname, const Vif& vif)
{
    typename map<string, RibVif<A>*>::iterator vi;
    RibVif<A>* new_rib_vif = NULL;

    debug_msg("RIB::new_vif: %s\n", vifname.c_str());
    if (_vifs.find(vifname) != _vifs.end())
	return XORP_ERROR;

    //
    // If the vif is pending deletion, then reuse it instead
    //
    vi = _deleted_vifs.find(vifname);
    if (vi != _deleted_vifs.end()) {
	// Reuse previously deleted vif
	new_rib_vif = vi->second;
	new_rib_vif->set_deleted(false);
	_deleted_vifs.erase(vi);
	new_rib_vif->copy_in(vif);
    } else {
	// Create a new vif
	new_rib_vif = new RibVif<A>(this, vif);
    }
    XLOG_ASSERT(new_rib_vif != NULL);
    _vifs[vifname] = new_rib_vif;

    if (new_rib_vif->is_underlying_vif_up()) {
	//
	// Add the directly connected routes associated with this vif
	//
	list<VifAddr>::const_iterator ai;
	for (ai = new_rib_vif->addr_list().begin();
	     ai != new_rib_vif->addr_list().end();
	     ++ai) {
	    if (ai->addr().af() != A::af())
		continue;

	    IPNet<A> subnet_addr;
	    A addr, peer_addr;
	    ai->subnet_addr().get(subnet_addr);
	    ai->addr().get(addr);
	    ai->peer_addr().get(peer_addr);
	    add_connected_route(*new_rib_vif, subnet_addr, addr, peer_addr);
	}
    }

    return XORP_OK;
}

template <typename A>
int
RIB<A>::delete_vif(const string& vifname, string& err_msg)
{
    typename map<string, RibVif<A>*>::iterator vi = _vifs.find(vifname);
    if (vi == _vifs.end()) {
	err_msg.append(c_format("RIB: %s  Cannot delete vif: %s, does not exist.\n",
				name().c_str(), vifname.c_str()));
	return XORP_ERROR;
    }
    RibVif<A>* rib_vif = vi->second;

    if (rib_vif->is_underlying_vif_up()) {
	//
	// Delete the directly connected routes associated with this vif
	//
	list<VifAddr>::const_iterator ai;
	for (ai = rib_vif->addr_list().begin();
	     ai != rib_vif->addr_list().end();
	     ++ai) {
	    if (ai->addr().af() != A::af())
		continue;

	    IPNet<A> subnet_addr;
	    A peer_addr;
	    ai->subnet_addr().get(subnet_addr);
	    ai->peer_addr().get(peer_addr);
	    delete_connected_route(*rib_vif, subnet_addr, peer_addr);
	}
    }

    _vifs.erase(vi);

    //
    // If the vif is still used by some routes, then add it to the
    // container with vifs pending deletion.
    // Otherwise just delete it.
    //
    if (rib_vif->usage_counter() > 0) {
	XLOG_ASSERT(_deleted_vifs.find(vifname) == _deleted_vifs.end());
	_deleted_vifs[vifname] = rib_vif;
	rib_vif->set_deleted(true);
    } else {
	delete rib_vif;
    }

    return XORP_OK;
}

template <typename A>
void
RIB<A>::destroy_deleted_vif(RibVif<A>* rib_vif)
{
    typename map<string, RibVif<A>*>::iterator vi = _deleted_vifs.find(rib_vif->name());

    XLOG_ASSERT(vi != _deleted_vifs.end());
    XLOG_ASSERT(vi->second == rib_vif);

    _deleted_vifs.erase(vi);
    delete rib_vif;
}

template <typename A>
int
RIB<A>::set_vif_flags(const string& vifname,
		      bool is_p2p,
		      bool is_loopback,
		      bool is_multicast,
		      bool is_broadcast,
		      bool is_up,
		      uint32_t mtu)
{
    RibVif<A>* vif = find_vif(vifname);
    if (!vif) {
	XLOG_ERROR("Attempting to set flags to non-existant Vif \"%s\"",
		   vifname.c_str());
	return XORP_ERROR;
    }

    bool old_is_up = vif->is_underlying_vif_up();

    vif->set_p2p(is_p2p);
    vif->set_loopback(is_loopback);
    vif->set_multicast_capable(is_multicast);
    vif->set_broadcast_capable(is_broadcast);
    vif->set_underlying_vif_up(is_up);
    vif->set_mtu(mtu);

    if (old_is_up == is_up)
	return XORP_OK;

    list<VifAddr>::const_iterator ai;

    if (is_up) {
	//
	// Add all connected routes
	//
	for (ai = vif->addr_list().begin();
	     ai != vif->addr_list().end();
	     ++ai) {
	    if (ai->addr().af() != A::af())
		continue;

	    IPNet<A> subnet_addr;
	    A addr, peer_addr;
	    ai->subnet_addr().get(subnet_addr);
	    ai->addr().get(addr);
	    ai->peer_addr().get(peer_addr);
	    add_connected_route(*vif, subnet_addr, addr, peer_addr);
	}
    }


    if (! is_up) {
	//
	// Delete all connected routes
	//
	for (ai = vif->addr_list().begin();
	     ai != vif->addr_list().end();
	     ++ai) {
	    if (ai->addr().af() != A::af())
		continue;

	    IPNet<A> subnet_addr;
	    A peer_addr;
	    ai->subnet_addr().get(subnet_addr);
	    ai->peer_addr().get(peer_addr);
	    delete_connected_route(*vif, subnet_addr, peer_addr);
	}
    }

    return XORP_OK;
}

template <typename A>
int
RIB<A>::add_vif_address(const string&	vifname,
			const A&	addr,
			const IPNet<A>&	subnet,
			const A&	broadcast_addr,
			const A&	peer_addr)
{
    RibVif<A>* vif = find_vif(vifname);
    if (!vif) {
	XLOG_ERROR("Attempting to add address to non-existant Vif \"%s\"",
		   vifname.c_str());
	return XORP_ERROR;
    }

    vif->add_address(VifAddr(addr, subnet, broadcast_addr, peer_addr));

    if (vif->is_underlying_vif_up())
	add_connected_route(*vif, subnet, addr, peer_addr);

    return XORP_OK;
}

template <typename A>
int
RIB<A>::delete_vif_address(const string& vifname,
			   const A& addr)
{
    RibVif<A>* vif = find_vif(vifname);
    if (!vif) {
	XLOG_ERROR("Attempting to delete address from non-existant Vif \"%s\"",
		   vifname.c_str());
	return XORP_ERROR;
    }

    list<VifAddr>::const_iterator ai;
    for (ai = vif->addr_list().begin(); ai != vif->addr_list().end(); ++ai) {
	const IPvX& ipvx = ai->addr();
	if (ipvx.af() != A::af())
	    continue;
	if (ipvx != IPvX(addr))
	    continue;

	IPNet<A> subnet_addr;
	A peer_addr;
	ai->subnet_addr().get(subnet_addr);
	ai->peer_addr().get(peer_addr);

	vif->delete_address(ipvx);

	if (vif->is_underlying_vif_up())
	    delete_connected_route(*vif, subnet_addr, peer_addr);

	return XORP_OK;
    }
    return XORP_ERROR;
}

template <typename A>
int
RIB<A>::add_route(const string&		tablename,
		  const IPNet<A>&	net,
		  const A&		nexthop_addr,
		  const string&		ifname,
		  const string&		vifname,
		  uint32_t		metric,
		  const PolicyTags&	policytags)
{
    UNUSED(ifname);

    // Sanity check - we should have initialized RIB
    XLOG_ASSERT(_connected_origin_table);

    OriginTable<A>* ot = find_origin_table(tablename);
    if (ot == NULL) {
	if (_errors_are_fatal) {
	    XLOG_FATAL("Attempting to add route to table \"%s\" "
		    "that doesn't exist", tablename.c_str());
	} else {
	    XLOG_ERROR("Attempting to add route to table \"%s\" "
		    "that doesn't exist", tablename.c_str());
	    return XORP_ERROR;
	}
    }
    const Protocol& protocol = ot->protocol();

    RibVif<A>* vif = NULL;
    IPNextHop<A>* nexthop = NULL;

    if (!vifname.empty()) {
	//
	// Add a route with explicitly specified network interface
	//
	RibVif<A>* vif = find_vif(vifname);
	if (!vif) {
	    XLOG_ERROR("Attempting to add route to table \"%s\" "
		       "(prefix %s next-hop %s ifname %s vifname %s): "
		       "no such network interface",
		       tablename.c_str(), net.str().c_str(),
		       nexthop_addr.str().c_str(),
		       ifname.c_str(), vifname.c_str());
	    return XORP_ERROR;
	}

	IPNextHop<A>* nexthop = create_peer_nexthop(nexthop_addr);
	ot->add_route(new IPRouteEntry<A>(net, vif, nexthop, &protocol, metric, policytags));
	flush();
	return XORP_OK;
    }

    //
    // Search for a route to a directly-connected destination
    //
    const IPRouteEntry<A>* re = _connected_origin_table->lookup_ip_route(nexthop_addr);
    if (re != NULL)
	// We found a route for the nexthop
	vif = re->vif();

    if (vif != NULL)
	nexthop = create_peer_nexthop(nexthop_addr);
    else if (vif == NULL && ot->protocol_type() == IGP) {
	debug_msg("**not directly connected route found for nexthop\n");
	//
	// XXX: If the route came from an IGP, then we must have
	// a directly-connected interface toward the next-hop router.
	//
	XLOG_ERROR("Attempting to add IGP route to table \"%s\" "
		"(prefix %s next-hop %s): no directly connected "
		"interface toward the next-hop router", tablename.c_str(), net.str().c_str(), nexthop_addr.str().c_str());
	return XORP_ERROR;
    } else
	nexthop = create_external_nexthop(nexthop_addr);

    XLOG_ASSERT(nexthop->addr() == nexthop_addr);
    //
    // Add the route
    //
    ot->add_route(new IPRouteEntry<A>(net, vif, nexthop, &protocol, metric, policytags));

    flush();
    return XORP_OK;
}

template <typename A>
int
RIB<A>::replace_route(const string&	tablename,
		      const IPNet<A>&	net,
		      const A&		nexthop_addr,
		      const string&	ifname,
		      const string&	vifname,
		      uint32_t		metric,
		      const PolicyTags&	policytags)
{
    UNUSED(ifname);

    OriginTable<A>* ot = find_origin_table(tablename);
    if (NULL == ot)
	return XORP_ERROR; // Table is not an origin table

    int response = ot->delete_route(net, true);
    if (response != XORP_OK)
	return response;

    response = add_route(tablename, net, nexthop_addr, ifname, vifname,
			 metric, policytags);

    // No need to flush here, as add_route will do it for us.

    return response;
}

template <typename A>
int
RIB<A>::delete_route(const string& tablename, const IPNet<A>& net)
{
    OriginTable<A>* ot = find_origin_table(tablename);
    if (NULL == ot)
	return XORP_ERROR; // Table is not an origin table

    int result = ot->delete_route(net);
    flush();
    return result;
}

template <typename A>
void
RIB<A>::flush()
{
    if (_register_table != NULL)
	_register_table->flush();
    if (_final_table != NULL && _final_table != _register_table)
	_final_table->flush();
}

template <typename A>
int
RIB<A>::verify_route(const A& lookup_addr,
		     const string& ifname,
		     const A& nexthop_addr,
		     uint32_t metric,
		     RibVerifyType matchtype)
{
    const IPRouteEntry<A>* re;
    int return_value = (matchtype == RibVerifyType(MISS) ? XORP_OK : XORP_ERROR);

    // 1. Check for an expected route miss.
    // 2. Check table entry validity and existence.
    re = _ext_int_table->lookup_route(lookup_addr);
    if (re == NULL || re->vif() == NULL) {
	if (matchtype == RibVerifyType(MISS)) {
	    debug_msg("****ROUTE MISS SUCCESSFULLY VERIFIED****\n");
	    return XORP_OK;
	}
	if (re == NULL) {
	    debug_msg("RouteVerify: Route Lookup failed\n");
	} else {
	    debug_msg("Route lookup returned NULL vif: %s\n",
		      re->str().c_str());
	}
	return XORP_ERROR;
    }

#ifdef notyet
    // 3a. Check for discard (blackhole) routes.
    // XXX: re->vif() must be non-NULL and valid. Revisit this in future.
    DiscardNextHop<A>* dnh = dynamic_cast<DiscardNextHop<A>*>(re->nexthop());
    if (matchtype == RibVerifyType(DISCARD)) {
	if (dnh == NULL) {
	    debug_msg("Next hop is not a DiscardNextHop");
	    return XORP_ERROR;
	} else {
	    debug_msg("****DISCARD ROUTE SUCCESSFULLY VERIFIED****\n");
	    return XORP_OK;
	}
    }
    // 3b. Check for unreachable routes.
    // XXX: re->vif() must be non-NULL and valid. Revisit this in future.
    UnreachableNextHop<A>* unh = dynamic_cast<UnreachableNextHop<A>*>(re->nexthop());
    if (matchtype == RibVerifyType(UNREACHABLE)) {
	if (unh == NULL) {
	    debug_msg("Next hop is not an UnreachableNextHop");
	    return XORP_ERROR;
	} else {
	    debug_msg("****UNREACHABLE ROUTE SUCCESSFULLY VERIFIED****\n");
	    return XORP_OK;
	}
    }
#endif

    // 4. Check for protocol level routes (specifically IP).
    IPNextHop<A>* route_nexthop = re->nexthop();
    if (route_nexthop == NULL) {
	debug_msg("Next hop is not an IPNextHop\n");
	return XORP_ERROR;
    } else if ((nexthop_addr != route_nexthop->addr())) {
	debug_msg("NextHop: Exp: %s != Got: %s\n",
		  nexthop_addr.str().c_str(),
		  route_nexthop->addr().str().c_str());
	return return_value;
    } else {
	debug_msg("NextHop: Exp: %s != Got: %s\n",
		  nexthop_addr.str().c_str(),
		  route_nexthop->addr().str().c_str());
    }
    if (ifname != re->vif()->name()) {
	XLOG_ERROR("Interface \"%s\" does not match expected \"%s\".",
		   re->vif()->str().c_str(), ifname.c_str());
	return return_value;
    } else {
	debug_msg("Ifname: Exp: %s == Got: %s\n",
		  ifname.c_str(),
		  re->vif()->name().c_str());
    }
    if (metric != re->metric()) {
	XLOG_ERROR("Metric \"%u\" does not match expected \"%u\".",
		   XORP_UINT_CAST(re->metric()), XORP_UINT_CAST(metric));
	return return_value;
    } else {
	debug_msg("Metric: Exp: %u == Got: %u\n",
		  XORP_UINT_CAST(metric),
		  XORP_UINT_CAST(re->metric()));
    }
    if (matchtype == RibVerifyType(MISS)) {
	XLOG_ERROR("****We got valid IP route, but we expected MISS****\n");
	return XORP_ERROR;
    }
    debug_msg("****IP ROUTE SUCCESSFULLY VERIFIED****\n");
    return XORP_OK;
}

template <typename A>
const A&
RIB<A>::lookup_route(const A& lookupaddr)
{
    debug_msg("looking up %s\n", lookupaddr.str().c_str());

    const IPRouteEntry<A>* re = _ext_int_table->lookup_route(lookupaddr);
    // Case 1: Route miss. Return the null IP address.
    // Vif cannot be NULL for a valid route.
    if (re == NULL || re->vif() == NULL)
	return A::ZERO();

#ifdef notyet
    DiscardNextHop<A>* discard_nexthop =
	dynamic_cast<DiscardNextHop<A>* >(re->nexthop());
    // Case 2a: Discard route. Return the loopback address.
    if (discard_nexthop != NULL)
	return A::LOOPBACK();

    UnreachableNextHop<A>* unreachable_nexthop =
	dynamic_cast<UnreachableNextHop<A>* >(re->nexthop());
    // Case 2b: Unreachable route. Return the loopback address.
    if (unreachable_nexthop != NULL)
	return A::LOOPBACK();

    IPNextHop<A>* ip_nexthop = re->nexthop();
    // Case 3: IP protocol route. Return the nexthop address.
    if (ip_nexthop != NULL)
	return ip_nexthop->addr();

    // Default: Return the null IP address.
    return A::ZERO();
#else
    // Default: Assume the route points to a resolved IPNextHop.
    IPNextHop<A>* route_nexthop = re->nexthop();
    return route_nexthop->addr();
#endif
}

template <typename A>
RouteRange<A>*
RIB<A>::route_range_lookup(const A& lookupaddr)
{
    return _ext_int_table->lookup_route_range(lookupaddr);
}

template <typename A>
RouteRegister<A>*
RIB<A>::route_register(const A& lookupaddr, const string& module)
{
    debug_msg("registering %s\n", lookupaddr.str().c_str());
    return _register_table->register_route_range(lookupaddr, module);
}

template <typename A>
int
RIB<A>::route_deregister(const IPNet<A>& subnet, const string& module)
{
    debug_msg("deregistering %s\n", subnet.str().c_str());
    return _register_table->deregister_route_range(subnet, module);
}

template <typename A>
RedistTable<A>*
RIB<A>::protocol_redist_table(const string& protocol)
{
    return  find_redist_table(redist_tablename(protocol));
}

template <typename A>
int
RIB<A>::add_igp_table(const string& tablename,
		      const string& target_class,
		      const string& target_instance)
{
    debug_msg("add_igp_table %s\n", tablename.c_str());
    int r = add_origin_table<IGP>(tablename, target_class, target_instance);
    if (r != XORP_OK)
	return r;

    // XXX For now we unconditionally plumb a RedistTable behind each
    // OriginTable.  We do this because the RedistTable needs to track
    // the routes within the OriginTable in order to be able to render
    // a dump when another protocol requests redistribution.
    r = add_redist_table(find_igp_origin_table(tablename));
    if (r != XORP_OK) {
	delete_origin_table(tablename, target_class, target_instance);
	return r;
    }

    if (tablename == "connected") {
	RedistTable<A>* rt = find_redist_table(redist_tablename(tablename));
	XLOG_ASSERT(rt != NULL);
	r = add_policy_connected_table(rt);
	if (r != XORP_OK) {
	    delete_origin_table(tablename, target_class, target_instance);
	    //
	    // XXX: we don't delete the redist table but keep it around,
	    // because delete_origin_table() does similar.
	    //
	    return r;
	}
    }
    return r;
}

template <typename A>
int
RIB<A>::add_egp_table(const string& tablename,
		      const string& target_class,
		      const string& target_instance)
{
    debug_msg("add_egp_table %s\n", tablename.c_str());
    int r = add_origin_table<EGP>(tablename, target_class, target_instance);
    if (r != XORP_OK) {
	return r;
    }

    return r;
}

template <typename A>
int
RIB<A>::add_policy_connected_table(RouteTable<A>* parent)
{
    if (parent == NULL) {
	XLOG_WARNING("add_policy_connected_table: parent table %s does not exist",
		parent->tablename().c_str());
	return XORP_ERROR;
    }

    if (_policy_connected_table != NULL)
	return XORP_OK;

    PolicyConnectedTable<A>* pt =
	new PolicyConnectedTable<A>(parent, _rib_manager.policy_filters());

    _policy_connected_table = pt;
    return XORP_OK;
}

template <typename A>
int
RIB<A>::add_redist_table(RouteTable<A>* parent)
{
    if (parent == NULL) {
	XLOG_WARNING("add_redist_table: parent table %s does not exist",
		parent->tablename().c_str());
	return XORP_ERROR;
    }

    if (find_redist_table(redist_tablename(parent->tablename())) != NULL) {
	return XORP_OK;		// RedistTable already exists, no sweat
    }

    RedistTable<A>* r = new RedistTable<A>(redist_tablename(parent->tablename()), parent);
    if (add_table(r) != XORP_OK) {
	delete r;
	return XORP_ERROR;
    }
    return XORP_OK;
}

template <typename A>
int
RIB<A>::plumb_origin_table(OriginTable<A>*& ot)
{
    // XXX: the table was created by new_origin_table() above, so it must exist
    XLOG_ASSERT(ot != NULL && _final_table != ot);

    return _ext_int_table->add_protocol_table(ot);
}

//
// All the magic is in add_origin_table.
//

template <typename A>
template <ProtocolType protocol_type>
int
RIB<A>::add_origin_table(const string& tablename,
			 const string& target_class,
			 const string& target_instance)
{
    debug_msg("add_origin_table %s type: %d\n",
	      tablename.c_str(), protocol_type);

    // Check if table exists and check type if so
    OriginTable<A>* ot = NULL;
    ot = find_origin_table_smart<protocol_type>(tablename);

    if (ot != NULL) {
	ot->protocol().increment_genid();
	//
	// Table already exists, hence use it.
	//

	//
	// Store the XRL target instance, so we know which OriginTable to
	// shutdown if the routing protocol dies.
	//
	if (!target_instance.empty()) {
	    _rib_manager.register_interest_in_target(target_class);
	    _routing_protocol_instances[tablename + " " + target_class + " "
		    + target_instance] = ot;
	}
	return XORP_OK;
    }

    if (new_origin_table<protocol_type>(tablename, target_class, target_instance,
	    get_protocol_admin_distance(tablename)) != XORP_OK) {
	debug_msg("new_origin_table failed\n");
	return XORP_ERROR;
    }

    ot = find_origin_table_smart<protocol_type>(tablename);

    return plumb_origin_table(ot);
}

template <typename A>
int
RIB<A>::delete_igp_table(const string& tablename,
			 const string& target_class,
			 const string& target_instance)
{
    return delete_origin_table(tablename, target_class, target_instance);
}

template <typename A>
int
RIB<A>::delete_egp_table(const string& tablename,
			 const string& target_class,
			 const string& target_instance)
{
    return delete_origin_table(tablename, target_class, target_instance);
}

template <class A>
int
RIB<A>::delete_origin_table(const string& tablename,
			    const string& target_class,
			    const string& target_instance)
{
    OriginTable<A>* ot = find_origin_table(tablename);
    if (NULL == ot)
	return XORP_ERROR;

    if (!target_instance.empty()) {
	if (find_table_by_instance(tablename, target_class, target_instance)
	    != ot) {
	    XLOG_ERROR("Got delete_origin_table for wrong target name\n");
	    return XORP_ERROR;
	} else {
	    _routing_protocol_instances.erase(tablename + " "
					      + target_class + " "
					      + target_instance);
	}
    }

    // Remove all the routes this table used to originate, but keep table
    ot->routing_protocol_shutdown();
    return XORP_OK;
}

template <class A>
void
RIB<A>::target_death(const string& target_class,
		     const string& target_instance)
{
    string s = " " + target_class + " " + target_instance;
    typename map<string, OriginTable<A>* >::iterator iter;
    for (iter = _routing_protocol_instances.begin();
	 iter != _routing_protocol_instances.end();
	 ++iter) {
	if (iter->first.find(s) != string::npos) {
	    // We've found the target.
	    XLOG_INFO("Received death event for protocol %s shutting down %s",
		      target_class.c_str(), iter->second->str().c_str());
	    iter->second->routing_protocol_shutdown();
	    _routing_protocol_instances.erase(iter);

	    // No need to go any further.
	    return;
	}
    }
}

#ifdef DEBUG_LOGGING
template <class A>
struct print_from_pair {
    print_from_pair(string s = "###"): _separator(s) {}
    void operator() (const pair<const string, A*>& table_pair) const {
	cout << table_pair.second->str() << _separator << endl;
    }
protected:
    string _separator;
};
#endif

template <typename A>
void
RIB<A>::print_rib() const
{
#ifdef DEBUG_LOGGING
    cout << "**************************************************************" << endl;
    cout << "Redist table count " << _redist_tables.size() << endl;
    for_each(_redist_tables.begin(), _redist_tables.end(),
	print_from_pair<RouteTable<A> >());

    cout << "==============================================================" << endl;
    cout << "IGP table count " << _igp_origin_tables.size() << endl;
    for_each(_igp_origin_tables.begin(), _igp_origin_tables.end(),
	print_from_pair<RouteTable<A> >());

    cout << "==============================================================" << endl;
    cout << "EGP table count " << _egp_origin_tables.size() << endl;
    for_each(_egp_origin_tables.begin(), _egp_origin_tables.end(),
	print_from_pair<RouteTable<A> >());

    cout << "==============================================================" << endl;
    if (_policy_connected_table)
	cout << _policy_connected_table->str() << "###" << endl;

    cout << "==============================================================" << endl;
    if (_ext_int_table)
	cout << _ext_int_table->str() << "###" << endl;

    cout << "==============================================================" << endl;
    if (_policy_redist_table)
	cout << _policy_redist_table->str() << "###" << endl;

    cout << "==============================================================" << endl;
    if (_register_table)
	cout << _register_table->str() << "###" << endl;

    cout << "==============================================================" << endl;

#endif // DEBUG_LOGGING
}

template <typename A>
string
RIB<A>::name() const
{
    return c_format("%s %s RIB",
		    (_multicast) ? "Multicast" : "Unicast",
		    A::ip_version_str().c_str());
}


template <typename A>
void
RIB<A>::push_routes()
{
    XLOG_ASSERT(_policy_connected_table != NULL);

    _policy_connected_table->push_routes();
}

template <typename A>
RibVif<A>*
RIB<A>::find_vif(const string& vifname)
{
    debug_msg("RIB::find_vif: %s\n", vifname.c_str());
    typename map<string, RibVif<A>*>::iterator vi = _vifs.find(vifname);
    if (vi == _vifs.end()) {
	return NULL;
    }
    return vi->second;
}

template class RIB<IPv4>;

#ifdef HAVE_IPV6

template class RIB<IPv6>;

#endif //ipv6
