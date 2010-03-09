#include "xml2html.h"
#include <grace/pcre.h>
#include <grace/filesystem.h>

$appobject(xml2htmlApp);

// ==========================================================================
// METHOD xml2htmlApp::main
// ==========================================================================
int xml2htmlApp::main (void)
{
	string xml = fs.load (argv["*"][0]);
	string res = "<div class=\"code\"><br/>\n";
	
	for (int i=0; i<xml.strlen(); ++i)
	{
		if (xml[i] == '<')
		{
			bool inspan=true;
			++i;
			res.strcat ("<span class=\"xmltag\">&lt;");
			while ((i<xml.strlen()) && (xml[i] != '>'))
			{
				if (xml[i] == '\"')
				{
					res.strcat ("</span><span class=\"string\">\"");
					++i;
					while ((i<xml.strlen()) && (xml[i] != '\"'))
					{
						res.strcat (xml[i]);
						++i;
					}
					res.strcat ("\"</span><span class=\"xmlattr\">");
				}
				else if ((xml[i] == ' ') && inspan)
				{
					res.strcat ("</span> <span class=\"xmlattr\">");
					inspan = false;
				}
				else res.strcat (xml[i]);
				++i;
			}
			if (! inspan) res.strcat ("</span><span class=\"xmltag\">");
			res.strcat ("&gt;</span>");
		}
		else if (xml[i] == ' ')
		{
			if (((i+1)<xml.strlen()) && (xml[i+1] == ' '))
			{
				res.strcat ("&nbsp;");
				++i;
			}
			res.strcat (' ');
		}
		else if (xml[i] == '\t')
		{
			res.strcat ("&nbsp; &nbsp; ");
		}
		else if (xml[i] == '\n')
		{
			res.strcat ("<br/>\n");
		}
		else
		{
			res.strcat (xml[i]);
		}
	}
	res += "<br/></div>\n";
	fout.puts (res);
	return 0;
}
