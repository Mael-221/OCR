#include "Tools/tools.h"
#include "segmentation/histogram.h"
#include <stdio.h>
#include "pretreatment/binari.h"
#include "pretreatment/grayscale.h"
#include "pretreatment/contrast.h"
#include "pretreatment/noise.h"
#include "pretreatment/rotation.h"
#include <string.h>
#include "segmentation2/image_conversion.h"
//#include "NeuralNetwork/NN_xor.h"
#include <unistd.h>
#include <gtk/gtk.h>
#include <math.h>
#include <ctype.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <glib-object.h>
#include <gobject/gvaluecollector.h>
#include "NeuralNetwork/Ntools.h"
#include "NeuralNetwork/neural_network.h"

//Make them Global

struct Neural_Network* net = load();
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
GtkWidget *display_b;
GtkBuilder *builder;
GtkWidget *launch_OCR;
GtkWidget *train_net;
int rotationlevel = 0;
SDL_Surface *image;
SDL_Surface *screen;
SDL_Surface *debug;
int is_file_load = 0;
int is_image_display = 0;
int rota = 0;

//begin SDL

//end SDL

//Begin GTK

void on_display_b_clicked()
{
  printf("display \n");
  screen = display_image(debug);
  is_image_display = 1;
}

void on_grayscale_b_clicked()
{
  
  if(is_file_load == 1)
  {
    //printf("grascale \n");
    grayscale(image);
    grayscale(debug);
  }
  if(is_image_display == 1)
  {
    update_surface(screen,debug);
  }
}

void on_binarisation_b_clicked ()
{
  
  if(is_file_load == 1)
  {
    //printf("binari \n");
    binari(image);
    binari(debug);
  }
  if(is_image_display == 1)
  {
    update_surface(screen,debug);
  }
}

char *on_segmentation_b_clicked ()
{
  static char array[img->LetterNumbers];
  if(is_file_load == 1)
  {
    //printf("segmentation \n");
    Iimage* img = createImage(image,debug,0);

    for (int i = 0; i < img->LetterNumbers; i++)
    {
      switch (img->Letters[i].state)
      {
      case character:
        array[i] = list_to_char(net, image_conversion(Letters[i].image));
        break;
      case space:
        array[i] = ' ';
        break;
      case linebreak:
        array[i] =  '\n';
        break;
      
      default:
        break;
      }
      printf("%s", array);
    }
  }
  if(is_image_display == 1)
  {
    update_surface(screen,debug);
  }
  return array;
}

void on_noise_b_clicked ()
{
  //printf("noise \n");
  
  if(is_file_load == 1)
  {
    noise(image);
    noise(debug);
  }
  if(is_image_display == 1)
  {
    update_surface(screen,debug);
  }
}

void on_rotate_b_clicked ()
{
  //printf("rotation of %i \n",rotationlevel);
  if(is_file_load == 1)
  {
    
    image = rotation(image,rotationlevel,rota);
    debug = rotation(debug,rotationlevel,rota);
    rota = 1;
  }
  if(is_image_display == 1)
  {
    screen = display_image(debug);
  }
}

void on_contrast_up_b_clicked ()
{
  //printf("contrast up \n");
  
  if(is_file_load == 1)
  {
    contrast(image,3);
    contrast(debug,3);
  }
  if(is_image_display == 1)
  {
    update_surface(screen,debug);
  }
}

void on_contrast_down_b_clicked ()
{
  //printf("contrast down \n");
  
  if(is_file_load == 1)
  {
    contrast(image,0.3333);
    contrast(debug,0.3333);
  }
  if(is_image_display == 1)
  {
    update_surface(screen,debug);
  }
}

void on_rotation_level_changed(GtkEntry *e)
{
  //printf("rotation level = %s \n",gtk_entry_get_text(e));
  rotationlevel = atoi(gtk_entry_get_text(e));
}

void on_choose_file_file_set(GtkFileChooserButton *f)
{
  //printf("file name = %s \n",gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f)));
  image = load_image(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f)));
  debug = load_image(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f)));
  is_file_load = 1;
  rota = 0;
  
}

void on_training_neural_net_b_clicked()
{
  printf("training neural network \n");
  struct Neural_Network *net=load();
  //struct Neural_Network *net = InitializeNetwork();
  Train_Neural_Network(net);
  
}

void on_launch_OCR_b_clicked ()
{
  printf("launched OCR \n");
}
//end GTK

int main ()
{
  init_sdl();
  
  gtk_init(NULL,NULL);
  
  builder = gtk_builder_new_from_file("OCR_gui.glade");
  window = GTK_WIDGET(gtk_builder_get_object(builder,"window"));
  
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
  display_b = GTK_WIDGET(gtk_builder_get_object(builder,"display_b"));
  launch_OCR = GTK_WIDGET(gtk_builder_get_object(builder,"launch_OCR_b"));
  train_net = GTK_WIDGET(gtk_builder_get_object(builder,"training_neural_net_b"));

  g_signal_connect(grayscale_b,"clicked",G_CALLBACK(on_grayscale_b_clicked),NULL);
  g_signal_connect(binarisation_b,"clicked",G_CALLBACK(on_binarisation_b_clicked),NULL);
  g_signal_connect(segmentation_b,"clicked",G_CALLBACK(on_segmentation_b_clicked),NULL);
  g_signal_connect(noise_b,"clicked",G_CALLBACK(on_noise_b_clicked),NULL);
  g_signal_connect(rotate_b,"clicked",G_CALLBACK(on_rotate_b_clicked),GTK_ENTRY(rotation_level));
  g_signal_connect(contrast_up_b,"clicked",G_CALLBACK(on_contrast_up_b_clicked),NULL);
  g_signal_connect(contrast_down_b,"clicked",G_CALLBACK(on_contrast_down_b_clicked),NULL);
  g_signal_connect(rotation_level,"activate",G_CALLBACK(on_rotation_level_changed),rotation_level);
  g_signal_connect(choose_file,"file-set",G_CALLBACK(on_choose_file_file_set),choose_file);
  g_signal_connect(display_b,"clicked",G_CALLBACK(on_display_b_clicked),NULL);
  g_signal_connect(launch_OCR,"clicked",G_CALLBACK(on_launch_OCR_b_clicked),NULL);
  g_signal_connect(train_net,"clicked",G_CALLBACK(on_training_neural_net_b_clicked),NULL);
  
  
  gtk_widget_show(window);

  gtk_main();

  return EXIT_SUCCESS;
}






