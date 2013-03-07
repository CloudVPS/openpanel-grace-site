#include "htparse.h"
#include <grace/scriptparser.h>
#include <grace/filesystem.h>

APPOBJECT(htparseApp);

//  =========================================================================
/// Main method.
//  =========================================================================
int htparseApp::main (void)
{
	string scriptfile = argv["*"][0];
	if (! scriptfile) return 1;
	
	value senv;
	if (argv.exists ("--xml"))
	{
		senv.loadxml (argv["--xml"]);
	}
	
	argv["*"].rmindex (0);
	foreach (arg, argv["*"])
	{
		string name, val;
		val = arg;
		name = val.cutat ('=');
		senv[name] = val;
	}
	
	string script;
	if (argv.exists ("--include"))
		script = fs.load (argv["--include"]);
		
	argv["*"].rmindex (0);
	foreach (varset, argv["*"])
	{
		string val = varset;
		string var = val.cutat ('=');
		senv[var] = val;
	}
	senv["_file"] = scriptfile;

	script.strcat (fs.load (scriptfile));
	
	scriptparser P;
	P.build (script);
	
	string buffer;
	P.run (senv, buffer, "main");
	
	fout.puts (buffer);
	return 0;
}

