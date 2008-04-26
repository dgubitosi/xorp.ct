// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2008 International Computer Science Institute
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

// $XORP$

#ifndef __FEA_FIREWALL_SET_HH__
#define __FEA_FIREWALL_SET_HH__

#include "firewall_entry.hh"
#include "iftree.hh"
#include "fea_data_plane_manager.hh"

class FirewallManager;


class FirewallSet {
public:
    /**
     * Constructor.
     *
     * @param fea_data_plane_manager the corresponding data plane manager
     * (@ref FeaDataPlaneManager).
     */
    FirewallSet(FeaDataPlaneManager& fea_data_plane_manager)
	: _is_running(false),
	  _firewall_manager(fea_data_plane_manager.firewall_manager()),
	  _fea_data_plane_manager(fea_data_plane_manager),
	  _in_configuration(false)
    {}

    /**
     * Virtual destructor.
     */
    virtual ~FirewallSet() {}

    /**
     * Get the @ref FirewallManager instance.
     *
     * @return the @ref FirewallManager instance.
     */
    FirewallManager&	firewall_manager() { return _firewall_manager; }

    /**
     * Get the @ref FeaDataPlaneManager instance.
     *
     * @return the @ref FeaDataPlaneManager instance.
     */
    FeaDataPlaneManager& fea_data_plane_manager() { return _fea_data_plane_manager; }

    /**
     * Test whether this instance is running.
     *
     * @return true if the instance is running, otherwise false.
     */
    virtual bool is_running() const { return _is_running; }

    /**
     * Start operation.
     * 
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int start(string& error_msg) = 0;
    
    /**
     * Stop operation.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int stop(string& error_msg) = 0;
    
    /**
     * Start a configuration interval.
     *
     * All modifications must be within a marked "configuration" interval.
     *
     * This method provides derived classes with a mechanism to perform
     * any actions necessary before forwarding table modifications can
     * be made.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int start_configuration(string& error_msg) {
	// Nothing particular to do, just label start.
	return mark_configuration_start(error_msg);
    }

    /**
     * End of configuration interval.
     *
     * This method provides derived classes with a mechanism to
     * perform any actions necessary at the end of a configuration, eg
     * write a file.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int end_configuration(string& error_msg) {
	// Nothing particular to do, just label start.
	return mark_configuration_end(error_msg);
    }
    
    /**
     * Add a single firewall entry.
     *
     * Must be within a configuration interval.
     *
     * @param firewall_entry the entry to add.
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int add_entry(const FirewallEntry& firewall_entry,
			  string& error_msg) = 0;

    /**
     * Delete a single firewall entry.
     *
     * Must be with a configuration interval.
     *
     * @param firewall_entry the entry to delete.
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int delete_entry(const FirewallEntry& firewall_entry,
			     string& error_msg) = 0;

    /**
     * Set the IPv4 firewall table.
     *
     * @param firewall_entry_list the list with all entries to install into
     * the IPv4 firewall table.
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int set_table4(const list<FirewallEntry>& firewall_entry_list,
			   string& error_msg) = 0;

    /**
     * Delete all entries in the IPv4 firewall table.
     *
     * Must be within a configuration interval.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int delete_all_entries4(string& error_msg) = 0;

    /**
     * Set the IPv6 firewall table.
     *
     * @param firewall_entry_list the list with all entries to install into
     * the IPv6 firewall table.
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int set_table6(const list<FirewallEntry>& firewall_entry_list,
			   string& error_msg) = 0;

    /**
     * Delete all entries in the IPv6 firewall table.
     *
     * Must be within a configuration interval.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    virtual int delete_all_entries6(string& error_msg) = 0;

protected:
    /**
     * Mark start of a configuration.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    int mark_configuration_start(string& error_msg) {
	if (_in_configuration != true) {
	    _in_configuration = true;
	    return (XORP_OK);
	}
	error_msg = c_format("Cannot start configuration: "
			     "configuration in progress");
	return (XORP_ERROR);
    }

    /**
     * Mark end of a configuration.
     *
     * @param error_msg the error message (if error).
     * @return XORP_OK on success, otherwise XORP_ERROR.
     */
    int mark_configuration_end(string& error_msg) {
	if (_in_configuration != false) {
	    _in_configuration = false;
	    return (XORP_OK);
	}
	error_msg = c_format("Cannot end configuration: "
			     "configuration not in progress");
	return (XORP_ERROR);
    }
    
    bool in_configuration() const { return _in_configuration; }

    // Misc other state
    bool	_is_running;

private:
    FirewallManager&	_firewall_manager;
    FeaDataPlaneManager& _fea_data_plane_manager;
    bool		_in_configuration;
};

#endif // __FEA_FIREWALL_SET_HH__