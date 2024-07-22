#include <gtk/gtk.h>
#include "tokens.h"

// Prototipo de la función
const char* get_token_name(enum yytokentype token_type);

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *scroll;
    GtkWidget *grid;
    GtkWidget *label;
    int row;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Token Table");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(window), scroll);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(scroll), grid);

    label = gtk_label_new("Token Type");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    label = gtk_label_new("Token Text");
    gtk_grid_attach(GTK_GRID(grid), label, 1, 0, 1, 1);

    for (row = 0; row < token_count; row++) {
        label = gtk_label_new(g_strdup_printf("%s", get_token_name(tokens[row].type)));
        gtk_grid_attach(GTK_GRID(grid), label, 0, row + 1, 1, 1);
        label = gtk_label_new(tokens[row].text);
        gtk_grid_attach(GTK_GRID(grid), label, 1, row + 1, 1, 1);
    }

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    // Ruta estática del archivo
    const char *file_path = "/home/piero/compiladores/prueba.txt";

    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Error abriendo el archivo");
        return 1;
    }

    tokenize(file);
    fclose(file);

    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Tipo: %s, Texto: %s\n", i, get_token_name(tokens[i].type), tokens[i].text);
    }

    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), 0, NULL);  // No necesitamos pasar argumentos adicionales
    g_object_unref(app);

    return status;
}
