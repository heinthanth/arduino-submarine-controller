namespace svr_builtin
{
	String page_404(String uri, IPAddress ip, int port) {
        return
        "<!DOCTYPE html>"
        "<html>"
        "    <head>"
        "        <title>404 - Not Found</title>"
        "    </head>"
        "    <body>"
        "        <h1>Not Found</h1>"
        "        <p>The requested URL " + uri + " was not found on this server.</p>"
        "        <hr>"
        "        <p>NodeMCU ESP8266 Server at " + ip.toString() + " Port " + port + "</p>"
        "    </body>"
        "</html>";
    }
}