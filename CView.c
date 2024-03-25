#include <SDL2/SDL.h>
#include <webkit2/webkit2.h>

int main(int argc, char *argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return -1;
    }

    // Create a SDL window
    SDL_Window *window = SDL_CreateWindow("SDL Webview", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        return -1;
    }

    // Create a SDL renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        return -1;
    }

    // Create a GTK WebKit WebView
    GtkWidget *webview = webkit_web_view_new();
    if (!webview) {
        fprintf(stderr, "Webview creation failed\n");
        return -1;
    }

    // Load HTML content
    webkit_web_view_load_html(WebKitWebView(webview),
        "<!DOCTYPE html>"
        "<html lang=\"en\">"
        "<head>"
        "    <meta charset=\"UTF-8\">"
        "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
        "    <title>SDL Webview</title>"
        "</head>"
        "<body>"
        "    <h1>Hello, SDL Webview!</h1>"
        "</body>"
        "</html>", NULL);

    // Create a GTK widget for SDL window
    GtkWidget *sdl_socket = gtk_socket_new();
    gtk_widget_set_size_request(sdl_socket, 800, 600);
    gtk_container_add(GTK_CONTAINER(sdl_socket), webview);
    gtk_widget_realize(sdl_socket);
    GdkWindow *sdl_window = gtk_widget_get_window(sdl_socket);

    // Set the SDL window handle
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    SDL_GetWindowWMInfo(window, &info);
    gdk_window_reparent(sdl_window, info.info.x11.window, 0, 0);

    // Main event loop
    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        SDL_Delay(10);
    }

    // Cleanup
    webkit_web_view_load_html(WebKitWebView(webview), "", NULL);
    gtk_widget_destroy(sdl_socket);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
