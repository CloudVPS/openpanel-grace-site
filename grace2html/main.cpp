#include "grace2html.h"
#include <grace/filesystem.h>
#include <grace/strutil.h>

APPOBJECT(grace2htmlApp);

//  =========================================================================
/// Main method.
//  =========================================================================
int grace2htmlApp::main (void)
{
	string code;
	code = fs.load (argv["*"][0]);
	bool indouble = false;
	bool insingle = false;
	bool inccomment = false;
	bool incppcomment = false;
	
	value kwdb =
		$("bool", 1) ->
		$("true", 1) ->
		$("false", 1) ->
		$("if", 1) ->
		$("else", 1) ->
		$("return", 1) ->
		$("while", 1) ->
		$("do", 1) ->
		$("for", 1) ->
		$("select", 1) ->
		$("case", 1) ->
		$("default", 1) ->
		$("break", 1) ->
		$("continue", 1) ->
		$("char", 1) ->
		$("short", 1) ->
		$("int", 1) ->
		$("void", 1) ->
		$("double", 1) ->
		$("float", 1) ->
		$("unsigned", 1) ->
		$("long", 1) ->
		$("const", 1) ->
		$("class", 1) ->
		$("public", 1) ->
		$("private", 1) ->
		$("protected", 1) ->
		$("new", 1) ->
		$("extern", 1) ->
		$("value", 1) ->
		$("statstring", 1) ->
		$("string", 1) ->
		$("returnclass", 1) ->
		$("retain", 1) ->
		$("foreach", 1) ->
		$("sharedsection", 1) ->
		$("exclusivesection", 1) ->
		$("breaksection", 1) ->
		$("caseselector", 1) ->
		$("incaseof", 1) ->
		$("defaultcase", 1) ->
		$("appobject", 1) ->
		$("%format", 1);
	
	value lines = strutil::split (code, '\n');
	
	fout.writeln ("<div class=\"code\"><pre>");
	lines.rmindex (-1);
	
	foreach (line, lines)
	{
		string l = line;
		string out;
		
		for (int i=0; i<l.strlen(); ++i)
		{
			if (l[i] == '\t')
			{
				do
				{
					out.strcat (' ');
				} while (out.strlen() & 3);
			}
			else if (l[i] == '<')
			{
				out.strcat ("&lt;");
			}
			else if (l[i] == '>')
			{
				out.strcat ("&gt;");
			}
			else if (l[i] == '&')
			{
				out.strcat ("&amp;");
			}
			else
			{
				out.strcat (l[i]);
			}
		}
		
		l = out;
		out.crop ();
		
		for (int i=0; i<l.strlen(); ++i)
		{
			if (l[i] == '\\')
			{
				out.strcat (l[i++]);
				out.strcat (l[i]);
			}
			else
			{
				if ((l[i] == '\"') && (! insingle) && (! inccomment) &&
					(! incppcomment))
				{
					indouble = !indouble;
					if (indouble)
					{
						out.strcat ("<span class=\"string\">\"");
					}
					else
					{
						out.strcat ("\"</span>");
					}
				}
				else if ((l[i] == '\'') && (! indouble) && (! inccomment) &&
					     (! incppcomment))
				{
					insingle = !insingle;
					if (insingle)
					{
						out.strcat ("<span class=\"string\">'");
					}
					else
					{
						out.strcat ("'</span>");
					}
				}
				else if ((l[i] == '#') && (! incppcomment) &&
						 (! insingle) && (! indouble))
				{
					out.strcat ("<span class=\"pre\">#");
					incppcomment = true;
				}
				else if ((l[i] == '/') && (! incppcomment) &&
						 (! inccomment) && (! insingle) && (! indouble))
				{
					if (l[i+1] == '/')
					{
						incppcomment = true;
						out.strcat ("<span class=\"comment\">//");
						++i;
					}
					else if (l[i+1] == '*')
					{
						inccomment = true;
						out.strcat ("<span class=\"comment\">/*");
						++i;
					}
					else
					{
						out.strcat (l[i]);
					}
				}
				else if (inccomment && (l[i] == '*') &&
						 (l[i+1] == '/'))
				{
					out.strcat ("*/</span>");
					inccomment = false;
				}
				else if ((! insingle) && (! indouble)  && (! inccomment) &&
						 (! incppcomment))
				{
					char oc = i ? l[i-1] : 0;
					if ((! isalpha (oc)) && (! isdigit (oc)) &&
						(oc != '_') && (oc != '$'))
					{
						int j;
						string kwmatch = l.mid (i, 20);
						for (j=0; j<kwmatch.strlen(); ++j)
						{
							char c = kwmatch[j];
							if ((! isalpha (c)) &&
								(! isdigit (c)) &&
								(c != '$') && (c != '_') &&
								(c != '%')) break;
						}
						if (j<20) kwmatch.crop (j);
						
						if (kwdb.exists (kwmatch))
						{
							out.strcat ("<span class=\"keyword\">"
										"%s</span>" %format (kwmatch));
							i += kwmatch.strlen() -1;
						}
						else
						{
							out.strcat (l[i]);
						}
					}
					else
					{
						out.strcat (l[i]);
					}
				}
				else
				{
					out.strcat (l[i]);
				}
			}
		}
		
		if (incppcomment)
		{
			out.strcat ("</span>");
			incppcomment = false;
		}
		
		fout.writeln (out);
	}
	
	fout.writeln ("</pre></div>");
	
	return 0;
}

