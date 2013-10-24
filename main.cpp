#define GTK
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "parse.hpp"

#include "util.hpp"

#include <gtk/gtk.h>

#include "cairo.h"


GdkImage *im;
GtkWidget *imWind;


//#include "CairoTagCloud.hpp"
#include "CairoBibtexCloud.hpp"

bool quit;

void
destroy (void)
{
  quit = true;
  gtk_main_quit ();
}


int main (int argc, char* argv[]) {
  if (argc!=2) {
    std::cerr<<"Usage :"<<argv[0]<<" <filename>"<<std::endl;
    return -1;
  }

  std::map<std::string, std::vector<std::string> > key_to_keywords;

  extract_key_keywords(argv[1], key_to_keywords);

  //put everything to lower case
  for (auto it = key_to_keywords.begin(); it != key_to_keywords.end(); ++it) {
    for (std::string &s : it->second)
      s = str_tolower(s);
  }

  

  //print
  for (auto it = key_to_keywords.begin(); it != key_to_keywords.end(); ++it) {
    std::cout<<it->first<<"=>"; 
    for (std::string &s : it->second)
      std::cout<<s<<"|";
    std::cout<<std::endl;
  }

  //count occurences
  std::map<std::string, int> count_m;
  
  for (auto it = key_to_keywords.begin(); it != key_to_keywords.end(); ++it) {
    for (std::string s : it->second) {
      count_m[s] ++;
    }
  }

  std::vector<std::pair<std::string, int > > count;
  //print occurences
  for (auto it = count_m.begin(); it != count_m.end(); ++it) {
    count.push_back(std::pair<std::string, int > (it->first, it->second));
  }
  
  std::sort (count.begin(), count.end(), [] (const std::pair<std::string, int> & a, const std::pair<std::string, int> & b) -> bool {return a.second>b.second;} );

  //print occurences
  for (auto it = count.begin(); it != count.end(); ++it) {
    std::cout<<it->first<<" "<<it->second<<std::endl;
  }



  //============================================================
  //GTK stuff

  gtk_init (&argc, &argv);
  GtkWidget *window;

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  g_set_application_name("cairo gtk");

  gtk_signal_connect (GTK_OBJECT (window), "destroy",
                      GTK_SIGNAL_FUNC (destroy), NULL);
  gtk_container_border_width (GTK_CONTAINER (window), 10);

  
  gtk_widget_add_events (window, GDK_BUTTON_RELEASE_MASK);
  gtk_widget_add_events (window, GDK_POINTER_MOTION_MASK);

  //  CairoTagCloud g (count);
  CairoBibtexCloud g (count);
  
    imWind = gtk_label_new("You should not see this message!");

  g_signal_connect (G_OBJECT (imWind), "expose_event",
                    G_CALLBACK (CairoGraphicController::expose), &g);


  g_signal_connect (G_OBJECT (window), "key-press-event",
                    G_CALLBACK (CairoGraphicController::key_press), &g);
  
  g_signal_connect (G_OBJECT (window), "button-press-event",
                    G_CALLBACK (CairoGraphicController::button_press), &g);
  
  g_signal_connect (G_OBJECT (window), "button-release-event",
                    G_CALLBACK (CairoGraphicController::button_release), &g);

  g_signal_connect (G_OBJECT (window), "motion-notify-event",
                    G_CALLBACK (CairoGraphicController::motion_notify), &g);
  
  
  gtk_widget_add_events(window, GDK_VISIBILITY_NOTIFY_MASK);

  g_signal_connect (G_OBJECT (window), "visibility-notify-event",
                    G_CALLBACK (CairoGraphicController::visibility_notify), &g);

  GTK_WIDGET_SET_FLAGS(window, GTK_CAN_FOCUS);
  gtk_widget_add_events (window, GDK_KEY_PRESS_MASK);
  gtk_widget_add_events (window, GDK_BUTTON_PRESS_MASK);



  gtk_widget_show (imWind);

  gtk_container_add(GTK_CONTAINER(window), imWind);
  gtk_widget_show (window);

  //  gtk_main ();
  quit = false;
  while (!quit)
    {
      gtk_main_iteration_do(false);

      int microsecondsinasecond = 1000*1000;
      usleep(microsecondsinasecond/100);
      //usleep(10000);
      gtk_widget_queue_draw (imWind);
    }



  return 0;
}
