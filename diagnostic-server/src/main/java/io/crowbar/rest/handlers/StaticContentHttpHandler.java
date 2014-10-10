package io.crowbar.rest.handlers;

import com.sun.net.httpserver.HttpExchange;
import java.io.IOException;
import java.io.File;
import java.io.OutputStream;
import java.io.FileInputStream;

public final class StaticContentHttpHandler extends AbstractHttpHandler {
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

    public void handle (HttpExchange t,
                        String relativePath) throws Exception {
        File file = new File(root + "/" + relativePath).getCanonicalFile();


        System.out.println("trying to access: " + file);

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