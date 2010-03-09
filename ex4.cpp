#include <grace/daemon.h>
#include <grace/httpd.h>
#include <grace/lock.h>

//  -------------------------------------------------------------------------
/// HTTP handler object for a simple in-memor document store
//  -------------------------------------------------------------------------
class memstore : public httpdobject
{
public:
					 /// Constructor.
					 /// \param srv Reference to parent httpd.
					 memstore (httpd &srv);
					~memstore (void);
					
					 /// Run-method.
					 /// \param uri The request URI
					 /// \param postbody Posted data
					 /// \param inhdr Input headers
					 /// \param out Output data
					 /// \param outhdr Output headers
					 /// \param env Meta-variables
					 /// \param s Raw socket.
	int				 run (string &uri, string &postbody, value &inhdr,
						  string &out, value &outhdr, value &env,
						  tcpsocket &s);
						  
	value			*put (const statstring &uri, const value &v);
	value			*post (const statstring &uri, const value &v);
	value			*del (const statstring &uri);
						  
protected:
	lock<value>      db; ///< The memory database.
};

//  -------------------------------------------------------------------------
/// Main daemon class.
//  -------------------------------------------------------------------------
class memstored : public daemon
{
public:
					 memstored (void);
					~memstored (void);
					
	int				 main (void);
	httpd			 srv;
};

// ==========================================================================
// CONSTRUCTOR memstore
// ==========================================================================
memstore::memstore (httpd &srv)
	: httpdobject (srv, "*")
{
}

// ==========================================================================
// DESTRUCTOR memstore
// ==========================================================================
memstore::~memstore (void)
{
}

// ==========================================================================
// METHOD memstore::put
// ==========================================================================
value *memstore::put (const statstring &uri, const value &dat)
{
	returnclass (value) res retain;
	
	exclusivesection (db)
	{
		if (! db.exists (uri))
		{
			db[uri] = dat;
			res = $("ok", true);
		}
		else
		{
			res = $("ok", false) -> $("error", "Resource exists");
		}
	}
	
	return &res;
}

// ==========================================================================
// METHOD memstore::post
// ==========================================================================
value *memstore::post (const statstring &uri, const value &dat)
{
	returnclass (value) res retain;
	
	exclusivesection (db)
	{
		if (db.exists (uri))
		{
			db[uri] = dat;
			res = $("ok", true);
		}
		else
		{
			res = $("ok", false) -> $("error", "Resource not found");
		}
	}
	
	return &res;
}

// ==========================================================================
// METHOD memstore::del
// ==========================================================================
value *memstore::del (const statstring &uri)
{
	returnclass (value) res retain;
	
	exclusivesection (db)
	{
		if (db.exists (uri))
		{
			db.rmval (uri);
			res = $("ok", true);
		}
		else
		{
			res = $("ok", false) -> $("error", "Resource not found");
		}
	}
	
	return &res;
}

// ==========================================================================
// METHOD memstore::run
// ==========================================================================
int memstore::run (string &uri, string &postbody, value &inhdr,
				   string &out, value &outhdr, value &env,
				   tcpsocket &s)
{
	value v;
	outhdr["Content-type"] = "application/json";
	
	caseselector (env["method"])
	{
		incaseof ("GET") :
			sharedsection (db)
			{
				if (db.exists (uri))
				{
					const value &vv = db[uri];
					outhdr["Content-type"] = vv["Content-type"];
					out = vv["data"].sval();
					breaksection return 200;
				}
			}
			
			v = $("ok",false) -> $("error","Not found");
			out = v.tojson ();
			return 404;
		
		incaseof ("POST") :
			v = $("Content-type",inhdr["Content-type"]) ->
				$("data", postbody);
			
			v = post (uri, v);
			out = v.tojson ();
			
			log::write (log::info, "memstore", "%P update <%s>"
						%format (env["ip"], uri));
						
			return v["ok"] ? 200 : 404;
			
		incaseof ("PUT") :
			v = $("Content-type",inhdr["Content-type"]) ->
				$("data", postbody);
			
			v = put (uri, v);
			out = v.tojson ();

			log::write (log::info, "memstore", "%P store <%s>"
						%format (env["ip"], uri));
			
			return v["ok"] ? 200 : 405;
		
		incaseof ("DELETE") :
			v = del (uri);
			out = v.tojson ();

			log::write (log::info, "memstore", "%P delete <%s>"
						%format (env["ip"], uri));
						
			return v["ok"] ? 200 : 404;
		
		defaultcase :
			return 500;
		
	}
}

// ==========================================================================
// CONSTRUCTOR memstored
// ==========================================================================
memstored::memstored (void) : daemon ("memstored")
{
	opt = $("-p", $("long", "--port")) ->
		  $("-h", $("long", "--help")) ->
		  $("--port",
		  		$("argc", 1) ->
		  		$("default", 1135) ->
		  		$("help", "TCP listen port number"));
}

// ==========================================================================
// DESTRUCTOR memstored
// ==========================================================================
memstored::~memstored (void)
{
}

// ==========================================================================
// METHOD memstored::main
// ==========================================================================
int memstored::main (void)
{
	addlogtarget (log::file, "event.log", log::all);
	int port = argv["--port"];
	srv.listento (port);
	log::write (log::info, "main", "Starting server on "
				"port *:%i" %format (port));
	
	daemonize ();
	log::write (log::info, "main", "Starting threads");
	new memstore (srv);
	srv.start ();
	
	while (true)
	{
		value ev = waitevent ();
		if (ev.type() == "shutdown") break;
	}
	
	log::write (log::info, "main", "Stopping web service");
	srv.shutdown ();
	log::write (log::info, "main", "Shutting down log thread");
	stoplog ();
	
	return 0;
}

$appobject (memstored);
$version (1.0);
