#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
using namespace std;

const int w_width = 640;
const int w_height= 480;
const int w_cout = 1;


bool initialize();
bool load_data();
void exit();


SDL_Window* window = NULL;
SDL_Surface* win_surface = NULL;
SDL_Renderer* win_renderer = NULL;

class Platform{

    public:
        int x;
        int y;
        SDL_Rect pl_rect;

        Platform( int x_, int y_ ){
            x = x_;
            y = y_;
            pl_rect = {x, y, 10, 10};
        }
        ~Platform(){}
        void draw( SDL_Renderer* renderer ){
            SDL_SetRenderDrawColor( renderer, 255, 255, 0, 255 );
            SDL_RenderFillRect( renderer, &pl_rect );
        }
};

class Player{

    public:
        int x;
        int y;
        SDL_Rect pl_rect;
    
        Player( int x_, int y_ ){
            x = x_;
            y = y_;
            pl_rect = {x, y, 40, 40};
        }
        ~Player(){}
        void draw( SDL_Renderer* renderer ){

            pl_rect.x = x;
            pl_rect.y = y;

            SDL_SetRenderDrawColor( renderer, 255, 0, 0, 0);
            SDL_RenderFillRect( renderer, &pl_rect );
        }

};

int main( int argn, char* args[] ){
    if (!initialize()){
        cerr<<"Hubo un error durante la inicialización: "<<SDL_GetError()<<endl;
        return -1;
    }

    //Main loop variables
    bool quit = false;
    SDL_Event e;

    Player player = Player( 10, 10 );
    vector<Platform> platforms;
    float accelX = 0, speedX = 0, max_speedX = 10;
    float accelY = 0, speedY = 0, max_speedY = 10;
    short directionX = 0, directionY = 0;
    SDL_Rect player_rect = {10, 10, 40, 40};
    SDL_Rect intersect_rect;

    platforms.emplace_back( 300, 240 );
    platforms.emplace_back( 310, 240 );
    platforms.emplace_back( 320, 240 );
    platforms.emplace_back( 330, 240 );
    platforms.emplace_back( 340, 240 );
    platforms.emplace_back( 350, 240 );

    platforms.emplace_back( 300, 120 );
    platforms.emplace_back( 310, 120 );
    platforms.emplace_back( 320, 120 );
    platforms.emplace_back( 330, 120 );
    platforms.emplace_back( 340, 120 );
    platforms.emplace_back( 350, 120 );

    platforms.emplace_back( 300, 360 );
    platforms.emplace_back( 310, 360 );
    platforms.emplace_back( 320, 360 );
    platforms.emplace_back( 330, 360 );
    platforms.emplace_back( 340, 360 );
    platforms.emplace_back( 350, 360 );

    while (!quit){

        SDL_Delay(16);

        while ( SDL_PollEvent(&e) ){
            switch (e.type){
                case SDL_KEYDOWN:
                    if ( e.key.keysym.sym == SDLK_ESCAPE ){
                        case SDL_QUIT:
                        quit = true;
                        break;
                    }
                    if ( e.key.keysym.sym == SDLK_d ){
                        accelX = 2;
                        directionX = 1;
                    }
                    if ( e.key.keysym.sym == SDLK_a ){
                        accelX = 2;
                        directionX = -1;
                    }
                    if ( e.key.keysym.sym == SDLK_s ){
                        accelY = 2;
                        directionY = 1;
                    }
                    if ( e.key.keysym.sym == SDLK_w ){
                        accelY = 2;
                        directionY = -1;
                    }
                break;
                case SDL_KEYUP:
                    if ( e.key.keysym.sym == SDLK_d && directionX > 0){
                        accelX = -5;
                    }
                    if ( e.key.keysym.sym == SDLK_a && directionX < 0){
                        accelX = -5;
                    }
                    if ( e.key.keysym.sym == SDLK_w && directionY < 0){
                        accelY = -5;
                    }
                    if ( e.key.keysym.sym == SDLK_s && directionY > 0){
                        accelY = -5;
                    }
            }
        }

        SDL_SetRenderDrawColor( win_renderer, 0, 0, 0, 255 );
        SDL_RenderClear(win_renderer);

        //Movimiento del jugador
        if ( speedX <= max_speedX )
            speedX += accelX;
        else if ( speedX > max_speedX )
            speedX = max_speedX;
        if ( speedX < 0 ){
            speedX = 0;
            directionX = 0;
            accelX = 0;
        }

        if ( speedY <= max_speedY )
            speedY += accelY;
        else if ( speedY > max_speedY )    
            speedY = max_speedY;
        if ( speedY < 0 ){
            speedY = 0;
            directionY = 0;
            accelY = 0;
        }

        player.x += speedX * directionX;
        player.y += speedY * directionY;


        for( Platform i : platforms ){
            i.draw( win_renderer );
            if( SDL_IntersectRect( &player.pl_rect, &i.pl_rect, &intersect_rect ) ){
                speedX = intersect_rect.w;
                accelX = 0;
                speedY = intersect_rect.h;
                accelY = 0;
                
                player.x += speedX * -directionX;
                player.y += speedY * -directionY;

                
            }
        }

        player.draw( win_renderer );

        SDL_RenderPresent(win_renderer);

    }


    exit();
    return 0;
}

//Inicializa las variables globales como la ventana y los modulos SDL
bool initialize(){
    bool  success = true;
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ){
        cerr<<"Hubo un error al iniciar SDL: "<<SDL_GetError()<<endl;
        success = false;
    }else{
        window = SDL_CreateWindow( 
            "SDL Level and movement test",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            w_width,
            w_height,
            SDL_WINDOW_SHOWN
        );
        if (window != NULL){
            win_surface = SDL_GetWindowSurface(window);
            win_renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
            if ( win_renderer == NULL){
                cerr<< "No se pudo crear el renderer: "<<SDL_GetError()<<endl;
                success = false;
            } else
                SDL_SetRenderDrawColor( win_renderer, 0, 0, 0, 255 );
        }
    }
    return success;
}

void exit(){
    SDL_FreeSurface(win_surface);
    win_surface = NULL;

    SDL_DestroyWindow(window);
    window = NULL;
    
    SDL_Quit();
}