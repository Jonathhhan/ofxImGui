#include "ofxImGui.h"

ofxImGui::ofxImGui()
{
    lastTime = 0.0f;
    engine = NULL;
    io = NULL;
}

void ofxImGui::setup(BaseTheme* theme_)
{
    io = &ImGui::GetIO();

    io->DisplaySize = ImVec2((float)ofGetWidth(), (float)ofGetHeight());
    io->MouseDrawCursor = false;

#if defined(TARGET_OPENGLES)
    engine = new EngineOpenGLES();
#else  
    engine = new EngineGLFW();
#endif

    engine->setup(io);
    if(theme_)
    {
       setTheme(theme_);
    }else
    {
        setTheme(new BaseTheme());
    }
    
    

}
void ofxImGui::setTheme(BaseTheme* theme_)
{
    if(theme)
    {
        delete theme;
        theme = NULL;
    }
    theme = theme_;
    theme->updateColors();
}

void ofxImGui::openThemeColorWindow()
{
    theme->themeColorsWindow(true);
}

GLuint ofxImGui::loadPixels(ofPixels& pixels)
{
    return engine->loadTextureImage2D(pixels);
}

GLuint ofxImGui::loadPixels(string imagePath)
{
    if(!engine) return -1;
    ofPixels pixels;
    ofLoadImage(pixels, imagePath);
    return loadPixels(pixels);
}

GLuint ofxImGui::loadImage(ofImage& image)
{
    if(!engine) return -1;
    return loadPixels(image.getPixels());
}

GLuint ofxImGui::loadImage(string imagePath)
{
    return loadPixels(imagePath);
}

void ofxImGui::begin()
{
    if(!io || !engine)
    {
        ofLogError() << "setup call required - calling it for you";
        setup();
    }
    float currentTime = ofGetElapsedTimef();
    if(lastTime > 0.f)
    {
        io->DeltaTime = currentTime - lastTime;
    }else
    {
        io->DeltaTime = 1.0f / 60.f;
    }
    lastTime = currentTime;
    

    io->MousePos = ImVec2((float)ofGetMouseX(), (float)ofGetMouseY());
    ImGui::NewFrame();
}

void ofxImGui::end()
{
    ImGui::Render();
}


 
void ofxImGui::close()
{
    if(engine)
    {
        delete engine;
        engine = NULL;
    }
    if(io)
    {
        io->Fonts->TexID = 0;
        io = nullptr;
    }
    if(theme)
    {
        delete theme;
        theme = NULL;
    }
}

ofxImGui::~ofxImGui()
{
    close();
    ImGui::Shutdown();
}
