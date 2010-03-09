#ifndef _xml2html_H
#define _xml2html_H 1
#include <grace/application.h>

//  -------------------------------------------------------------------------
/// Main application class.
//  -------------------------------------------------------------------------
class xml2htmlApp : public application
{
public:
		 	 xml2htmlApp (void) :
				application ("nl.madscience.tools.xml2html")
			 {
			 	opt = $("-h", $("long", "--help"));
			 }
			~xml2htmlApp (void)
			 {
			 }

	int		 main (void);
};

#endif

