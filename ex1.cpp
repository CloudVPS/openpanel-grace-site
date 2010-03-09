#include <grace/application.h>
#include <grace/filesystem.h>

class helloapp : public application
{
public:
	helloapp (void) : application ("hello")
	{
	}
	~helloapp (void)
	{
	}
	
	int main (void)
	{
		value greetings =
			$("world", "Hello") ->
			$("everyone", "Hi") ->
			$("you", "Hey");
			
		foreach (g, greetings)
		{
			fout.writeln ("%s, %s." %format (g, g.id()));
		}
		
		return 0;
	}
};

$appobject (helloapp);
