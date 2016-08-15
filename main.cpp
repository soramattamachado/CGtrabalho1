/* 
 * File:   main.cpp
 * Author: renato
 *
 * Created on 12 de Agosto de 2016, 19:04
 */

#include <cstdlib>
#include <gtk/gtk.h>

/* +++++++++++++++++++++++++++++++ CONSTANTS ++++++++++++++++++++++++++++++++ */
static const int WINDOW_WIDTH = 700;
static const int WINDOW_HEIGHT = 500;
static const int OBJECT_VIEWER_WIDTH = 190;
static const int OBJECT_VIEWER_HEIGHT = 150;
static const int STEP_INPUT_AREA_WIDTH = 20;
static const int STEP_INPUT_AREA_HEIGHT = 10;
static const int VIEW_PORT_WIDTH = 500;
static const int VIEW_PORT_HEIGHT = 400;
static const int LOG_TEXT_AREA_WIDTH = 500;
static const int LOG_TEXT_AREA_HEIGHT = 90;
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* +++++++++++++++++++++++++++ GLOBAL VARIABLES +++++++++++++++++++++++++++++ */
/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* ++++++++++++++++++++++++ STATIC METHODS DECLARATION ++++++++++++++++++++++ */
static void close_window(void);
/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data);
/* Handle motion events by continuing to draw if button 1 is
 * still held down. The ::motion-notify signal handler receives
 * a GdkEventMotion struct which contains this information.
 */
static gboolean motion_notify_event_cb(GtkWidget *widget, GdkEventMotion *event, gpointer data);
static void clear_surface(void);
/* Create a new surface of the appropriate size to store our scribbles */
static gboolean configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, gpointer data);
/* Handle button press events by either drawing a rectangle
 * or clearing the surface, depending on which button was pressed.
 * The ::button-press signal handler receives a GdkEventButton
 * struct which contains this information.
 */
static gboolean button_press_event_cb(GtkWidget *widget, GdkEventButton *event, gpointer data);

static void actionAddObject(GtkWidget *w, gpointer p);
static void actionMoveStep(GtkWidget *w, gpointer p);
static void actionMoveUp(GtkWidget *w, gpointer p);
static void actionMoveIn(GtkWidget *w, gpointer p);
static void actionMoveLeft(GtkWidget *w, gpointer p);
static void actionMoveRight(GtkWidget *w, gpointer p);
static void actionMoveDown(GtkWidget *w, gpointer p);
static void actionMoveOut(GtkWidget *w, gpointer p);
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

int main(int argc, char **argv) {

    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *buttonAddObject;
    GtkWidget *objectsListViewer;
    GtkWidget *buttonStep;
    GtkWidget *StepInputArea;
    GtkWidget *buttonUp;
    GtkWidget *buttonLeft;
    GtkWidget *buttonRight;
    GtkWidget *buttonDown;
    GtkWidget *buttonIn;
    GtkWidget *buttonOut;
    GtkWidget *viewPort;
    GtkWidget *logTextArea;
    GtkTextBuffer *objectsListTextBuffer;
    GtkTextBuffer *stepInputTextBuffer;
    GtkTextBuffer *logTextBuffer;

    gtk_init(&argc, &argv);

    /* DEFINE GRAPHIC ELEMENTS */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    grid = gtk_grid_new();
    buttonAddObject = gtk_button_new_with_label("Adiciona Objeto");
    objectsListViewer = gtk_text_view_new();
    buttonStep = gtk_button_new_with_label("Passo");
    StepInputArea = gtk_text_view_new();
    buttonUp = gtk_button_new_with_label("Up");
    buttonLeft = gtk_button_new_with_label("Left");
    buttonRight = gtk_button_new_with_label("Right");
    buttonDown = gtk_button_new_with_label("Down");
    buttonIn = gtk_button_new_with_label("In");
    buttonOut = gtk_button_new_with_label("Out");
    viewPort = gtk_drawing_area_new();
    logTextArea = gtk_text_view_new();
    objectsListTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(objectsListViewer));
    stepInputTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(StepInputArea));
    logTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(logTextArea));

    /* SET MAIN WINDOW TITLE, SIZE, SIGNAL AND BORDER */
    gtk_window_set_title(GTK_WINDOW(window), "Trabalho 1 de CG");
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
    g_signal_connect(window, "destroy", G_CALLBACK(close_window), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 5);

    /* SET OBJECTS LIST VIEWER SIZE */
    gtk_widget_set_size_request(logTextArea, OBJECT_VIEWER_WIDTH, OBJECT_VIEWER_HEIGHT);

    /* SET STEP INPUT AREA SIZE */
    gtk_widget_set_size_request(StepInputArea, STEP_INPUT_AREA_WIDTH, STEP_INPUT_AREA_HEIGHT);

    /* SET VIEW PORT SIZE */
    gtk_widget_set_size_request(viewPort, VIEW_PORT_WIDTH, VIEW_PORT_HEIGHT);

    /* SET LOG TEXT AREA SIZE */
    gtk_widget_set_size_request(objectsListViewer, LOG_TEXT_AREA_WIDTH, LOG_TEXT_AREA_HEIGHT);

    /* ADD THE GRID TO THE MAIN WINDOW */
    gtk_container_add(GTK_CONTAINER(window), grid);

    /* ADD GRAPHIC ELEMENTS TO THE GRID */
    gtk_grid_attach(GTK_GRID(grid), buttonAddObject,    0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), objectsListViewer,  0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonStep,         0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), StepInputArea,      1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonUp,           0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonIn,           1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonLeft,         0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonRight,        1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonDown,         0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buttonOut,          1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), viewPort,           2, 0, 1, 5);
    gtk_grid_attach(GTK_GRID(grid), logTextArea,        2, 6, 1, 1);

    /* DEFINE BUTTONS SIGNALS */
    g_signal_connect(buttonAddObject, "clicked", G_CALLBACK(actionAddObject), (gpointer) window);
    g_signal_connect(buttonStep, "clicked", G_CALLBACK(actionMoveStep), (gpointer) window);
    g_signal_connect(buttonUp, "clicked", G_CALLBACK(actionMoveUp), (gpointer) window);
    g_signal_connect(buttonIn, "clicked", G_CALLBACK(actionMoveIn), (gpointer) window);
    g_signal_connect(buttonLeft, "clicked", G_CALLBACK(actionMoveLeft), (gpointer) window);
    g_signal_connect(buttonRight, "clicked", G_CALLBACK(actionMoveRight), (gpointer) window);
    g_signal_connect(buttonDown, "clicked", G_CALLBACK(actionMoveDown), (gpointer) window);
    g_signal_connect(buttonOut, "clicked", G_CALLBACK(actionMoveOut), (gpointer) window);

    /* SET OBJECT LIST TEXT (SET IT'S BUFFER) */
    gtk_text_buffer_set_text(objectsListTextBuffer, "Hello, this is \nsome text\nto objects list\narea", -1);

    /* SET STEP INPUT TEXT (SET IT'S BUFFER) */
    gtk_text_buffer_set_text(stepInputTextBuffer, "10", -1);

    /* SET LOG AREA TEXT (SET IT'S BUFFER) */
    gtk_text_buffer_set_text(logTextBuffer, "Hello, this is \nsome text\nto log text area", -1);

    /* DEFINE VIEW PORT SIGNALS */
    /* Signals used to handle the backing surface */
    g_signal_connect(viewPort, "draw", G_CALLBACK(draw_cb), NULL);
    g_signal_connect(viewPort, "configure-event", G_CALLBACK(configure_event_cb), NULL);
    /* Event signals */
    g_signal_connect(viewPort, "motion-notify-event", G_CALLBACK(motion_notify_event_cb), NULL);
    g_signal_connect(viewPort, "button-press-event", G_CALLBACK(button_press_event_cb), NULL);
    /* Ask to receive events the drawing area doesn't normally
     * subscribe to. In particular, we need to ask for the
     * button press and motion notify events that want to handle. */
    gtk_widget_set_events(viewPort, gtk_widget_get_events(viewPort) | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);


    //    GdkRectangle rect;
    //    gtk_text_view_get_visible_rect(GTK_TEXT_VIEW(view),&rect);
    //    gtk_container_add(GTK_CONTAINER(minha função), view);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

static void close_window(void) {
    if (surface)
        cairo_surface_destroy(surface);

    gtk_main_quit();
}

static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data) {
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);

    return FALSE;
}

static void draw_brush(GtkWidget *widget, gdouble x, gdouble y) {
    cairo_t *cr;

    /* Paint to the surface, where we store our state */
    cr = cairo_create(surface);

    cairo_rectangle(cr, x - 3, y - 3, 6, 6);
    cairo_fill(cr);

    cairo_destroy(cr);

    /* Now invalidate the affected region of the drawing area. */
    gtk_widget_queue_draw_area(widget, x - 3, y - 3, 6, 6);
}

static gboolean motion_notify_event_cb(GtkWidget *widget, GdkEventMotion *event, gpointer data) {
    /* paranoia check, in case we haven't gotten a configure event */
    if (surface == NULL)
        return FALSE;

    if (event->state & GDK_BUTTON1_MASK)
        draw_brush(widget, event->x, event->y);

    /* We've handled it, stop processing */
    return TRUE;
}

static void clear_surface(void) {
    cairo_t *cr;

    cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_destroy(cr);
}

static gboolean configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, gpointer data) {
    if (surface)
        cairo_surface_destroy(surface);

    surface = gdk_window_create_similar_surface(gtk_widget_get_window(widget),
            CAIRO_CONTENT_COLOR,
            gtk_widget_get_allocated_width(widget),
            gtk_widget_get_allocated_height(widget));

    /* Initialize the surface to white */
    clear_surface();

    /* We've handled the configure event, no need for further processing. */
    return TRUE;
}

static gboolean button_press_event_cb(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    /* paranoia check, in case we haven't gotten a configure event */
    if (surface == NULL)
        return FALSE;

    if (event->button == GDK_BUTTON_PRIMARY) {
        draw_brush(widget, event->x, event->y);
    } else if (event->button == GDK_BUTTON_SECONDARY) {
        clear_surface();
        gtk_widget_queue_draw(widget);
    }

    /* We've handled the event, stop processing */
    return TRUE;
}

static void actionAddObject(GtkWidget *w, gpointer p) {
    g_print("O botao \"Adiciona objeto\" foi clicado\n");
}

static void actionMoveStep(GtkWidget *w, gpointer p) {
    g_print("O botao \"Passo\" foi clicado\n");
}

static void actionMoveUp(GtkWidget *w, gpointer p) {
    g_print("O botao \"Up\" foi clicado\n");
}

static void actionMoveIn(GtkWidget *w, gpointer p) {
    g_print("O botao \"In\" foi clicado\n");
}

static void actionMoveLeft(GtkWidget *w, gpointer p) {
    g_print("O botao \"Left\" foi clicado\n");
}

static void actionMoveRight(GtkWidget *w, gpointer p) {
    g_print("O botao \"Right\" foi clicado\n");
}

static void actionMoveDown(GtkWidget *w, gpointer p) {
    g_print("O botao \"Down\" foi clicado\n");
}

static void actionMoveOut(GtkWidget *w, gpointer p) {
    g_print("O botao \"Out\" foi clicado\n");
}