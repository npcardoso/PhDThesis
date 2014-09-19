package io.crowbar.diagnostic;

import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;

import java.io.IOException;
import java.io.OutputStream;

public abstract class AbstractHttpHandler implements HttpHandler {
    public void handle (HttpExchange t,
                        String relativePath) throws Exception {}

    public final void handle (HttpExchange t) throws IOException {
        String contextPath = t.getHttpContext().getPath();
        String requestPath = t.getRequestURI().getPath();
        String relativePath = requestPath.substring(contextPath.length());


        System.out.println("Got Connection @ " + contextPath + ": " + relativePath);

        OutputStream os = t.getResponseBody();
        try {
            handle(t, relativePath);
        }
        catch (Throwable e) {
            e.printStackTrace();
            String response = "403 (Forbidden)\n";
            t.sendResponseHeaders(403, response.length());
            os.write(response.getBytes());
        }
        os.close();
    }
}