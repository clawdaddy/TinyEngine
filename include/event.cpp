/*
This class handles user input, creates an appropriate stack of activated events and handles
them so that user inputs have continuous effect.
*/

class Event{
private:
  SDL_Event in;

public:
  //Global Flags
  bool quit = false;
  void input();                   //Take inputs and add them to stack

  //Event Handlers
  void handle(View &view);  //General Event Handler
  Handle handler; //User defined event Handler

  //Special Actions
  bool fullscreenToggle = false;

  //Queued Inputs
  std::deque<SDL_Event> inputs;  //General Key Inputs
  std::deque<SDL_Event> scroll;  //Scrolling Motion Inputs
  SDL_Event windowEvent;         //Window Resizing Event
  bool windowEventTrigger;
  SDL_Event mouseEvent;          //Mouse Click Event
  bool mouseEventTrigger;
  SDL_Event moveEvent;           //Mouse Movement Events
  bool moveEventTrigger;
};

void Event::input(){
  if(SDL_PollEvent(&in) == 0) return;

  if(in.type == SDL_QUIT) quit = true;
  ImGui_ImplSDL2_ProcessEvent(&in);

  //Keyboard Inputs
  if(in.type == SDL_KEYDOWN){
    if(in.key.keysym.sym == SDLK_F11) fullscreenToggle = true;
    else inputs.push_front(in);
    return;
  }

  //Scrolling Inputs
  if(in.type == SDL_MOUSEWHEEL){
    scroll.push_front(in);
    return;
  }

  //Mouse Click Inputs
  if(in.type == SDL_MOUSEBUTTONDOWN ||
     in.type == SDL_MOUSEBUTTONUP){
       mouseEvent = in;
       mouseEventTrigger = true;
       return;
  }

  //Mouse Move Inputs
  if(in.type == SDL_MOUSEBUTTONDOWN ||
     in.type == SDL_MOUSEBUTTONUP){
       moveEvent = in;
       moveEventTrigger = true;
       return;
  }

  //Window Resize
  if(in.type == SDL_WINDOWEVENT){
    windowEvent = in;
    windowEventTrigger = true;
    return;
  }
}

void Event::handle(View &view){
  //Call the user-defined handler!
  (handler)();

  //Do standard event handling stuff
  if(fullscreenToggle){
    view.fullscreen = !view.fullscreen; //Toggle Fullscreen!
    if(!view.fullscreen) SDL_SetWindowFullscreen(view.gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
    else SDL_SetWindowFullscreen(view.gWindow, 0);
    fullscreenToggle = false;
  }

  //Window Resize Event
  if(windowEventTrigger && windowEvent.window.event == SDL_WINDOWEVENT_RESIZED){
    view.WIDTH = windowEvent.window.data1;
    view.HEIGHT = windowEvent.window.data2;
    windowEventTrigger = false;
  }

  //Toggle Interface with Escape
  if(in.type == SDL_KEYDOWN && in.key.keysym.sym == SDLK_ESCAPE){
    view.showInterface = !view.showInterface;
  }

  //Remove old Events
  if(!inputs.empty()) inputs.pop_back();
  if(!scroll.empty()) scroll.pop_back();
}
