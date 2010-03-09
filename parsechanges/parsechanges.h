#ifndef _parsechanges_H
#define _parsechanges_H 1
#include <grace/application.h>

//  -------------------------------------------------------------------------
/// Main application class.
//  -------------------------------------------------------------------------
class parsechangesApp : public application
{
public:
		 		 parsechangesApp (void) :
					application ("nl.madscience.site.parsechanges")
				 {
				 	opt = $("-h", $("long", "--help")) ->
				 		  $("-t", $("long", "--toc")) ->
				 		  $("-i", $("long", "--input")) ->
				 		  $("--toc",
				 		  	$("argc",1) ->
				 		  	$("default","toc.xml") ->
				 		  	$("help","Location of the toc.xml file")
				 		   ) ->
				 		  $("--input",
				 		  	$("argc", 1) ->
				 		  	$("default","Changes.txt") ->
				 		  	$("help","Location of the Changes.txt file")
				 		   );
				 }
				~parsechangesApp (void)
				 {
				 }
	
	int			 main (void);

};

#endif

