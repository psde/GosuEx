#FpsCounter
A simple fps counter extension for gosu

![Screenshot.](http://bayimg.com/image/makemaacf.jpg)

#Usage
 * FpsCounter::update() should be called after your scene is drawn and before FpsCounter::draw()
 * FpsCounter::draw()  is not necessary for the calculation of the current fps
 
#Example usage
 
    class Window : public Gosu::Window
    {
    private:
        GosuEx::FpsCounter fps;
        
    public:
        Window()
            :	Gosu::Window(1024, 768, false), fps(&graphics(), 1024-210, 10, 200, 100)
        {
            [...]
        }

        void draw()
        {
            [draw your scene here]
            fps.updateFPS();
            fps.draw();
        }
    }
