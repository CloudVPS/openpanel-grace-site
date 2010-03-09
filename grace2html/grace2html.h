#ifndef _grace2html_H
#define _grace2html_H 1
#include <grace/application.h>

//  -------------------------------------------------------------------------
/// Main application class.
//  -------------------------------------------------------------------------
class grace2htmlApp : public application
{
public:
		 	 grace2htmlApp (void) :
				application ("nl.madscience.tools.grace2html")
			 {
			 }
			~grace2htmlApp (void)
			 {
			 }

	int		 main (void);
};

#endif
