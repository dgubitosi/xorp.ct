// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-
// vim:set sts=4 ts=8:

// Copyright (c) 2001-2004 International Computer Science Institute
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

// $XORP: xorp/ospf/ospf.hh,v 1.73 2005/11/13 06:55:10 atanu Exp $

#ifndef __OSPF_OSPF_HH__
#define __OSPF_OSPF_HH__

/**
 * OSPF Types
 */
struct OspfTypes {

    /**
     * The OSPF version.
     */
    enum Version {V2 = 2, V3 = 3};
    
    /**
     * The type of an OSPF packet.
     */
    typedef uint16_t Type;

    /**
     * Router ID.
     */
    typedef uint32_t RouterID;

    /**
     * Area ID.
     */
    typedef uint32_t AreaID;

    /**
     * Link Type
     */
    enum LinkType {
	PointToPoint,
	BROADCAST,
	NBMA,
	PointToMultiPoint,
	VirtualLink
    };

    /**
     * Authentication type: OSPFv2 standard header.
     */
    typedef uint16_t AuType;
    static const AuType NULL_AUTHENTICATION = 0;
    static const AuType SIMPLE_PASSWORD = 1;
    static const AuType CRYPTOGRAPHIC_AUTHENTICATION = 2;

    /**
     * Area Type
     */
    enum AreaType {
	NORMAL,		// Normal Area
	STUB,		// Stub Area
	NSSA,		// Not-So-Stubby Area
    };

    /**
     * Routing Entry Type.
     */
    enum VertexType {
	Router,
	Network
    };

    /**
     * NSSA Translator Role.
     */
    enum NSSATranslatorRole {
	ALWAYS,
	CANDIDATE
    };

    /**
     * NSSA Translator State.
     */
    enum NSSATranslatorState {
	ENABLED,
	ELECTED,
	DISABLED,
    };

    /**
     * The AreaID for the backbone area.
     */
    static const AreaID BACKBONE = 0;

    /**
     * An opaque handle that identifies a neighbour.
     */
    typedef uint32_t NeighbourID;

    /**
     * The IP protocol number used by OSPF.
     */
    static const uint16_t IP_PROTOCOL_NUMBER = 89;

    /** 
     * An identifier meaning all neighbours. No single neighbour can
     * have this identifier.
     */
    static const NeighbourID ALLNEIGHBOURS = 0;

    /**
     *
     * The maximum time between distinct originations of any particular
     * LSA.  If the LS age field of one of the router's self-originated
     * LSAs reaches the value LSRefreshTime, a new instance of the LSA
     * is originated, even though the contents of the LSA (apart from
     * the LSA header) will be the same.  The value of LSRefreshTime is
     * set to 30 minutes.
     */
    static const uint32_t LSRefreshTime = 30 * 60;

    /**
     * The minimum time between distinct originations of any particular
     * LSA.  The value of MinLSInterval is set to 5 seconds.
     */
    static const uint32_t MinLSInterval = 5;

    /**
     * For any particular LSA, the minimum time that must elapse
     * between reception of new LSA instances during flooding. LSA
     * instances received at higher frequencies are discarded. The
     * value of MinLSArrival is set to 1 second.
     */
    static const uint32_t MinLSArrival = 1;

    /**
     * The maximum age that an LSA can attain. When an LSA's LS age
     * field reaches MaxAge, it is reflooded in an attempt to flush the
     * LSA from the routing domain. LSAs of age MaxAge
     * are not used in the routing table calculation.	The value of
     * MaxAge is set to 1 hour.
     */
    static const uint32_t MaxAge = 60 * 60;

    /**
     * When the age of an LSA in the link state database hits a
     * multiple of CheckAge, the LSA's checksum is verified.  An
     * incorrect checksum at this time indicates a serious error.  The
     * value of CheckAge is set to 5 minutes.
     */
    static const uint32_t CheckAge = 5 * 60;

    /*
     * The maximum time dispersion that can occur, as an LSA is flooded
     * throughout the AS.  Most of this time is accounted for by the
     * LSAs sitting on router output queues (and therefore not aging)
     * during the flooding process.  The value of MaxAgeDiff is set to
     * 15 minutes.
     */
    static const int32_t MaxAgeDiff = 15 * 60;

    /*
     * The metric value indicating that the destination described by an
     * LSA is unreachable. Used in summary-LSAs and AS-external-LSAs as
     * an alternative to premature aging. It is
     * defined to be the 24-bit binary value of all ones: 0xffffff.
     */
    static const uint32_t LSInfinity = 0xffffff;

    /*
     * The value used for LS Sequence Number when originating the first
     * instance of any LSA.
     */
    static const int32_t InitialSequenceNumber = 0x80000001;

    /*
     * The maximum value that LS Sequence Number can attain.
     */
    static const int32_t MaxSequenceNumber = 0x7fffffff;
};

/**
 * XRL target name.
 */
static const char TARGET_OSPFv2[] = "ospfv2";
static const char TARGET_OSPFv3[] = "ospfv3";

/**
 * Get the XRL target name.
 */
inline
const char *
xrl_target(OspfTypes::Version version)
{
    switch (version) {
    case OspfTypes::V2:
	return TARGET_OSPFv2;
	break;
    case OspfTypes::V3:
	return TARGET_OSPFv3;
	break;
    }

    XLOG_UNREACHABLE();
}

/**
 * Pretty print a router or area ID.
 */
inline
string
pr_id(uint32_t id)
{
    return IPv4(htonl(id)).str();
}

/**
 * Set a router or area ID using dot notation: "128.16.64.16".
 */
inline
uint32_t
set_id(const char *addr)
{
    return ntohl(IPv4(addr).addr());
}

/**
 * Pretty print the link type.
 */
inline
string
pp_link_type(OspfTypes::LinkType link_type)
{
    switch(link_type) {
    case OspfTypes::PointToPoint:
	return "PointToPoint";
    case OspfTypes::BROADCAST:
	return "BROADCAST";
    case OspfTypes::NBMA:
	return "NBMA";
    case OspfTypes::PointToMultiPoint:
	return "PointToMultiPoint";
    case OspfTypes::VirtualLink:
	return "VirtualLink";
    }
    XLOG_UNREACHABLE();
}

/**
 * Convert from a string to the type of area
 */
inline
OspfTypes::LinkType
from_string_to_link_type(const string& type, bool& status)
{
    status = true;
    if (type == "p2p")
	return OspfTypes::PointToPoint;
    else if (type == "broadcast")
	return OspfTypes::BROADCAST;
    else if (type == "nbma")
	return OspfTypes::NBMA;
    else if (type == "p2m")
	return OspfTypes::PointToMultiPoint;
    else if (type == "vlink")
	return OspfTypes::VirtualLink;

    XLOG_WARNING("Unable to match %s", type.c_str());
    status = false;

    return OspfTypes::BROADCAST;
}

/**
 * Pretty print the area type.
 */
inline
string
pp_area_type(OspfTypes::AreaType area_type)
{
    switch(area_type) {
    case OspfTypes::NORMAL:
	return "NORMAL";
    case OspfTypes::STUB:
	return "STUB";
    case OspfTypes::NSSA:
	return "NSSA";
    }
    XLOG_UNREACHABLE();
}

/**
 * Convert from a string to the type of area
 */
inline
OspfTypes::AreaType
from_string_to_area_type(const string& type, bool& status)
{
    status = true;
    if (type == "normal")
	return OspfTypes::NORMAL;
    else if (type == "stub")
	return OspfTypes::STUB;
    else if (type == "nssa")
	return OspfTypes::NSSA;

    XLOG_WARNING("Unable to match %s", type.c_str());
    status = false;

    return OspfTypes::NORMAL;
}

inline
const char *
pb(bool val)
{
    return val ? "true" : "false";
}

/**
 * Neighbour information that is returned by XRLs.
 */
struct NeighbourInfo {
    string _address;		// Address of neighbour.
    string _interface;		// Interface name.
    string _state;		// The current state.
    IPv4 _rid;			// The neighbours router id.
    uint32_t _priority;		// The priority in the hello packet.
    uint32_t _deadtime;		// Number of seconds before the
				// peering is considered down.
    IPv4 _area;			// The area this neighbour belongs to.
    uint32_t _opt;		// The options on the hello packet.
    IPv4 _dr;			// The designated router.
    IPv4 _bdr;			// The backup designated router.
    uint32_t _up;		// Time there has been neighbour awareness.
    uint32_t _adjacent;		// Time peering has been adjacent.
};

#include "policy_varrw.hh"
#include "io.hh"
#include "exceptions.hh"
#include "lsa.hh"
#include "packet.hh"
#include "transmit.hh"
#include "peer_manager.hh"
#include "external.hh"
#include "vlink.hh"
#include "routing_table.hh"
#include "trace.hh"

template <typename A>
class Ospf {
 public:
    Ospf(OspfTypes::Version version, EventLoop& eventloop, IO<A>* io);
	
    /**
     * @return version of OSPF this implementation represents.
     */
    OspfTypes::Version version() { return _version; }

    /**
     * @return true if ospf should still be running.
     */
    bool running() { return _io->running(); }

    /**
     * XXX - Temporary hack to get us up an running.
     */
    ProcessStatus status(string& reason) {
	reason = _reason;
	return _process_status;
    }

    /**
     * Shutdown OSPF.
     */
    void shutdown() {
	_io->shutdown();
	_reason = "shutting down";
	_process_status = PROC_SHUTDOWN;
    }

    /**
     * Used to send traffic on the IO interface.
     */
    bool transmit(const string& interface, const string& vif,
		  A dst, A src, uint8_t* data, uint32_t len);
    
    /**
     * The callback method that is called when data arrives on the IO
     * interface.
     */
    void receive(const string& interface, const string& vif,
		 A dst, A src, uint8_t* data, uint32_t len);

    /**
     * Enable the interface/vif to receive frames.
     */
    bool enable_interface_vif(const string& interface, const string& vif);

    /**
     * Disable this interface/vif from receiving frames.
     */
    bool disable_interface_vif(const string& interface, const string& vif);

    /**
     * Is this interface/vif/address enabled? 
     * This is a question asked of the FEA, has the interface/vif been
     * marked as up.
     *
     * @return true if it is.
     */
    bool enabled(const string& interface, const string& vif, A address);

    /**
     * @return prefix length for this address.
     */
    uint32_t get_prefix_length(const string& interface, const string& vif,
			       A address);

    /**
     * @return the mtu for this interface.
     */
    uint32_t get_mtu(const string& interface);

    /**
     * On the interface/vif join this multicast group.
     */
    bool join_multicast_group(const string& interface, const string& vif,
			      A mcast);
    
    /**
     * On the interface/vif leave this multicast group.
     */
    bool leave_multicast_group(const string& interface, const string& vif,
			       A mcast);

    /**
     * Set the interface ID OSPFv3 only.
     */
    bool set_interface_id(const string& interface, const string& vif,
			  OspfTypes::AreaID area,
			  uint32_t interface_id);

    /**
     * Set the hello interval in seconds.
     */
    bool set_hello_interval(const string& interface, const string& vif,
			    OspfTypes::AreaID area,
			    uint16_t hello_interval);

#if	0
    /**
     * Set options.
     */
    bool set_options(const string& interface, const string& vif,
		     OspfTypes::AreaID area,
		     uint32_t options);
#endif

    /**
     * Create a virtual link
     *
     * @param rid neighbours router ID.
     */
    bool create_virtual_link(OspfTypes::RouterID rid);

    /**
     * Delete a virtual link
     *
     * @param rid neighbours router ID.
     */
    bool delete_virtual_link(OspfTypes::RouterID rid);

    /**
     * Attach this transit area to the neighbours router ID.
     */
    bool transit_area_virtual_link(OspfTypes::RouterID rid,
				   OspfTypes::AreaID transit_area);
    

    /**
     * Set router priority.
     */
    bool set_router_priority(const string& interface, const string& vif,
			     OspfTypes::AreaID area,
			     uint8_t priority);

    /**
     * Set the router dead interval in seconds.
     */
    bool set_router_dead_interval(const string& interface, const string& vif,
				  OspfTypes::AreaID area,
				  uint32_t router_dead_interval);

    /**
     * Set the interface cost.
     */
    bool set_interface_cost(const string& interface, const string& vif,
			    OspfTypes::AreaID area,
			    uint16_t interaface_cost);

    /**
     * Set InfTransDelay
     */
    bool set_inftransdelay(const string& interface, const string& vif,
			   OspfTypes::AreaID area,
			   uint16_t inftransdelay);

    /**
     *  Configure authentication.
     */
    bool set_authentication(const string& ifname, const string& vif,
			    OspfTypes::AreaID area,
			    string type, string password);

    /**
     * Send router alerts in IP packets or not.
     */
    bool set_ip_router_alert(bool alert);

    /**
     * Add area range.
     */
    bool area_range_add(OspfTypes::AreaID area, IPNet<A> net, bool advertise);

    /**
     * Delete area range.
     */
    bool area_range_delete(OspfTypes::AreaID area, IPNet<A> net);

    /**
     * Change the advertised state of this area.
     */
    bool area_range_change_state(OspfTypes::AreaID area, IPNet<A> net,
				 bool advertise);

    /**
     *  Get a single lsa from an area. A stateless mechanism to get LSAs. The
     *  client of this interface should start from zero and continue to request
     *  LSAs (incrementing index) until toohigh becomes true.
     *
     *  @param area database that is being searched.
     *  @param index into database starting from 0.
     *  @param valid true if a LSA has been returned. Some index values do not
     *  contain LSAs. This should not be considered an error.
     *  @param toohigh true if no more LSA exist after this index.
     *  @param self if true this LSA was originated by this router.
     *  @param lsa if valid is true the LSA at index.
     */
    bool get_lsa(const OspfTypes::AreaID area, const uint32_t index,
		 bool& valid, bool& toohigh, bool& self, vector<uint8_t>& lsa);

    /**
     *  Get a list of all the configured areas.
     */
    bool get_area_list(list<OspfTypes::AreaID>& areas) const;

    /**
     *  Get a list of all the neighbours.
     */
    bool get_neighbour_list(list<OspfTypes::NeighbourID>& neighbours) const;

    /**
     * Get state information about this neighbour.
     *
     * @param nid neighbour information is being request about.
     * @param ninfo if neighbour is found its information.
     *
     */
    bool get_neighbour_info(OspfTypes::NeighbourID nid,
			    NeighbourInfo& ninfo) const;

    /**
     * Add route
     *
     * @param net network
     * @param nexthop
     * @param metric to network
     * @param equal true if this in another route to the same destination.
     * @param discard true if this is a discard route.
     * @param policytags policy info to the RIB.
     */
    bool add_route(IPNet<A> net, A nexthop, uint32_t metric, bool equal,
		   bool discard, const PolicyTags& policytags);
    /**
     * Replace route
     *
     * @param net network
     * @param nexthop
     * @param metric to network
     * @param equal true if this in another route to the same destination.
     * @param discard true if this is a discard route.
     * @param policytags policy info to the RIB.
     */
    bool replace_route(IPNet<A> net, A nexthop, uint32_t metric, bool equal,
		       bool discard, const PolicyTags& policytags);

    /**
     * Delete route
     */
    bool delete_route(IPNet<A> net);

    /**
     * Configure a policy filter
     *
     * @param filter Id of filter to configure.
     * @param conf Configuration of filter.
     */
    void configure_filter(const uint32_t& filter, const string& conf);

    /**
     * Reset a policy filter.
     *
     * @param filter Id of filter to reset.
     */
    void reset_filter(const uint32_t& filter);

    /**
     * Push routes through policy filters for re-filtering.
     */
    void push_routes();

    /**
     * Originate a route.
     *
     * @param net to announce
     * @param nexthop to forward to
     * @param metric 
     * @param policytags policy-tags associated with route.
     *
     * @return true on success
     */
    bool originate_route(const IPNet<A>& net, const A& nexthop,
			 const uint32_t& metric,
			 const PolicyTags& policytags);

    /**
     * Withdraw a route.
     *
     * @param net to withdraw
     *
     * @return true on success
     */
    bool withdraw_route(const IPNet<A>&	net);

    /**
     * Get the current OSPF version.
     */
    const OspfTypes::Version get_version() const { return _version; }

    /**
     * @return a reference to the eventloop, required for timers etc...
     */
    EventLoop& get_eventloop() { return _eventloop; }

    /**
     * @return a reference to the PeerManager.
     */
    PeerManager<A>& get_peer_manager() { return _peer_manager; }

    /**
     * @return a reference to the RoutingTable.
     */
    RoutingTable<A>& get_routing_table() { return _routing_table; }

    /**
     * @return a reference to the LSA decoder.
     */
    LsaDecoder& get_lsa_decoder() { return _lsa_decoder; }

    /**
     * @return a reference to the policy filters
     */
    PolicyFilters& get_policy_filters() { return _policy_filters; }

    /**
     * Get the Router ID.
     */
    OspfTypes::RouterID get_router_id() const { return _router_id; }

    /**
     * Set the Router ID.
     */
    void set_router_id(OspfTypes::RouterID id) { _router_id = id; }


    Trace& trace() { return _trace; }

 private:
    const OspfTypes::Version _version;	// OSPF version.
    EventLoop& _eventloop;

    IO<A>* _io;			// Indirection for sending and
				// receiving packets, as well as
				// adding and deleting routes. 
    string _reason;
    ProcessStatus _process_status;

    PacketDecoder _packet_decoder;	// Packet decoders.
    LsaDecoder _lsa_decoder;		// LSA decoders.
    PeerManager<A> _peer_manager;
    RoutingTable<A> _routing_table;
    PolicyFilters _policy_filters;	// The policy filters.

    OspfTypes::RouterID _router_id;	// Router ID.

    Trace _trace;		// Trace variables.
};

/**
 * A class that should be inherited by all major subsytems that can be
 * used to cleanly shutdown OSPF.
 *
 * XXX 
 * At the moment this class is just a placeholder but in the future
 * the constructor can be implemented to add all classes to a static
 * list of objects that can be systematically shutdown.   
 */
class Subsystem {
 public:
    virtual ~Subsystem()
    {}

    /**
     * To be implemented by the subsystem.
     * Called to start the subsystem.
     */
    virtual void start() = 0;

    /**
     * To be implemented by the subsystem.
     * Called to shutdown the subsystem.
     */
    virtual void shutdown() = 0;

    /**
     * To be implemented by the subsystem.
     * @return true if the subsystem is running.
     */
    virtual bool running() = 0;

    /**
     * To be implemented by the subsystem.
     * @return the name of the subsystem.
     */
    virtual string str() = 0;

    /**
     * To be called by the subsystem when the shutdown is complete.
     */
    void shutdown_complete() {
	XLOG_INFO("Shutdown complete %s", str().c_str());
    }
};

// The original design did not leave MaxAge LSAs in the database. When
// an LSA reached MaxAge it was removed from the database and existed
// only in retransmission lists. If an LSA was received which seemed
// to be from a previous incarnation of OSPF it had its age set to
// MaxAge and was fired out, also not being added to the database.
// If while a MaxAge LSA is on the retransmission only, either a new
// LSA such as a Network-LSA is generated or an updated LSA arrives a
// second LSA can be created with the same <Type,ID,ADV> tuple. Two LSAs
// can exist on the retransmission list. Leaving the a MaxAge LSA in
// the database solves both problems.

// #define MAX_AGE_IN_DATABASE

#define PARANOIA

#endif // __OSPF_OSPF_HH__
