#ifndef GUI_INTERFACE
#define GUI_INTERFACE




class GuiOptionChooser;


struct VerbListener {
  typedef std::string Verb;

  virtual void say(const GuiOptionChooser* from, Verb verb)=0;
};


#endif
