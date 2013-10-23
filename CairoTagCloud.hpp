#ifndef CAIRO_TAG_CLOUD
#define CAIRO_TAG_CLOUD

#include "cairo_graphic_controller.hpp"

class CairoTagCloud : public CairoGraphicController {
  
  std::vector<std::pair<std::string, int > > count;
  
  //  float fontsize (int occurence) {return occurence;}
  //  float fontsize (int occurence) {return 3*sqrt((float)occurence)+5;}
  float fontsize (int occurence) {return (float)occurence+5;}

  struct placement {
    float x, y, w, h;

    std::string tostring() const {
      std::stringstream ss;
      ss<<"("<<x<<", "<<y<<", "<<w<<", "<<h<<")";
      return ss.str();
    }
  };

  std::map<std::string, placement > actual_placement;

  //return true if collide
  bool check_collision(const placement& p1, const placement& p2) {
    float bl = p2.x;
    float br = p2.x+p2.w;
    float bt = p2.y+p2.h;
    float bb = p2.y;

    float al = p1.x;
    float ar = p1.x+p1.w;
    float at = p1.y+p1.h;
    float ab = p1.y;    
    
    if (bl >= ar) return false;
    if (br <= al) return false;
    if (bb >= at) return false;
    if (bt <= ab) return false;

    return true;
  }
  
  bool within(const placement& p1, float x, float y) {
    if (x< p1.x || x > p1.x+p1.w) return false;
    if (y< p1.y || y > p1.y+p1.h) return false;
    return true;
  }

  void compute_placement() {
    if (!refresh_placement) return;

    std::cout<<"compute placement"<<std::endl;
    
    refresh_placement = false;

    actual_placement.clear();
    for (auto ent : count) {
      placement p;

      bool cont = true;

      int try_count = 0;

      while (cont) {
	try_count ++;
	if (try_count == 50) break;
	cont = false;
	//compute tentative placement
	p.h = fontsize(ent.second);
	p.w = fontsize(ent.second)*ent.first.size();
	p.x = rand() % (getSizeX()-(int) p.w);
	p.y = p.h+ (rand() % ( getSizeY() - (int) p.y));

	//check for collisions
	for (auto ent : actual_placement) {
	  placement& pl = ent.second;
	  if (check_collision(p, pl))
	    cont = true;
	  if (cont) {
	    std::cout<<"collide"<<p.tostring()<<" "<<pl.tostring()<<std::endl;
	    break;
	  }
	}
      } 
      if (try_count < 50)
	actual_placement[ent.first] = p;
    }
  }

  bool refresh_placement;
  cairo_pattern_t * bgcolor;
  cairo_pattern_t * fgcolor;
  cairo_pattern_t * fgcolor_mp;

  std::map<std::string, bool> marked_plus;
public:
  
  void setTagCloud (std::vector<std::pair<std::string, int > >& c) {

    refresh_placement = true;
    count = c;
    marked_plus.clear();
    for (auto ent : count) {
      marked_plus[ent.first] = false;
    }
  }
  

  CairoTagCloud (  std::vector<std::pair<std::string, int > >& c ) {
    setTagCloud (c);

    bgcolor = cairo_pattern_create_rgb(1,1,1);
    fgcolor = cairo_pattern_create_rgb(0,0,0);
    fgcolor_mp = cairo_pattern_create_rgb(1,0,0);
  }

  virtual ~CairoTagCloud() {   
    cairo_pattern_destroy(bgcolor);
    cairo_pattern_destroy(fgcolor);
    cairo_pattern_destroy(fgcolor_mp);
  }

  virtual void setSizeX(int sx){if (sx != getSizeX()) refresh_placement = true; CairoGraphicController::setSizeX(sx);}
  virtual void setSizeY(int sy){if (sy != getSizeY()) refresh_placement = true; CairoGraphicController::setSizeY(sy); }

  
  virtual void clickat(int x, int y) {
    for (auto ent : actual_placement) {
      if (within(ent.second, x, y)) {
	marked_plus[ent.first] = ! (marked_plus[ent.first]);
	break;
      }
    }
  }
  
  virtual void clickmove(int x, int y){}

  virtual void clickrelease(int x, int y){}

  virtual void render(cairo_t* cr) {
    compute_placement();

    //paint background
    cairo_set_source(cr, bgcolor);
    cairo_paint(cr);


    //paint rectangles
    cairo_set_source(cr, fgcolor);
    for (auto pl : actual_placement) {
      
      //cairo_rectangle(cr, pl.second.x, pl.second.y, pl.second.w, pl.second.h);
      //cairo_stroke(cr);

      cairo_move_to (cr, pl.second.x, pl.second.y + pl.second.h);

      if (marked_plus[pl.first]) {
	cairo_set_source(cr, fgcolor_mp);
      }
      else {
	cairo_set_source(cr, fgcolor);
      }
      show_text (cr, pl.first, pl.second.h);
    }
  }

  virtual bool quit() const{return false;}

};


#endif
