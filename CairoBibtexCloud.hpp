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
public:
  //============ Constructors  =================
  CairoBibtexCloud(std::vector<std::pair<std::string, int > >& c) {

    ctc = new CairoTagCloud(c);
    gl.addObject(ctc);
    gl.placeObject(ctc, 10, 50);
    gl.sizeObject(ctc, 10, 50);
    gl.showObject(ctc, true);
    gl.allowInput(ctc, true);

    options = new GuiOptionChooser(*this);
    options->setOptionHeight(1);
    options->setOptionWidth(.3);
    options->setFontSize(.8);
    options->addOption("filter");


    gl.addObject(options);
    gl.placeObject(options, 10, 10);
    gl.sizeObject(options, 10, 40);
    gl.showObject(options, true);
    gl.allowInput(options, true);

  }

  virtual ~CairoBibtexCloud(){}


  virtual void say(const GuiOptionChooser*, VerbListener::Verb verb){
    std::cout<<verb<<std::endl;
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

    gl.placeObjectY(options, 10);
    gl.sizeObjectY(options, 40);
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
