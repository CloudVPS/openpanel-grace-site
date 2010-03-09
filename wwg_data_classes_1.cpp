#include <grace/str.h> 
#include <grace/value.h>

// Main application method 
int myApp::main (void) 
{ 
    string mystr = "Hello, world"; 
    statstring statstr = "messageOfPeace";
    value val = $(statstr, mystr) ->
    			$("ultimateAnswer", 42);

    if (mystr != val[statstr]) 
    { 
        ferr.writeln ("Something's wrong with this world!"); 
        return 1; 
    }
    
    // Clear the string and put some formatted text in 
    mystr = "Message of Peace: %[messageOfPeace]s\n"
    		"Ultimate Answer: %[ultimateAnswer]i\n" %format (val);

    // Send the string to the output channel. 
    fout.puts (mystr); 
    return 0; 
}
