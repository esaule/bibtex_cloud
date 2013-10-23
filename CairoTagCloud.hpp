#ifndef CAIRO_TAG_CLOUD
#define CAIRO_TAG_CLOUD

#include "cairo_graphic_controller.hpp"

class CairoTagCloud : public CairoGraphicController {
  
  std::vector<std::pair<std::string, int > > count;
  
  float fontsize (int occurence) {return occurence;}

  struct placement {
    float x, y, w, h;
  };

  std::map<std::string, placement > actual_placement;
  
  void compute_placement() {
    actual_placement.clear();
    for (auto ent : count) {
      
    }
  }

public:
  
  CairoTagCloud (  std::vector<std::pair<std::string, int > >& c )
    :count(c) {
    compute_placement();
  }
  
  virtual void clickat(int x, int y){}
  
  virtual void clickmove(int x, int y){}

  virtual void clickrelease(int x, int y){}

  virtual void render(cairo_t* cr){}

  virtual bool quit() const{return false;}

};


#endif
