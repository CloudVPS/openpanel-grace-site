#include "parsechanges.h"
#include <grace/strutil.h>
#include <grace/filesystem.h>

$appobject(parsechangesApp);

// ==========================================================================
// METHOD parsechangesApp::main
// ==========================================================================
int parsechangesApp::main (void)
{
	string doc = fs.load (argv["--input"]);
	value res;
	
	res.loadxml (argv["--toc"]);
	
	value &out = res["changes"];
	int bulletno = -1;
	statstring curversion;
	string curline;
	
	value lines = strutil::splitlines (doc);
	foreach (line, lines)
	{
		const string &ln = line;
		if (ln.strlen())
		{
			if (isdigit (ln[0]))
			{
				if (curline && bulletno>=0)
				{
					out[curversion]["bullets"][bulletno]["bullet"] = curline;
					curline.crop ();
				}
				value splt = strutil::splitspace (ln);
				curversion = splt[0];
				out[curversion]["date"] =
					"%s %s %s %s" %format (splt[1],splt[2],splt[3],splt[4]);
				bulletno = -1;
				continue;
			}
			string l = ln;
			l.chomp ();
			l.replace ($("<","&lt;") -> $(">","&gt;"));
			
			if (l.strlen() && l[0] == '*')
			{
				l = l.mid (2);
				if (curline && bulletno>=0)
				{
					out[curversion]["bullets"][bulletno]["bullet"] = curline;
					curline.crop ();
				}
				bulletno++;
			}
			
			if (l.strlen())
			{
				if (curline) curline.strcat (" %s" %format (l));
				else curline = l;
			}
		}
	}
	
	if (curline && bulletno>=0)
	{
		out[curversion]["bullets"][bulletno]["bullet"] = curline;
	}
	
	res.savexml (argv["--toc"]);
	return 0;
}
