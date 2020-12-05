#include "Tools/tools.h"
#include "segmentation/histogram.h"
#include <stdio.h>
#include "pretreatment/binari.h"
#include "pretreatment/grayscale.h"
#include "pretreatment/contrast.h"
#include "pretreatment/noise.h"
#include <string.h>
#include "segmentation2/segmentation.h"
#include "NeuralNetwork/NN_xor.h"
#include <unistd.h>
#include <gtk/gtk.h>
#include <math.h>
#include <ctype.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


//Make them Global

GtkWidget *window;
GtkWidget *fixed1;
GtkWidget *grayscale_b;
GtkWidget *binarisation_b;
GtkWidget *segmentation_b;
GtkWidget *noise_b;
GtkWidget *rotate_b;
GtkWidget *contrast_up_b;
GtkWidget *contrast_down_b;
GtkWidget *choose_file;
GtkWidget *rotation_level;
GtkBuilder *builder;

int main (int argc, char *argv[])
{
  gtk_init(&argc,&argc);
  
  builder = gtk_builder_new_from_file("OCR_gui.glade");
  window = GTK_WIDGET(gtk_builder_get_object(builder,window));
  
  g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
  
  gtk_builder_connect_signals(builder,NULL);

  //connection to widgets in the glade file
  fixed1 = GTK_WIDGET(gtk_builder_get_object(builder,"fixed1"));
  grayscale_b = GTK_WIDGET(gtk_builder_get_object(builder,"grayscale_b"));
  binarisation_b = GTK_WIDGET(gtk_builder_get_object(builder,"binarisation_b"));
  segmentation_b = GTK_WIDGET(gtk_builder_get_object(builder,"segmentation_b"));
  noise_b = GTK_WIDGET(gtk_builder_get_object(builder,"noise_b"));
  rotate_b = GTK_WIDGET(gtk_builder_get_object(builder,"rotate_b"));
  contrast_up_b = GTK_WIDGET(gtk_builder_get_object(builder,"contrast_up_b"));
  contrast_down_b = GTK_WIDGET(gtk_builder_get_object(builder,"contrast_down_b"));
  choose_file = GTK_WIDGET(gtk_builder_get_object(builder,"choose_file"));
  rotation_level = GTK_WIDGET(gtk_builder_get_object(builder,"rotation_level"));

  gtk_widget_show(window);

  gtk_main();

  return EXIT_SUCCESS;
}

void on_grayscale_b_clicked (GtkButton *b)
{
  printf("grascale \n");
}

void on_binarisation_b_clicked (GtkButton *b)
{
  printf("binari \n");
}

void on_segmentation_b_clicked (GtkButton *b)
{
  printf("segmentation \n");
}

void on_noise_b_clicked (GtkButton *b)
{
  printf("noise \n");
}

void on_rotate_b_clicked (GtkButton *b)
{
  printf("rotate \n");
}

void on_contrast_up_b_clicked (GtkButton *b)
{
  printf("contrast up \n");
}

void on_contrast_down_b_clicked (GtkButton *b)
{
  printf("contrast down \n");
}




