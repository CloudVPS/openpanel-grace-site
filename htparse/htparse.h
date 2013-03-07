#ifndef _htparse_H
#define _htparse_H 1
#include <grace/application.h>

//  -------------------------------------------------------------------------
/// Main application class.
//  -------------------------------------------------------------------------
class htparseApp : public application
{
public:
		 	 htparseApp (void) :
				application ("nl.madscience.tools.htparse")
			 {
			 	opt = $("-x", $("long", "--xml")) ->
			 		  $("-i", $("long", "--include")) ->
			 		  $("-h", $("long", "--help")) ->
			 		  $("--xml",
			 		  		$("argc", 1) ->
			 		  		$("help", "Load environment from XML file")
			 		   ) ->
			 		  $("--include",
			 		  		$("argc", 1) ->
			 		  		$("help", "Template file to include")
			 		   );
			 }
			~htparseApp (void)
			 {
			 }

	int		 main (void);
};

#endif
