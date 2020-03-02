namespace svr_builtin
{
	String page_404(String uri, IPAddress ip, int port) {
        return
        "<!DOCTYPE html>"
        "<html>"
        "    <head>"
        "        <meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">"
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

    String page_422(String uri, IPAddress ip, int port) {
        return
        "<!DOCTYPE html>"
        "<html>"
        "    <head>"
        "        <meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">"
        "        <title>404 - Unprocessable Entity</title>"
        "    </head>"
        "    <body>"
        "        <h1>Unprocessable Entity</h1>"
        "        <p>Incomplete request: argument required.</p>"
        "        <hr>"
        "        <p>NodeMCU ESP8266 Server at " + ip.toString() + " Port " + port + "</p>"
        "    </body>"
        "</html>";
    }

    String page_home(String uri, IPAddress ip, int port) {
        return
        "<!DOCTYPE html>"
        "<html>"
        "    <head>"
        "        <meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">"
        "        <title>Home - NodeMCU ESP8266 Server</title>"
        "    </head>"
        "    <body>"
        "        <h1>It's works!</h1>"
        "        <p>Server is working properly!</p>"
        "        <hr>"
        "        <p>NodeMCU ESP8266 Server at " + ip.toString() + " Port " + port + "</p>"
        "    </body>"
        "</html>";
    }
}