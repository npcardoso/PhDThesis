package io.crowbar.diagnostic;

import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpExchange;
import java.io.IOException;
import java.io.File;
import java.io.OutputStream;
import java.io.FileInputStream;

public final class StaticContentHttpHandler implements HttpHandler {
    private String root;

    public String getRoot () {
        return root;
    }

    public void setRoot (String root) throws IOException {
        this.root = new File(root).getCanonicalFile().getPath();
    }

    public StaticContentHttpHandler (String root) throws IOException {
        setRoot(root);
    }

    public void handle (HttpExchange t) throws IOException {
        String contextPath = t.getHttpContext().getPath();
        String requestPath = t.getRequestURI().getPath();

        String relativePath = requestPath.substring(contextPath.length());


        System.out.println("Trying to access: " + relativePath + ": " + root + relativePath);

        File file = new File(root + relativePath).getCanonicalFile();

        if (!file.getPath().startsWith(root)) {
            // Suspected path traversal attack: reject with 403 error.
            String response = "403 (Forbidden)\n";
            t.sendResponseHeaders(403, response.length());
            OutputStream os = t.getResponseBody();
            os.write(response.getBytes());
            os.close();
        } else if (!file.isFile()) {
            // Object does not exist or is not a file: reject with 404 error.
            String response = "404 (Not Found)\n";
            t.sendResponseHeaders(404, response.length());
            OutputStream os = t.getResponseBody();
            os.write(response.getBytes());
            os.close();
        } else {
            // Object exists and is a file: accept with response code 200.
            t.sendResponseHeaders(200, 0);
            OutputStream os = t.getResponseBody();
            FileInputStream fs = new FileInputStream(file);
            final byte[] buffer = new byte[0x10000];
            int count = 0;

            while ((count = fs.read(buffer)) >= 0) {
                os.write(buffer, 0, count);
            }

            fs.close();
            os.close();
        }
    }
}