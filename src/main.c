//
// Created by andys on 23/07/2025.
//

#include <gtk/gtk.h>

#include "ui.h"

int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv);

	// Just an initialize message
	g_print("GTK initialized successfully!\n");

	show_main_window();

	gtk_main();

	return 0;
}
