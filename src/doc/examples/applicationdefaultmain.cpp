#include <QtCore/QCoreApplication>

#include <Tufao/HttpServer>

#include <Tufao/HttpServerRequestRouter>
#include <Tufao/HttpFileServer>
#include <Tufao/HttpPluginServer>

#include "notfound.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(static);
    // First we create the event loop and the server objects
    QCoreApplication a(argc, argv);
    Tufao::HttpServer server;
    
    // Then we create a request router and configure it...
    Tufao::HttpServerRequestRouter router;
    
    //   ...using some handlers
    
    // to allow you change the running code without restart the application
    Tufao::HttpPluginServer pluginServer("routes.conf", true);
    // to server static files under public folder
    Tufao::HttpFileServer fileServer("public");
    // to respond the remaining requests with a "404 - not found"
    NotFoundHandler handler404;
    
    // you should also create handlers to:
    //   - "/" (maybe redirecting to some other path)
    //   - "/favicon.ico" (to serve the page's icon)
    router.map(QRegExp(""), &pluginServer)
            .map(QRegExp(""), &fileServer)
            .map(QRegExp(""), &handler404);
    
    // We set the router as the global request handler
    QObject::connect(&server, SIGNAL(requestReady(Tufao::HttpServerRequest*,Tufao::HttpServerResponse*)),
                     &router, SLOT(handleRequest(Tufao::HttpServerRequest*,Tufao::HttpServerResponse*)));
    
    // Last, we run our server...
    server.listen(QHostAddress::Any, 8080);

    //   ...and start the event loop    
    return a.exec();
}
