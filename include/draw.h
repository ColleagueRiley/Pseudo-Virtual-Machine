void drawPixel(RGFW_window* win, unsigned int x, unsigned int y, char color[3]) {
    int i;

    for (i = 0; i < 4; i++)
        win->buffer[4 * win->h * x + y * 4 + i] = color[
                                                        i == 2 ? 0 : 
                                                        i == 0 ? 2 :
                                                        i
                                                       ];
}

void drawRect(RGFW_window* win, unsigned int x, unsigned int y, unsigned int w, unsigned int h, char color[3]) {
    unsigned int mx, my, z;

    for (my = y; my < y + h; my++)
        for (mx = x; mx < x + w; mx++)
            drawPixel(win, mx, my, color);
}

void clearScreen(RGFW_window* win) {
    drawRect(win, 0, 0, win->w, win->h, (char[]){0, 0, 0});
}