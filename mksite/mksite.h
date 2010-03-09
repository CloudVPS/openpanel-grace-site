#ifndef _mksite_H
#define _mksite_H 1
#include <grace/application.h>

//  -------------------------------------------------------------------------
/// Main application class.
//  -------------------------------------------------------------------------
class mksiteApp : public application
{
public:
		 	 mksiteApp (void) :
				application ("nl.madscience.tools.mksite")
			 {
			 }
			~mksiteApp (void)
			 {
			 }

	int		 main (void);
	void	 printterminal (const string &);
	void	 handlecode (value &, int &);
	void	 handlexml (value &, int &);
	
	string	 outtext;
};

#endif

