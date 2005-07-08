%{
#define YYSTYPE char*

#include <assert.h>
#include <stdio.h>
#include <string>

#include "rtrmgr_module.h"
#include "libxorp/xorp.h"

#include "conf_tree.hh"
#include "conf_tree_node.hh"
#include "template_tree.hh"
#include "template_tree_node.hh"
#include "config_operators.hh"

/* XXX: sigh - -p flag to yacc should do this for us */
#define yystacksize bootstacksize
#define yysslim bootsslim
%}

%token UPLEVEL
%token DOWNLEVEL
%token END
%left ASSIGN_OPERATOR
%token BOOL_VALUE
%token UINT_VALUE
%token IPV4_VALUE
%token IPV4NET_VALUE
%token IPV6_VALUE
%token IPV6NET_VALUE
%token MACADDR_VALUE
%token URL_FILE_VALUE
%token URL_FTP_VALUE
%token URL_HTTP_VALUE
%token URL_TFTP_VALUE
%token LITERAL
%token STRING
%token ARITH
%token COMPARATOR
%token MODIFIER
%token SYNTAX_ERROR
%token LINENUM

%%

input:		/* empty */
		| definition input
		| emptystatement input
		| syntax_error
		;

definition:	long_nodename nodegroup
		| short_nodename long_nodegroup
		;

short_nodename:	literal { push_path(); }
		;

long_nodename:	literals { push_path(); }
		;

literal:	LITERAL { nodenum = 0;
                          extend_path($1, NODE_VOID, nodenum); }
		| LINENUM LITERAL { nodenum = strtoll($1, (char **)NULL, 10);
		                    free($1);
				    extend_path($2, NODE_VOID, nodenum); }
		;

literals:	literals literal
		| literal LITERAL { extend_path($2, NODE_TEXT, nodenum); }
		| literal BOOL_VALUE { extend_path($2, NODE_BOOL, nodenum); }
		| literal UINT_VALUE { extend_path($2, NODE_UINT, nodenum); }
		| literal IPV4_VALUE { extend_path($2, NODE_IPV4, nodenum); }
		| literal IPV4NET_VALUE { extend_path($2, NODE_IPV4NET, nodenum); }
		| literal IPV6_VALUE { extend_path($2, NODE_IPV6, nodenum); }
		| literal IPV6NET_VALUE { extend_path($2, NODE_IPV6NET, nodenum); }
		| literal MACADDR_VALUE { extend_path($2, NODE_MACADDR, nodenum); }
		| literal URL_FILE_VALUE { extend_path($2, NODE_URL_FILE, nodenum); }
		| literal URL_FTP_VALUE { extend_path($2, NODE_URL_FTP, nodenum); }
		| literal URL_HTTP_VALUE { extend_path($2, NODE_URL_HTTP, nodenum); }
		| literal URL_TFTP_VALUE { extend_path($2, NODE_URL_TFTP, nodenum); }
		;

nodegroup:	long_nodegroup
		| END { pop_path(); }
		;

long_nodegroup:	UPLEVEL statements DOWNLEVEL { pop_path(); }
		;

statements:	/* empty string */
		| statement statements
		;

statement:	terminal
		| definition
		| emptystatement
		;

emptystatement:	END
		;

term_literal:	LITERAL { nodenum = 0;
			  extend_path($1, NODE_VOID, nodenum); }
		| LINENUM LITERAL { nodenum = strtoll($1, (char **)NULL, 10);
		                    free($1);
				    extend_path($2, NODE_VOID, nodenum);}
		;

terminal:	term_literal END {
			terminal(strdup(""), NODE_VOID, OP_NONE);
		}
		| term_literal ASSIGN_OPERATOR STRING END {
			terminal($3, NODE_TEXT, OP_ASSIGN); 
		}
		| term_literal ASSIGN_OPERATOR BOOL_VALUE END {
			terminal($3, NODE_BOOL, OP_ASSIGN);
		}
		| term_literal ASSIGN_OPERATOR UINT_VALUE END {
			terminal($3, NODE_UINT, OP_ASSIGN);
		}
		| term_literal ASSIGN_OPERATOR IPV4_VALUE END {
			terminal($3, NODE_IPV4, OP_ASSIGN);
		}
		| term_literal ASSIGN_OPERATOR IPV4NET_VALUE END {
			terminal($3, NODE_IPV4NET, OP_ASSIGN);
		}
		| term_literal ASSIGN_OPERATOR IPV6_VALUE END {
			terminal($3, NODE_IPV6, OP_ASSIGN);
		}
		| term_literal ASSIGN_OPERATOR IPV6NET_VALUE END {
			terminal($3, NODE_IPV6NET, OP_ASSIGN);
		}
		| term_literal ASSIGN_OPERATOR MACADDR_VALUE END {
			terminal($3, NODE_MACADDR, OP_ASSIGN);
		}
		| term_literal ASSIGN_OPERATOR URL_FILE_VALUE END {
			terminal($3, NODE_URL_FILE, OP_ASSIGN);
		}
		| term_literal ASSIGN_OPERATOR URL_FTP_VALUE END {
			terminal($3, NODE_URL_FTP, OP_ASSIGN);
		}
		| term_literal ASSIGN_OPERATOR URL_HTTP_VALUE END {
			terminal($3, NODE_URL_HTTP, OP_ASSIGN);
		}
		| term_literal ASSIGN_OPERATOR URL_TFTP_VALUE END {
			terminal($3, NODE_URL_TFTP, OP_ASSIGN);
		}
		| term_literal COMPARATOR ARITH END {
			terminal($3, NODE_ARITH, boot_lookup_comparator($2));
			free($2);
		}
		| term_literal COMPARATOR UINT_VALUE END{
			terminal($3, NODE_UINT, boot_lookup_comparator($2));
			free($2);
		}
		| term_literal MODIFIER ARITH END{
			terminal($3, NODE_ARITH, boot_lookup_modifier($2));
			free($2);
		}
		| term_literal MODIFIER UINT_VALUE END{
			terminal($3, NODE_UINT, boot_lookup_modifier($2));
			free($2);
		}
		;

syntax_error:	SYNTAX_ERROR {
			booterror("syntax error");
		}
		;


%%

extern void boot_scan_string(const char *configuration);
extern int boot_linenum;
extern "C" int bootparse();
extern int bootlex();

static ConfigTree *config_tree = NULL;
static string boot_filename;
static string lastsymbol;
static uint64_t nodenum;


static void
extend_path(char* segment, int type, uint64_t node_num)
{
    lastsymbol = segment;
    config_tree->extend_path(string(segment), type, node_num);
    free(segment);
}

static void
push_path()
{
    config_tree->push_path();
}

static void
pop_path()
{
    config_tree->pop_path();
}

static void
terminal(char* value, int type, ConfigOperator op)
{
    push_path();

    lastsymbol = value;

    config_tree->terminal_value(string(value), type, op);
    free(value);
    pop_path();
}

void
booterror(const char *s) throw (ParseError)
{
    string errmsg;

    if (! boot_filename.empty()) {
	errmsg = c_format("PARSE ERROR [Config File %s, line %d]: %s",
			  boot_filename.c_str(),
			  boot_linenum, s);
    } else {
	errmsg = c_format("PARSE ERROR [line %d]: %s", boot_linenum, s);
    }
    errmsg += c_format("; Last symbol parsed was \"%s\"", lastsymbol.c_str());

    xorp_throw(ParseError, errmsg);
}

int
init_bootfile_parser(const char *configuration,
		     const char *filename,
		     ConfigTree *ct)
{
    config_tree = ct;
    boot_filename = filename;
    boot_linenum = 1;
    boot_scan_string(configuration);
    return 0;
}

void
parse_bootfile() throw (ParseError)
{
    if (bootparse() != 0)
	booterror("unknown error");
}

ConfigOperator boot_lookup_comparator(const char* s)
{
    char *s0, *s1, *s2;

    /* skip leading spaces */
    s0 = strdup(s);
    s1 = s0;
    while (*s1 != '\0' && *s1 == ' ') {
        s1++;
    }

    /* trim trailing spaces */
    s2 = s1;
    while (*s2 != '\0') {
        if (*s2 == ' ') {
            *s2 = 0;
            break;
        }
        s2++;
    }

    ConfigOperator op;
    string str = s1;
    free(s1);
    try {
        op = lookup_comparator(str);
	return op;
    } catch (const ParseError& pe) {
        string errmsg = pe.why();
	errmsg += c_format("\n[Line %d]\n", boot_linenum);
	errmsg += c_format("Last symbol parsed was \"%s\"", lastsymbol.c_str());
	xorp_throw(ParseError, errmsg);
    }
    XLOG_UNREACHABLE();
}

ConfigOperator boot_lookup_modifier(const char* s)
{
    char *s0, *s1, *s2;

    /* skip leading spaces */
    s0 = strdup(s);
    s1 = s0;
    while (*s1 != '\0' && *s1 == ' ') {
        s1++;
    }

    /* trim trailing spaces */
    s2 = s1;
    while (*s2 != '\0') {
        if (*s2 == ' ') {
            *s2 = 0;
            break;
        }
        s2++;
    }

    ConfigOperator op;
    string str = s1;
    free(s1);
    try {
        op = lookup_modifier(str);
	return op;
    } catch (const ParseError& pe) {
        string errmsg = pe.why();
	errmsg += c_format("\n[Line %d]\n", boot_linenum);
	errmsg += c_format("Last symbol parsed was \"%s\"", lastsymbol.c_str());
	xorp_throw(ParseError, errmsg);
    }
    XLOG_UNREACHABLE();
}
