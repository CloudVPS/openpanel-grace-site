#include <grace/daemon.h>
#include <grace/httpd.h>

/// A serverpage sending out a nice greeting to the world
/// on the '/' URI.
class hellopage : public serverpage
{
public:
	/// Constructor: Set up serverpage and URI.
	/// \param pparent The httpd to attach to.
	hellopage (httpd &pparent) : serverpage (pparent, "/")
	{
	}
	
	/// Boring virtual destructor.
	~hellopage (void)
	{
	}
	
	/// Serverpage execution, prints greeting.
	/// \param env Meta variables.
	/// \param argv POST or GET variables.
	/// \param out Output page.
	/// \param outhdr Output headers.
	int execute (value &env, value &argv, string &out, value &outhdr)
	{
		outhdr["Content-type"] = "text/plain";
		out = "Hello, world.\n";
		
		return 200;
	}
};

/// Main daemon object.
class helloserver : public daemon
{
public:
	/// Constructor. Set up command line arguments.
	helloserver (void) : daemon ("hello")
	{
		opt = $("-p", $("long", "--port")) ->
			  $("-h", $("long", "--help")) ->
			  $("--port",
			  		$("argc", 1) ->
			  		$("default", 1337) ->
			  		$("help", "TCP listening port")
			   );
	}
	
	/// Virtual destructor.
	~helloserver (void)
	{
	}
	
	/// Main method. Starts the service and waits for an opportunity
	/// to shut down.
	int main (void)
	{
		/// Log in the current directory.
		addlogtarget (log::file, "event.log", log::all);
		
		/// Set up listening port from command line argument (or its
		/// provided default).
		int port = argv["--port"];
		srv.listento (port);
		log::write (log::info, "main", "Starting webserver "
					"on *:%i" %format (port));

		/// Spawn to the background.
		daemonize ();
		
		/// Set up the httpd object chain and start the httpd threads.
		new hellopage (srv);
		srv.start ();
		
		while (true)
		{
			value ev = waitevent ();
			if (ev.type() == "shutdown") break;
		}
		
		/// Shut down auxiliary threads.
		log::write (log::info, "main", "Shutting down webserver");
		srv.shutdown ();
		
		log::write (log::info, "main", "Shutting down log thread");
		stoplog ();
		
		return 0;
	}
	
	httpd srv;
};

$appobject (helloserver);
$version (1.0);
