#include <grace/application.h>
#include <grace/filesystem.h>

class HelloApp : public application
{
public:
	HelloApp (void) : application ("hello")
	{
	}
	~HelloApp (void)
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

$appobject (HelloApp);
