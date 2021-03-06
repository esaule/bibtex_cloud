//This file is part of Cassis.

//Copyright (C) 2012, Erik Saule

//Cassis is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//Cassis is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with Cassis.  If not, see <http://www.gnu.org/licenses/>.

#ifndef TACT_GUI_CONTROLLER
#define TACT_GUI_CONTROLLER

#include "log.hpp"
#include "cairo_graphic_controller.hpp"
#include "gui_interface.hpp"
#include "gui_option_chooser.hpp"
#include "CairoTagCloud.hpp"
#include "GuiLayout.hpp"

class CairoBibtexCloud : public CairoGraphicController,
			 public VerbListener
{
private:
  GuiLayout gl;

  util::smart_ptr<CairoTagCloud> ctc;

  util::smart_ptr<GuiOptionChooser> options;

  std::map<std::string, std::vector<std::string> > key_to_keywords;
  std::vector<std::pair<std::string, int > > count;

  std::set<std::string> ignore_list;

  void generate_count() {
    count.clear();
    //count occurences
    std::map<std::string, int> count_m;
    
    for (auto it = key_to_keywords.begin(); it != key_to_keywords.end(); ++it) {
      for (std::string s : it->second) {
	count_m[s] ++;
      }
    }
    
    for (auto it = count_m.begin(); it != count_m.end(); ++it) {
      if (std::find (ignore_list.begin(), ignore_list.end(), it->first) == ignore_list.end())
	count.push_back(std::pair<std::string, int > (it->first, it->second));
    }
    
    std::sort (count.begin(), count.end(), [] (const std::pair<std::string, int> & a, const std::pair<std::string, int> & b) -> bool {return a.second>b.second;} );
    
    //print occurences
    for (auto it = count.begin(); it != count.end(); ++it) {
      std::cout<<it->first<<" "<<it->second<<std::endl;
    }
  }

public:
  //============ Constructors  =================
  CairoBibtexCloud(const std::map<std::string, std::vector<std::string> > & k_t_k)
    :key_to_keywords(k_t_k) {
    generate_count();

    ctc = new CairoTagCloud(count);
    gl.addObject(ctc);
    gl.placeObject(ctc, 10, 50);
    gl.sizeObject(ctc, 10, 50);
    gl.showObject(ctc, true);
    gl.allowInput(ctc, true);

    options = new GuiOptionChooser(*this);
    options->setOptionHeight(1);
    options->setOptionWidth(.15);
    options->setInterWidth(.20);
    options->setOptionWidthInternal(.02);
    options->setFontSize(.8);
    options->addOption("Filter");
    options->addOption("ZoomIn");
    options->addOption("ZoomOut");
    options->addOption("Ignore");
    options->addOption("Print");

    gl.addObject(options);
    gl.placeObject(options, 10, 10);
    gl.sizeObject(options, 5, 45);
    gl.showObject(options, true);
    gl.allowInput(options, true);
  }

  virtual ~CairoBibtexCloud(){}


  virtual void say(const GuiOptionChooser*, VerbListener::Verb verb) {
    std::cout<<verb<<std::endl;
    if (verb.compare("Filter") == 0) {
      std::set<std::string> marked;
      ctc->getMarkedPlus(marked);
      
      //filter out all entries that do not have a tag in the marked set.
      for (auto it = key_to_keywords.begin(); it != key_to_keywords.end(); ) {
	bool erase = true;
	
	for (std::string ent : it->second) {
	  if (std::find(marked.begin(), marked.end(), ent) != marked.end()) {
	    erase = false;
	    break;
	  }
	}

	if (erase) {
	  it = key_to_keywords.erase(it);
	}
	else {
	  ++it;
	}
      }
      
      //update display
      generate_count();
      ctc->setTagCloud(count);

      //printout list
      std::cout<<"==================================================="<<std::endl;
      for (auto ent : key_to_keywords) {
	std::cout<<ent.first<<"=>"; 
	for (std::string &s : ent.second)
	  std::cout<<s<<"|";
	std::cout<<std::endl;
      }
      std::cout<<"==================================================="<<std::endl;
    }
    if (verb.compare("ZoomIn") == 0) {
      ctc->scaleUp();
    }

    if (verb.compare("ZoomOut") == 0) {
      ctc->scaleDown();
    }

    if (verb.compare("Ignore") == 0) {
      std::set<std::string> marked;
      ctc->getMarkedPlus(marked);
      ignore_list.insert (marked.begin(), marked.end());
      
      //update display
      generate_count();
      ctc->setTagCloud(count);
    }
    if (verb.compare("Print") == 0) {
      std::set<std::string> output;
      for (auto ent : key_to_keywords) {
	output.insert(ent.first);
      }
      print_to_stdout(output);
    }
  }

  //==========Rendering stuff===========
  virtual void render(cairo_t* cr){
    gl.render(cr);
  }

  virtual void setSizeX(int sx)
  {
    gl.placeObjectX(ctc, 10);
    gl.sizeObjectX(ctc, sx-10);

    gl.placeObjectX(options, 10);
    gl.sizeObjectX(options, sx-10);
  }

  virtual void setSizeY(int sy)
  {
    gl.placeObjectY(ctc, 50);
    gl.sizeObjectY(ctc, sy-50);

    gl.placeObjectY(options, 5);
    gl.sizeObjectY(options, 45);
  }


  //==========Input stuff===========
  virtual void clickat(int x, int y) {
    gl.clickat(x,y);
  }
  
  virtual void clickmove(int x, int y) {
    gl.clickmove(x,y);
  }

  virtual void clickrelease(int x, int y) {
    gl.clickrelease(x,y);
  }


  //==========Management stuff===========
  virtual bool quit() const{return false;}


  //==========Serialization stuff===========
  virtual void deserialize(const char* ) 
  {
    std::cerr<<"NYE"<<std::endl;
  }

  virtual void serialize(char* ) const
  {
    std::cerr<<"NYE"<<std::endl;
  }
  
  virtual size_t serializesize() const
  {
    std::cerr<<"NYE"<<std::endl;
    return 1;
  }

};


#endif
