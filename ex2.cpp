#include <grace/application.h>
#include <grace/filesystem.h>

class helloapp : public application
{
public:
	/// Constructor.
	/// Sets up the option parsing for this application. The --help
	/// option has a built-in binding.
	helloapp (void) : application ("hello")
	{
		opt = $("-l", $("long", "--lang")) ->
			  $("-h", $("long", "--help")) ->
			  $("--lang",
			  		$("argc", 1) ->
			  		$("help", "Language (en,nl)") ->
			  		$("default", "en")
			   );
	}
	
	/// Destructor.
	~helloapp (void)
	{
	}
	
	/// Get a greeting apropriate for a given language.
	/// \param language the language code to look up.
	value *getgreetings (const statstring &language)
	{
		caseselector (language)
		{
			incaseof ("en") :
				return $("world", "Hello") ->
					   $("everyone", "Hi") ->
					   $("you", "Hey");
			
			incaseof ("nl") :
				return $("wereld", "Hallo") ->
					   $("iedereen", "Hoi") ->
					   $("jij", "Hee");
			
			defaultcase :
				return NULL;
		}
	}
	
	/// Main method.
	int main (void)
	{
		// --lang defaults to 'en' as per opt["--lang"]["default"];
		value greetings = getgreetings (argv["--lang"]);
		if (! greetings)
		{
			ferr.writeln ("Unrecognized language: %s" %format (argv["--lang"]));
			return 1;
		}
		
		foreach (g, greetings)
		{
			fout.writeln ("%s, %s." %format (g, g.id()));
		}
		
		return 0;
	}
};

$appobject (helloapp);
$version (1.0.1);
