#include "mksite.h"
#include <grace/filesystem.h>
#include <grace/system.h>

$appobject(mksiteApp);

// ==========================================================================
// METHOD mksiteApp::main
// ==========================================================================
int mksiteApp::main (void)
{
	foreach (curfile, argv["*"])
	{
		fs.rm ("site/%s" %format (curfile));
		fout.writeln (">>> %s" %format (curfile));
		string filedat = fs.load (curfile);
		
		filedat.replace ($("<sym>","<<sym>>") ->
						 $("</sym>","<</sym>>") ->
						 $("<sh>","<<sh>>") ->
						 $("</sh>","<</sh>>") ->
						 $("<file>","<<file>>") ->
						 $("</file>","<</file>>") ->
						 $("<xmltag>","<<xmltag>>") ->
						 $("</xmltag>","<</xmltag>>") ->
						 $("<class>","<<class name=\"") ->
						 $("</class>","\">>") ->
						 $("<h2>","<<section name=\"") ->
						 $("</h2>","\">>"));
		
		value lines = strutil::splitlines (filedat);
		outtext.crop ();

		for (int i=0; i<lines.count(); ++i)
		{
			const string &line = lines[i];
			string ln = line;
			ln.chomp ();
			if (ln.strlen() && (ln[0] == '%'))
			{
				string cmd = ln.cutat (' ');
				caseselector (cmd)
				{
					incaseof ("%include") :
						fout.writeln ("   include %s" %format (ln));
						core.sh ("./grace2html/grace2html %s >> _html" %format (ln));
						string txt = fs.load ("_html");
						txt.replace ($("$","$$"));
						outtext.strcat (txt);
						fs.rm ("_html");
						break;
						
					incaseof ("%terminal") :
						fout.writeln ("   term %s" %format (ln));
						printterminal (ln);
						break;
					
					incaseof ("%code") :
						fout.writeln ("   code %s" %format (ln));
						handlecode (lines, i);
						break;
					
					incaseof ("%xmlcode") :
						fout.writeln ("   xml %s" %format (ln));
						handlexml (lines, i);
						break;
						
					defaultcase :
						break;
				}
			}
			else
			{
				outtext.strcat (line);
				outtext.strcat ('\n');
			}
		}
		fs.save ("%s.tmphtml" %format (curfile), outtext);
		core.sh ("./htparse/htparse -x toc.xml -i template.thtml %s.tmphtml > site/%s"
				 %format (curfile, curfile));
		
		fs.rm ("%s.tmphtml" %format (curfile));
	}
	return 0;
}

// ==========================================================================
// METHOD mksiteApp::printterminal
// ==========================================================================
void mksiteApp::printterminal (const string &file)
{
	string dat = fs.load (file);
	value lines = strutil::splitlines (dat);

	outtext.strcat ("<div class=\"terminal\"><pre>\n");
	foreach (vln, lines)
	{
		string line = vln;
		line.replace ($("$","$$")->$("<","&lt;")->$(">","&gt;"));
		if (! line.strlen())
		{
			outtext.strcat ('\n');
			continue;
		}
		if (line[0] == '$')
		{
			line = line.mid (2);
			outtext.strcat ("<span class=\"prompt\">%s</span>\n" %format (line));
		}
		else outtext.strcat ("%s\n" %format (line));
	}
	outtext.strcat ("</pre></div>\n");
}

// ==========================================================================
// METHOD mksiteApp::handlecode
// ==========================================================================
void mksiteApp::handlecode (value &lines, int &i)
{
	i++;
	file outf;
	outf.openwrite ("__code.cpp");
	for (;i < lines.count(); ++i)
	{
		string ln = lines[i];
		ln.chomp();
		if (ln == "%endcode") break;
		ln = lines[i].sval().mid(1);
		ln.replace ($("$","$$")->$("@","$atsign$"));
		outf.writeln (ln);
	}
	outf.close ();
	core.sh ("./grace2html/grace2html __code.cpp > _html");
	outtext.strcat (fs.load ("_html"));
	fs.rm ("__code.cpp");
	fs.rm ("_html");
}

// ==========================================================================
// METHOD mksiteApp::handlexml
// ==========================================================================
void mksiteApp::handlexml (value &lines, int &i)
{
	i++;
	file outf;
	outf.openwrite ("__code.xml");
	for (;i < lines.count(); ++i)
	{
		string ln = lines[i];
		ln.chomp();
		if (ln == "%endcode") break;
		ln = lines[i].sval().mid(1);
			ln.replace ($("$","$$"));
		outf.writeln (ln);
	}
	outf.close ();
	core.sh ("./xml2html/xml2html __code.xml > _html");
	outtext.strcat (fs.load ("_html"));
	fs.rm ("__code.xml");
	fs.rm ("_html");
}

