//
// Source File:		ui.c
// Path:			src/ui
// Project Name:	GTK Calculator
// Created by Andrew Holden on 23/07/2025.
//

#include "ui.h"
#include "logic.h"

#include <stdlib.h>
#include <string.h>

static GtkWidget *input_entry;
static GtkWidget *result_label;

// Update user input entry display with current buffer
static void update_input_display(void) {
	gtk_entry_set_text(GTK_ENTRY(input_entry), get_input());
}

/*
 *	---------------------------------------------------------------------------
 *	CALLBACKS
 *	---------------------------------------------------------------------------
 */

// CB: Handle button event for digits and operations
static void handle_input_button(GtkButton *button, gpointer user_data) {
	const char *label = gtk_button_get_label(button);
	if (label && strlen(label) == 1) {
		append_input(label[0]);
		update_input_display();
	}
}

// CB: Handle button event for equals
static void handle_equal_button(GtkButton *button, gpointer user_data) {
	const char *input = get_input();
	const char *output = eval_expression(input);

	if (strncmp(output, "Error:", 6) == 0) {
		gtk_label_set_text(GTK_LABEL(result_label), output);
	}
	else {
		gtk_label_set_text(GTK_LABEL(result_label), output);
		reset_logic_state();
	}
	gtk_entry_set_text(GTK_ENTRY(input_entry), "");
}

// CB: Handle button event for clear
static void handle_clear_button(GtkButton *button, gpointer user_data) {
	reset_logic_state();
	clear_input();
	gtk_entry_set_text(GTK_ENTRY(input_entry), "");
	gtk_label_set_text(GTK_LABEL(result_label), "Result: ");
}

// CB: Handle button event for delete/remove character from buffer
static void handle_del_button(GtkButton *button, gpointer user_data) {
	g_print("REMOVE ME\n");
}

// CB: Handle button event for memory ops.
static void handle_memory_button(GtkButton *button, gpointer user_data) {
	g_print("REMOVE ME\n");
}

// CB: Handle button event for square root
static void handle_sqrt_button(GtkButton *button, gpointer user_data) {
	g_print("REMOVE ME\n");
}

// CB: Handle button event for percentage
static void handle_PLACEHOLDER(GtkButton *button, gpointer user_data) {
	g_print("REMOVE ME\n");
}

// CB: Handle button event for toggle numeric sign
static void handle_togsign_button(GtkButton *button, gpointer user_data) {
	g_print("REMOVE ME\n");
}

// CB: Handle button event for power of two
static void handle_power_of_two_button(GtkButton *button, gpointer user_data) {
	g_print("REMOVE ME\n");
}

/*
 *	---------------------------------------------------------------------------
 *	STYLE WIDGETS
 *	---------------------------------------------------------------------------
 */



/*
 *	---------------------------------------------------------------------------
 *	TOPLEVEL WINDOW CONFIG
 *	---------------------------------------------------------------------------
 */

void show_main_window(void) {
	// CSS provider
	GtkCssProvider *css_provider = gtk_css_provider_new();

	GdkDisplay *display = gdk_display_get_default();
	GdkScreen *screen = gdk_display_get_default_screen(display);

	gtk_style_context_add_provider_for_screen(
		screen,
		GTK_STYLE_PROVIDER(css_provider),
		GTK_STYLE_PROVIDER_PRIORITY_USER
		);

	GError *css_error = NULL;
	gtk_css_provider_load_from_path(css_provider, "style.css", &css_error);
	if (css_error) {
		g_printerr("CSS Load Error: %s\n", css_error->message);
		g_error_free(css_error);
	}

	// Generate main window
	GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(main_window), "Retro Calculator GTK");
	//gtk_window_set_default_size(GTK_WINDOW(main_window), 360, 720);

	g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Generate parent container box
	GtkWidget *top_vbox_main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_style_context_add_class(gtk_widget_get_style_context(top_vbox_main), "parent-container");
	// Add top level vbox to main_window
	gtk_container_add(GTK_CONTAINER(main_window), top_vbox_main);

	// Generate line graphic box
	GtkWidget *line_box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_style_context_add_class(gtk_widget_get_style_context(line_box1), "sub-line");
	gtk_box_pack_start(GTK_BOX(top_vbox_main), line_box1, FALSE, FALSE, 0);

	// Generate header text label
	GtkWidget *title_label = gtk_label_new("RC-GTK");
	gtk_widget_set_halign(title_label, GTK_ALIGN_START);
	gtk_style_context_add_class(gtk_widget_get_style_context(title_label), "title-label");
	gtk_box_pack_start(GTK_BOX(top_vbox_main), title_label, FALSE, FALSE, 0);

	// Generate status indicator box
	GtkWidget *status_light_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
	gtk_style_context_add_class(gtk_widget_get_style_context(status_light_box), "status-light-box");
	GtkWidget *status_light_active = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_style_context_add_class(gtk_widget_get_style_context(status_light_active), "status-light");
	gtk_style_context_add_class(gtk_widget_get_style_context(status_light_active), "active");
	//
	GtkWidget *status_light_inactive = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	//gtk_style_context_add_class(gtk_widget_get_style_context(status_light_inactive), "status-light");
	//
	gtk_box_pack_start(GTK_BOX(status_light_box), status_light_active, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(status_light_box), status_light_inactive, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(top_vbox_main), status_light_box, FALSE, FALSE, 0);

	// Generate graphics box
	GtkWidget *graphics_top_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_style_context_add_class(gtk_widget_get_style_context(graphics_top_box), "graphics-box");
	gtk_box_pack_start(GTK_BOX(top_vbox_main), graphics_top_box, FALSE, FALSE, 0);

	/*
	 *	DISPLAY PARENT BOX:
	 *		L> Display bezel/bounding box
	 *			L> Display box
	 *				L> Result label
	 *				L> Input entry
	 *				L> Scanline overlay box
	 */

	// Generate parent display box
	GtkWidget *top_display_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_style_context_add_class(gtk_widget_get_style_context(top_display_box), "top-display-box");
	gtk_box_pack_start(GTK_BOX(top_vbox_main), top_display_box, FALSE, FALSE, 0);

	// Generate display bounding box
	GtkWidget *display_bounding_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_style_context_add_class(gtk_widget_get_style_context(display_bounding_box), "display-bounding-box");
	gtk_box_pack_start(GTK_BOX(top_display_box), display_bounding_box, TRUE, TRUE, 0);

	// Generate display box
	GtkWidget *display_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_style_context_add_class(gtk_widget_get_style_context(display_box), "display-box");
	gtk_box_pack_start(GTK_BOX(display_bounding_box), display_box, TRUE, TRUE, 0);

	// --> Entry field box using result_label
	result_label = gtk_label_new("Result: ");
	gtk_style_context_add_class(gtk_widget_get_style_context(result_label), "result-label");
	gtk_label_set_xalign(GTK_LABEL(result_label), 1.0);
	gtk_label_set_yalign(GTK_LABEL(result_label), 1.0);
	gtk_box_pack_start(GTK_BOX(display_box), result_label, TRUE, TRUE, 0);

	// --> Input entry box using input_entry
	input_entry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(input_entry), "Enter expression...");
	gtk_entry_set_alignment(GTK_ENTRY(input_entry), 1.0);  // Right align
	gtk_style_context_add_class(gtk_widget_get_style_context(input_entry), "entry-label");
	gtk_box_pack_start(GTK_BOX(display_box), input_entry, TRUE, TRUE, 0);

	// Generate scanline overlay box
	GtkWidget *scanline_overlay_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_style_context_add_class(gtk_widget_get_style_context(scanline_overlay_box), "scan-overlay-box");
	gtk_box_pack_start(GTK_BOX(display_box), scanline_overlay_box, TRUE, TRUE, 0);

	// ---

	// Generate screen/key area separator
	GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_box_pack_start(GTK_BOX(top_vbox_main), separator, FALSE, FALSE, 5);

	// Generate key area grid box
	GtkWidget *key_area_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_style_context_add_class(gtk_widget_get_style_context(key_area_box), "key-grid-box");
	gtk_box_pack_start(GTK_BOX(top_vbox_main), key_area_box, TRUE, TRUE, 0);

	// Generate grid; digit matrix, operation buttons etc.
	GtkWidget *button_grid = gtk_grid_new();
	gtk_style_context_add_class(gtk_widget_get_style_context(button_grid), "buttons-grid");
	gtk_grid_set_row_spacing(GTK_GRID(button_grid), 10);
	gtk_grid_set_column_spacing(GTK_GRID(button_grid), 10);

	const char *buttons[5][6] = {
		{"CLR", "(", ")", "DEL", "/", "*"},
		{"7", "8", "9", "-", "MS", "MR"},
		{"4", "5", "6", "+", "MC", "\u221A"},
		{"1", "2", "3", "0", "", "."},
		{"=", "", "%", "\u00B1", "x\u00B2", "ON/C"}
	};

	for (int row = 0; row < 5; row++) {
		for (int col = 0; col < 6; col++) {
			// empty cells to be skipped!
			const char *btn_ptr = buttons[row][col];
			if (btn_ptr[0] == '\0') continue;

			GtkWidget *btn = gtk_button_new_with_label(btn_ptr);

			gtk_style_context_add_class(gtk_widget_get_style_context(btn), "btn");

			if (strcmp(btn_ptr, "0") == 0 || strcmp(btn_ptr, "=") == 0) {
				gtk_grid_attach(GTK_GRID(button_grid), btn, col, row, 2, 1);
				col++;
				// 0
				if (strcmp(btn_ptr, "0") == 0) {
					gtk_style_context_add_class(gtk_widget_get_style_context(btn), "btn-dark");
					g_signal_connect(btn, "clicked", G_CALLBACK(handle_input_button), NULL);
				}
				// =
				else {
					gtk_style_context_add_class(gtk_widget_get_style_context(btn), "btn-orange");
					g_signal_connect(btn, "clicked", G_CALLBACK(handle_equal_button), NULL);
				}
			}
			// CLR
			else if (strcmp(btn_ptr, "CLR") == 0) {
				gtk_grid_attach(GTK_GRID(button_grid), btn, col, row, 1, 1);
				gtk_style_context_add_class(gtk_widget_get_style_context(btn), "btn-orange");
				g_signal_connect(btn, "clicked", G_CALLBACK(handle_clear_button), input_entry);
			}
			// DEL
			else if (strcmp(btn_ptr, "DEL") == 0) {
				gtk_grid_attach(GTK_GRID(button_grid), btn, col, row, 1, 1);
				gtk_style_context_add_class(gtk_widget_get_style_context(btn), "btn-orange");
				g_signal_connect(btn, "clicked", G_CALLBACK(handle_del_button), input_entry);
			}
			// numeric and others
			else {
				gtk_grid_attach(GTK_GRID(button_grid), btn, col, row, 1, 1);
				gtk_style_context_add_class(gtk_widget_get_style_context(btn), "btn-dark");
				g_signal_connect(btn, "clicked", G_CALLBACK(handle_input_button), NULL);
			}
		}
	}
	gtk_box_pack_start(GTK_BOX(top_vbox_main), button_grid, FALSE, FALSE, 0);

	// Generates footer version label box
	GtkWidget *version_label = gtk_label_new("v0.0.1");
	gtk_widget_set_halign(version_label, GTK_ALIGN_END);
	gtk_style_context_add_class(gtk_widget_get_style_context(version_label), "version-label");
	gtk_box_pack_start(GTK_BOX(top_vbox_main), version_label, TRUE, TRUE, 0);

	gtk_widget_show_all(main_window);
}